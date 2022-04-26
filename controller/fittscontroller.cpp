#include <iostream>
//#include <cstdlib>
#include "fittscontroller.h"
#include "fittsview.h"
#include "fittsmodel.h"

QT_CHARTS_USE_NAMESPACE
//Introduction d'une bibliothèque de graphiques
using namespace std;
//Constructeur pour controller
FittsController::FittsController() {
    this->fittsModel = new FittsModel();
    this->fittsView = new FittsView(this, this->fittsModel);
    this->histModel = new QList<FittsModel>();
    this->start();
}

void FittsController::start() {

    this->fittsView->show();
    startSimulation();

}
//Lancer un test fitts
void FittsController::startSimulation() {
    this->fittsView->mainStack->setCurrentIndex(1);
    this->fittsModel->cibleLeft = this->fittsModel->nbCible;
    this->fittsView->updateTestMsg();
    this->fittsView->graphicView->setEnabled(true);
    this->fittsModel->cercleSize.clear();
    this->fittsModel->cercleCenter.clear();
    this->fittsModel->clickPoints.clear();
    this->fittsModel->times.clear();

    this->initGame();
}
//Procédures de sortie
void FittsController::quit() {
    QApplication::quit();
}
//Pour basculer entre les graphiques créés avec des points et ceux créés avec des distances
void FittsController::changeGraphHome(){
    if(this->fittsView->switchGraphHome->minimumHeight() == 250){
        this->fittsView->switchGraphHome->setIcon(QIcon(":/icons/switchGraphe_2"));
        this->fittsView->switchGraphHome->setMinimumHeight(249);
        this->fittsView->plotHomeDistance->setVisible(true);
        this->fittsView->plotHome->setVisible(false);
        this->fittsView->graphTitleHome->setText("Temps en fonction de la distance");
    }else{
        this->fittsView->switchGraphHome->setIcon(QIcon(":/icons/switchGraphe_1"));
        this->fittsView->switchGraphHome->setMinimumHeight(250);
        this->fittsView->plotHomeDistance->setVisible(false);
        this->fittsView->plotHome->setVisible(true);
        this->fittsView->graphTitleHome->setText("Temps pour atteindre une cible");
    }
}


void FittsController::backToSettings() {
    this->fittsView->mainStack->setCurrentIndex(0);
    this->calculateResultHome();
    this->fittsView->displayResults();
}

//Abandon du test
void FittsController::cancel() {
    this->fittsView->mainStack->setCurrentIndex(0);
}


//Modifier la valeur de a
void FittsController::aValueChanged(double value) {
    this->fittsModel->a = value;
}
//Modifier la valeur de b
void FittsController::bValueChanged(double value) {
    this->fittsModel->b = value;
}
//Modifier le nombre des cibles
void FittsController::nbCibleChanged(int value) {
    this->fittsModel->nbCible = value;
}
//Modifier la taille minimale des cibles
void FittsController::minSizeChanged(int value) {
    this->fittsModel->minSize = value;
}
//Modifier la taille maximale des cibles
void FittsController::maxSizeChanged(int value) {
    this->fittsModel->maxSize = value;
}
//Fonctions permettant de cliquer sur des points et d'enregistrer des données
void FittsController::cibleClicked(int x, int y) {
    if(this->fittsModel->cercleCenter.isEmpty()) {
        // Si vide alors premier click, on demarre le timer
        this->timer = new QElapsedTimer;
        timer->start();

        // On démarre avec la première cible
        this->fittsModel->clickPoints.append(QPoint(x,y));
        this->nextCible();
    }
    else {
        QPointF coords = this->fittsView->graphicView->mapToScene(x,y);
        //Si nous n'avons pas mesuré tous les points
        if(sqrt(pow(coords.x() - this->fittsModel->cercleCenter.last().x(),2) + pow(coords.y() - this->fittsModel->cercleCenter.last().y(),2)) <= this->fittsModel->cercleSize.last() / 2) {
            // On stock le temps de click
            this->fittsModel->times.append(timer->elapsed());
            // On restart le chrono
            timer->restart();

            // On stock la position du click
            this->fittsModel->clickPoints.append(QPoint(x,y));
            this->nextCible();
        }
    }
}
//Commencez à cliquer sur le point suivant
void FittsController::nextCible() {
    if(!this->fittsModel->cercleCenter.isEmpty())
        this->fittsModel->cibleLeft--;
    this->fittsView->updateTestMsg();

    QGraphicsScene *scene = this->fittsView->scene;
    scene->clear();

    // On stop si c'est finis
    if(this->fittsModel->cibleLeft == 0) {
        this->finish();
        return;
    }

    // On génère la taille du cercle rouge
    // qrand() % ((high + 1) - low) + low;
    //QRandomGenerator est utilisé pour générer des nombres aléatoires
    int size = QRandomGenerator::global()->generate() % ((this->fittsModel->maxSize + 1) - this->fittsModel->minSize) + this->fittsModel->minSize;
    // Car on veut le rayon
    // On place le cercle dans la scene (Attention faut pas qu'il soit en dehors du cadre)
    int sceneW = int(this->fittsView->scene->width());
    int sceneH = int(this->fittsView->scene->height());
    //qreal est un type fractionnaire
    //Emplacement des points générés aléatoirement
    qreal posX = QRandomGenerator::global()->generate() % std::max(((sceneW - size) - size) + size, 1);
    qreal posY = QRandomGenerator::global()->generate() % std::max(((sceneH - size) - size) + size, 1);

    // On stock les infos sur le cercle
    this->fittsModel->cercleCenter.append(QPoint(int(posX),int(posY)));
    this->fittsModel->cercleSize.append(size);

    // On place le cercle
    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor(color_red)),QBrush(QColor(color_red)))->setCursor(Qt::PointingHandCursor);
}

//Fin du test
void FittsController::finish() {
    this->fittsView->graphicView->setEnabled(false);
    backToSettings();
}
//Initialisation de l'écran de démarrage du test
void FittsController::initGame() {
    QGraphicsScene *scene = this->fittsView->scene;
    scene->clear();

    if(this->fittsModel->maxSize >= this->fittsView->graphicView->width() / 2)
        this->fittsModel->maxSize = this->fittsView->graphicView->width() / 2;

    if(this->fittsModel->maxSize >= this->fittsView->graphicView->height() / 2)
        this->fittsModel->maxSize = this->fittsView->graphicView->height() / 2;

    qreal posX = scene->width() / 2;
    qreal posY = scene->height() / 2;
    int size = 180;

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(30);
    effect->setXOffset(0);
    effect->setYOffset(0);
    effect->setColor(color_blue);
    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor(color_blue)),QBrush(QColor(color_blue)))->setGraphicsEffect(effect);
    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor(color_blue)),QBrush(QColor(color_blue)))->setCursor(Qt::PointingHandCursor);
    QLabel *labelStart;
    labelStart = new QLabel("Start");
    labelStart->setAlignment(Qt::AlignCenter);
    labelStart->setStyleSheet("color: #ffffff; font: bold 35px 'ROBOTO'; background-color: transparent;");
    labelStart->setCursor(Qt::PointingHandCursor);
    labelStart->move((size / 2) + 189, (size / 2) + 43);
    scene->addWidget(labelStart);
}

//Calculer et tracer les résultats des tests
void FittsController::calculateResultHome() {
    //New plotHome
    QChart *chartHome = new QChart;
    this->fittsView->plotHome->setChart(chartHome);
    this->fittsView->plotHome->setRenderHint(QPainter::Antialiasing);
    chartHome->setAnimationOptions(QChart::AllAnimations);
    chartHome->createDefaultAxes();
    chartHome->legend()->setVisible(false);
    chartHome->legend()->setLabelBrush(QBrush(QColor(color_white)));
    chartHome->setBackgroundVisible(false);

    QLineSeries *expSeries = new QLineSeries;
    QLineSeries *fittsSeries = new QLineSeries;
    QCategoryAxis *axis = new QCategoryAxis;
    //End plotHome

    //New plotHomeDistance
    QChart *chartDistanceHome = new QChart;
    this->fittsView->plotHomeDistance->setChart(chartDistanceHome);
    this->fittsView->plotHomeDistance->setRenderHint(QPainter::Antialiasing);
    chartDistanceHome->setAnimationOptions(QChart::AllAnimations);
    chartDistanceHome->createDefaultAxes();
    chartDistanceHome->legend()->setVisible(false);
    chartDistanceHome->legend()->setLabelBrush(QBrush(QColor(color_white)));
    chartDistanceHome->setBackgroundVisible(false);

    QLineSeries *expSeriesDistance = new QLineSeries;
    QLineSeries *fittsSeriesDistance = new QLineSeries;
    QCategoryAxis *axisDistance = new QCategoryAxis;
    //end New plotHomeDistance

    QList<double> fittsValues;
    QList<double> DistRelatif;
    QList<double> listeTemps;
    QList<double> listeTempsCalcule;

    for(int i = 0; i < this->fittsModel->nbCible; ++i) {
        //Courbe Experimental
        double T = this->fittsModel->times[i];
        listeTemps.append(T);
        expSeries->append(i,T);
        double D = sqrt(pow(this->fittsModel->clickPoints[i].x() - this->fittsModel->cercleCenter[i].x(),2) + pow(this->fittsModel->clickPoints[i].y() - this->fittsModel->cercleCenter[i].y(),2));

        //Courbe Theorique
        // On multiplie par 1000 pour être en ms
        double value = (this->fittsModel->a * 1000) + ((this->fittsModel->b * 1000) * log2((D / this->fittsModel->cercleSize[i]) + 1));
        listeTempsCalcule.append(value);
        fittsValues.append(value);
        fittsSeries->append(i,value);

        axis->append(QString::number(i + 1) + "<br />T: "+QString::number(T)+"<br />D: " + QString::number(D),i);

        //Ajout d'une nouvelle courbe : le temps exécution (ordonnée) en fonction de la distance relative log(2D/L) en abscisse
        //On utilise D en ordonnée et value en abscisse

        DistRelatif.append(log(2*D/this->fittsModel->cercleSize[i]));

    }


    //Sort
    for (int i=DistRelatif.length()-1; i > 0; i--){
        for (int j=0; j < i; j++){
            if (DistRelatif[j]>DistRelatif[j+1]){
                double tmp = DistRelatif[j+1];
                DistRelatif[j+1] = DistRelatif[j];
                DistRelatif[j] = tmp;

                tmp = listeTemps[j+1];
                listeTemps[j+1] = listeTemps[j];
                listeTemps[j] = tmp;

                tmp = listeTempsCalcule[j+1];
                listeTempsCalcule[j+1] = listeTempsCalcule[j];
                listeTempsCalcule[j] = tmp;
            }
        }
    }

    for(int i = 0; i < this->fittsModel->nbCible; ++i) {
        axisDistance->append(QString::number(DistRelatif[i]), DistRelatif[i]);
        expSeriesDistance->append(DistRelatif[i], DistRelatif[i]);
        fittsSeriesDistance->append(DistRelatif[i], listeTemps[i]);
    }

    axis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisDistance->setLabelsAngle(70);

    QPen pen(QRgb(0xffffff));
    pen.setColor(color_purple);
    pen.setWidth(3);
    expSeries->setPen(pen);
    expSeries->setVisible();
    expSeriesDistance->setVisible();
    fittsSeriesDistance->setPen(pen);

    pen.setColor(color_blue);
    pen.setWidth(3);
    expSeriesDistance->setPen(pen);
    fittsSeries->setPen(pen);

    chartHome->addSeries(expSeries);
    chartHome->addSeries(fittsSeries);

    QPen dotted;
    dotted.setWidth(2);
    QVector<qreal> dashes;
    dashes << 4 << 4;
    dotted.setDashPattern(dashes);
    dotted.setColor(color_grid);

    axis->setGridLinePen(dotted);
    axisDistance->setGridLinePen(dotted);

    QFont reperes;
    reperes.setFamily("ROBOTO");
    axis->setLabelsFont(reperes);
    axisDistance->setLabelsFont(reperes);

    axis->setLabelsColor(color_white);
    axis->setTitleText("Distance (T = f(D))");

    axisDistance->setLabelsColor(color_white);
    axisDistance->setTitleText("log(2D/L)");

    chartHome->setAxisX(axis,expSeries);
    chartHome->setAxisX(axis,fittsSeries);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("temps (en ms)");
    axisY->setGridLinePen(dotted);
    axisY->setLabelsColor(color_white);
    chartHome->setAxisY(axisY,expSeries);

    //New plotHomeDistance axes
    axisDistance->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    chartDistanceHome->addSeries(expSeriesDistance);
    chartDistanceHome->addSeries(fittsSeriesDistance);
    chartDistanceHome->setAxisX(axisDistance,expSeriesDistance);


    QValueAxis *axisYDistance = new QValueAxis;
    axisYDistance->setTitleText("temps (en ms)");
    axisYDistance->setGridLinePen(dotted);
    axisYDistance->setLabelsColor(color_white);
    chartDistanceHome->addAxis(axisYDistance, Qt::AlignLeft);
    chartDistanceHome->setAxisY(axisYDistance,fittsSeriesDistance);

    //end New plotHomeDistance axes



    // Calcul des valeurs
    // Moyennes
    QList<double> diffValues;
    double diffMoy = 0;

    for (int i = 0; i < fittsValues.size(); ++i) {
        diffValues.append(fabs(fittsValues[i] - this->fittsModel->times[i]));
        diffMoy += fabs(fittsValues[i] - this->fittsModel->times[i]);
    }
    diffMoy /= fittsValues.size();

    // On stock la difference de moyenne
    this->fittsModel->diffMoy = fabs(diffMoy);


    // Ecart type
    double variance = 0;

    for (int i = 0; i < fittsValues.size(); ++i) {
        variance += pow(diffValues[i] - diffMoy,2);
    }
    variance /= fittsValues.size();

    double ecartTypeHome = sqrt(variance);

    // On stock l'ecart type
    this->fittsModel->ecartType = ecartTypeHome;
    // On stock l'erreur type
    this->fittsModel->erreurType = fabs(ecartTypeHome / sqrt(fittsValues.size()));

    // On stock itc 95%
    this->fittsModel->itc95 = 2 * this->fittsModel->erreurType;

    this->fittsView->displayResults();

}
//Ajouter le graphique obtenu
void FittsController::addHisto(){
    this->histModel->prepend(*this->fittsModel);
    this->fittsModel->addHistoJSON();
    fittsView->reloadHisto();
}
//Afficher le chart
QJsonArray FittsController::getHisto(){
    return fittsModel->getHistoJSON();
}

//Cette fonction permet d'afficher le contenu du document sur la console.
void FittsController::Aide(){

       QFile fileaide(":/icons/aide/aide.txt");

       // fileaide.open(QFile::ReadOnly | QFile::Text);

        if(!fileaide.exists()){
            //qCritical() << "This file couldn't be found";//Deuxième méthode d'affichage des erreurs
            QMessageBox::information(0, "This file couldn't be found", fileaide.errorString());
            //return;
        }

        if(!fileaide.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error can't be read", fileaide.errorString());
               //qCritical() << "Could not open file";
               //qCritical()  << fileaide.errorString();
               //return;
        }

//        fileaide.seek(0);

        QTextStream ReadFile(&fileaide);
       // QString data=ReadFile.readAll();
       // qInfo() << data;
       while(!ReadFile.atEnd()){
            QString line = ReadFile.readLine();
            qInfo() << line;
       }
        fileaide.close();
//        QTextStream ReadFile(&fileaide);
//        textDisplay=new QTextBrowser();
//        textDisplay->setText(ReadFile.readAll());//Une deuxième façon de lire les documents

        //fileaide.close();

}
//Affichage du contenu du document sur le programme
void FittsController::MenuAide(){
    QGraphicsScene *scene = this->fittsView->scene;
    scene->clear();
    int size = 200;
    QFile fileaide(":/icons/aide/aide.txt");

     if(!fileaide.exists()){

         QMessageBox::information(0, "This file couldn't be found", fileaide.errorString());

     }

     if(!fileaide.open(QIODevice::ReadOnly)) {
         QMessageBox::information(0, "error can't be read", fileaide.errorString());
     }


     QTextStream ReadFile(&fileaide);

    while(!ReadFile.atEnd()){
        QString data=ReadFile.readAll();
        QLabel *labelaide= new QLabel(data);
        //labelaide->setAlignment(Qt::AlignCenter);//Cette ligne de code sera le texte au milieu de l'affichage
        labelaide->setStyleSheet("color: #ffffff; font: bold 35px 'ROBOTO'; background-color: transparent;");
        labelaide->move((size / 2) - 400, (size / 2) - 200);
        scene->addWidget(labelaide);
    }
     fileaide.close();
}

void FittsController::deleteSave(int index) {
    cout << index << endl;
    this->histModel->prepend(*this->fittsModel);


    // On écrit dans le répertoire .config de l'utilisateur
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/fitts");

    qDebug() << QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);

    QString jsonPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/fitts/data.json";

    QFile fileReader(jsonPath);
    fileReader.open(QIODevice::ReadOnly);

    QJsonDocument json = QJsonDocument::fromJson(fileReader.readAll());
    QJsonArray array = json.array();
    array.removeAt(index);

    QJsonDocument newJson(array);

    fileReader.close();


    QFile fileWriter(jsonPath);
    fileWriter.open(QIODevice::WriteOnly);
    fileWriter.write(newJson.toJson());
    fileWriter.close();
    this->fittsView->reloadHisto();
}

void FittsController::useSave(int index) {
    fittsModel->getDataJSON(index);
    calculateResultHome();
}

void FittsController::save() {
    addHisto();
}


