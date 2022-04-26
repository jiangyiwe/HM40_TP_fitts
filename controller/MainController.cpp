#include "MainController.h"

MainController::MainController() {
    this->fittsModel = new FittsModel();
    this->resultsModel = new ResultsModel();
    this->applyConfig();
    this->mainView = new MainView(this);
    this->start();
    this->startSimulation();
}

MainController::~MainController(){}

void MainController::start() {
    this->mainView->show();
    this->mainView->fittsView->resizeScene();
}

void MainController::startSimulation() {
    this->mainView->fittsView->graphicWidget->setEnabled(true);
    this->initGame();
}

void MainController::quit() {
    QApplication::quit();
}

void MainController::aSpinChanged(double value) {
    this->fittsModel->a = value;
}

void MainController::bSpinChanged(double value) {
    this->fittsModel->b = value;
}

void MainController::nbCibleSpinChanged(int value) {
    this->fittsModel->nbCible = value;
}

void MainController::minSizeSpinChanged(int value) {
    this->mainView->fittsView->updateValMinOfMaxSize();
    this->fittsModel->minSize = value;
}

void MainController::maxSizeSpinChanged(int value) {
    this->mainView->fittsView->updateValMaxOfMinSize();
    this->fittsModel->maxSize = value;
}

void MainController::cibleClicked(int x, int y) {
    if(this->fittsModel->cercleCenter.isEmpty()) {
        this->fittsModel->cibleLeft = this->fittsModel->nbCible;
        this->mainView->fittsView->disableAllSpins();
        // Si vide alors premier click, on demarre le timer
        this->timer->start();

        // On démarre avec la première cible
        this->resultsModel->clickPoints.append(QPoint(x,y));
        this->nextCible();
    }
    else {
        QPointF coords = this->mainView->fittsView->graphicWidget->mapToScene(x,y);
        if(sqrt(pow(coords.x() - this->fittsModel->cercleCenter.last().x(),2) + pow(coords.y() - this->fittsModel->cercleCenter.last().y(),2)) <= this->fittsModel->cercleSize.last()/2) {
            // On stock le temps de click
            this->resultsModel->times.append(this->timer->elapsed());

            this->timer->restart();

            // On stock la position du click
            this->resultsModel->clickPoints.append(QPoint(x,y));
            this->nextCible();
        }
    }
}

void MainController::nextCible() {
    if(!this->fittsModel->cercleCenter.isEmpty())
        this->fittsModel->cibleLeft--;

    QGraphicsScene *scene = this->mainView->fittsView->scene;
    scene->clear();

    // On stop si c'est finis
    if(this->fittsModel->cibleLeft == 0) {
        this->finish();
        return;
    }

    // On génère la taille du cercle rouge
    // qrand() % ((high + 1) - low) + low;
    int size;

    if(this->fittsModel->minSize == this->fittsModel->maxSize){
        size = this->fittsModel->maxSize;
    }else{
        size = QRandomGenerator::global()->bounded(this->fittsModel->minSize, this->fittsModel->maxSize);
    }
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    size*=factor;
    // Car on veut le rayon
    // On place le cercle dans la scene (Attention faut pas qu'il soit en dehors du cadre)
    int sceneW = int(this->mainView->fittsView->scene->width());
    int sceneH = int(this->mainView->fittsView->scene->height());
    qreal posX = QRandomGenerator::global()->bounded(size/2, sceneW-size);
    qreal posY = QRandomGenerator::global()->bounded(size/2, sceneH-size);

    // On stock les infos sur le cercle
    this->fittsModel->cercleCenter.append(QPoint(int(posX+size/2),int(posY+size/2)));
    this->fittsModel->cercleSize.append(size);

    // On place le cercle
    scene->addEllipse(posX, posY, size, size, QPen(QColor("red")),QBrush(QColor("red")));
}


void MainController::finish() {
    this->mainView->fittsView->graphicWidget->setEnabled(false);
    this->mainView->mainStack->setCurrentIndex(1);

    this->calculateResults();
}

void MainController::initGame() {
    QGraphicsScene *scene = this->mainView->fittsView->scene;
    scene->clear();
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;

    qreal posX = scene->width() / 2;
    qreal posY = scene->height() / 2;
    int size = 100*factor;

    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor("green")),QBrush(QColor("green")));
    this->timer = new QElapsedTimer();
}

void MainController::calculateResults() {
    this->calculateResultIDT();
    this->calculateResult2DL();

    // Calcul des valeurs
    // Moyennes
    QList<double> diffValues;
    double diffMoy = 0;

    for (int i = 0; i < this->resultsModel->fittsValues.size(); ++i) {
        diffValues.append(fabs(this->resultsModel->fittsValues[i] - this->resultsModel->times[i]));
        diffMoy += fabs(this->resultsModel->fittsValues[i] - this->resultsModel->times[i]);
    }
    diffMoy /= this->resultsModel->fittsValues.size();

    // On stock la difference de moyenne
    this->resultsModel->diffMoy = fabs(diffMoy);


    // Ecart type
    double variance = 0;

    for (int i = 0; i < this->resultsModel->fittsValues.size(); ++i) {
        variance += pow(diffValues[i] - diffMoy,2);
    }
    variance /= this->resultsModel->fittsValues.size();

    double ecartType = sqrt(variance);

    // On stock l'ecart type
    this->resultsModel->ecartType = ecartType;
    // On stock l'erreur type
    this->resultsModel->erreurType = fabs(ecartType / sqrt(this->resultsModel->fittsValues.size()));

    // On stock itc 95%
    this->resultsModel->itc95 = 2 * this->resultsModel->erreurType;

    this->mainView->resultsView->updateResults(this->fittsModel->a, this->fittsModel->b, this->fittsModel->nbCible, this->fittsModel->minSize, this->fittsModel->maxSize);
}

void MainController::calculateResultIDT(){
    for(int i = 0; i < this->fittsModel->nbCible; ++i) {
        double T = this->resultsModel->times[i];
        this->resultsModel->expSeriesIDT->append(i,T);
        double D = sqrt(pow(this->resultsModel->clickPoints[i].x() - this->fittsModel->cercleCenter[i].x(),2) + pow(this->resultsModel->clickPoints[i].y() - this->fittsModel->cercleCenter[i].y(),2));
        this->resultsModel->DValues.append(log2(2*D / this->fittsModel->cercleSize[i]));
        // On multiplie par 1000 pour être en ms
        double value = (this->fittsModel->a * 1000) + ((this->fittsModel->b * 1000) * log2(2*D / this->fittsModel->cercleSize[i]));
        this->resultsModel->fittsValues.append(value);
        this->resultsModel->fittsSeriesIDT->append(i,value);
        this->mainView->resultsView->axisXIDT->append(QString::number(i + 1) + "<br />T: "+QString::number(T)+"<br />D: " + QString::number(D),i);
    }

    this->mainView->resultsView->chart->addSeries(this->resultsModel->expSeriesIDT);
    this->mainView->resultsView->chart->addSeries(this->resultsModel->fittsSeriesIDT);

    double minExp = *std::min_element(this->resultsModel->times.begin(), this->resultsModel->times.end());
    double maxExp = *std::max_element(this->resultsModel->times.begin(), this->resultsModel->times.end());
    double minFitts = *std::min_element(this->resultsModel->fittsValues.begin(), this->resultsModel->fittsValues.end());
    double maxFitts= *std::max_element(this->resultsModel->fittsValues.begin(), this->resultsModel->fittsValues.end());

    double min = minExp;
    if (minExp > minFitts){
        min = minFitts;
    }
    double max = maxExp;
    if (maxExp < maxFitts){
        max = maxFitts;
    }

    this->mainView->resultsView->axisY->setMin(min);
    this->mainView->resultsView->axisY->setMax(max);

    this->mainView->resultsView->chart->addAxis(this->mainView->resultsView->axisXIDT, Qt::AlignBottom);
    this->resultsModel->fittsSeriesIDT->attachAxis(this->mainView->resultsView->axisXIDT);
    this->resultsModel->expSeriesIDT->attachAxis(this->mainView->resultsView->axisXIDT);
    this->mainView->resultsView->chart->addAxis(this->mainView->resultsView->axisY, Qt::AlignLeft);
    this->resultsModel->expSeriesIDT->attachAxis(this->mainView->resultsView->axisY);
    this->resultsModel->fittsSeriesIDT->attachAxis(this->mainView->resultsView->axisY);
}

void MainController::calculateResult2DL(){
    bool end = false;
    int i = 0;
    while(!end){
        if(i == this->fittsModel->nbCible-1){
            end = true;
        }else{
            if(this->resultsModel->DValues[i] >= this->resultsModel->DValues[i+1]){
                this->resultsModel->DValues.swapItemsAt(i, i+1);
                this->resultsModel->times.swapItemsAt(i, i+1);
                i=0;
            }else{
               i++;
            }
        }
    }

    for(int i = 0; i < this->fittsModel->nbCible; ++i){
        double T = this->resultsModel->times[i];
        this->resultsModel->expSeries2DL->append(i,T);
        double value = (this->fittsModel->a * 1000) + (this->fittsModel->b * 1000) * this->resultsModel->DValues[i];
        this->resultsModel->fittsSeries2DL->append(i,value);
        this->mainView->resultsView->axisX2DL->append(QString::number(this->resultsModel->DValues[i]),i);
    }
}

void MainController::saveConfig(){
    QSettings settings("Garrido", "Fitts2.0");

    settings.setValue("a", this->fittsModel->a);
    settings.setValue("b", this->fittsModel->b);

    settings.setValue("nbCible", this->fittsModel->nbCible);
    settings.setValue("minSize", this->fittsModel->minSize);
    settings.setValue("maxSize", this->fittsModel->maxSize);
}

void MainController::applyConfig(){
    QSettings settings("Garrido", "Fitts2.0");

    if(settings.value("a").toDouble() != 0){
        this->fittsModel->a = settings.value("a").toDouble();
    }
    if(settings.value("b").toDouble() != 0){
        this->fittsModel->b = settings.value("b").toDouble();
    }
    if(settings.value("nbCible").toInt() != 0){
        this->fittsModel->nbCible = settings.value("nbCible").toInt();
    }
    if(settings.value("minSize").toInt() != 0){
        this->fittsModel->minSize = settings.value("minSize").toInt();
    }
    if(settings.value("maxSize").toInt() != 0){
        this->fittsModel->maxSize = settings.value("maxSize").toInt();
    }
}

void MainController::prevChart(){
    if(this->mainView->resultsView->chartIndex == 0){
        this->mainView->resultsView->chartIndex = 1;
    }else{
        this->mainView->resultsView->chartIndex--;
    }
    this->updateAxisX();
    this->mainView->resultsView->updateChart();
}

void MainController::nextChart(){
    if(this->mainView->resultsView->chartIndex == 1){
        this->mainView->resultsView->chartIndex = 0;
    }else{
        this->mainView->resultsView->chartIndex++;
    }
    this->updateAxisX();
    this->mainView->resultsView->updateChart();
}

void MainController::updateAxisX(){
    if(this->mainView->resultsView->chartIndex == 0){
        this->mainView->resultsView->chart->removeSeries(this->resultsModel->expSeries2DL);
        this->mainView->resultsView->chart->removeSeries(this->resultsModel->fittsSeries2DL);
        this->mainView->resultsView->chart->removeAxis(this->mainView->resultsView->axisX2DL);

        this->mainView->resultsView->chart->addSeries(this->resultsModel->expSeriesIDT);
        this->mainView->resultsView->chart->addSeries(this->resultsModel->fittsSeriesIDT);
        this->mainView->resultsView->chart->addAxis(this->mainView->resultsView->axisXIDT, Qt::AlignBottom);
        this->resultsModel->fittsSeriesIDT->attachAxis(this->mainView->resultsView->axisY);

        this->resultsModel->fittsSeriesIDT->attachAxis(this->mainView->resultsView->axisXIDT);
        this->resultsModel->expSeriesIDT->attachAxis(this->mainView->resultsView->axisXIDT);
        this->resultsModel->expSeriesIDT->attachAxis(this->mainView->resultsView->axisY);
    }else{
        this->mainView->resultsView->chart->removeSeries(this->resultsModel->expSeriesIDT);
        this->mainView->resultsView->chart->removeSeries(this->resultsModel->fittsSeriesIDT);
        this->mainView->resultsView->chart->removeAxis(this->mainView->resultsView->axisXIDT);

        this->mainView->resultsView->chart->addSeries(this->resultsModel->expSeries2DL);
        this->mainView->resultsView->chart->addSeries(this->resultsModel->fittsSeries2DL);
        this->mainView->resultsView->chart->addAxis(this->mainView->resultsView->axisX2DL, Qt::AlignBottom);
        this->resultsModel->fittsSeries2DL->attachAxis(this->mainView->resultsView->axisY);

        this->resultsModel->fittsSeries2DL->attachAxis(this->mainView->resultsView->axisX2DL);
        this->resultsModel->expSeries2DL->attachAxis(this->mainView->resultsView->axisX2DL);
        this->resultsModel->expSeries2DL->attachAxis(this->mainView->resultsView->axisY);
    }
}

double MainController::getA(){
    return this->fittsModel->a;
}

double MainController::getB(){
    return this->fittsModel->b;
}

int MainController::getNbCible(){
    return this->fittsModel->nbCible;
}

int MainController::getMinSize(){
    return this->fittsModel->minSize;
}

int MainController::getMaxSize(){
    return this->fittsModel->maxSize;
}

double MainController::getEcartType(){
    return this->resultsModel->ecartType;
}

double MainController::getErreurType(){
    return this->resultsModel->erreurType;
}

double MainController::getDiffMoy(){
    return this->resultsModel->diffMoy;
}

double MainController::getItc95(){
    return this->resultsModel->itc95;
}


void MainController::restart(){
    this->resultsModel->fittsValues.clear();
    this->resultsModel->DValues.clear();
    this->resultsModel->times.clear();
    this->resultsModel->clickPoints.clear();
    this->resultsModel->expSeriesIDT->clear();
    this->resultsModel->expSeries2DL->clear();
    this->resultsModel->fittsSeriesIDT->clear();
    this->resultsModel->fittsSeries2DL->clear();
    this->fittsModel->cercleSize.clear();
    this->fittsModel->cercleCenter.clear();
    this->mainView->resultsView->restartChart();
    this->mainView->mainStack->setCurrentIndex(0);
    this->mainView->fittsView->enableAllSpins();
    this->startSimulation();
    this->mainView->resultsView->updateChart();
}
