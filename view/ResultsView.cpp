#include "ResultsView.h"

ResultsView::ResultsView(QStackedLayout *mainStack, MainController *mainController)
{
    this->mainController = mainController;

    QWidget *resultWidget = new QWidget;
    mainStack->addWidget(resultWidget);
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    resultWidget->setStyleSheet("font-size : "+QString::number(16*factor)+"px");

    QVBoxLayout *resultLayout = new QVBoxLayout(resultWidget);

    QHBoxLayout *chartLayout = new QHBoxLayout();
    resultLayout->addLayout(chartLayout);

    this->displayChartView(chartLayout);
    this->displayFigures(resultLayout);
    this->displayButtons(resultLayout);

    connect(this->prevChart,SIGNAL(clicked()),mainController,SLOT(prevChart()));
    connect(this->nextChart,SIGNAL(clicked()),mainController,SLOT(nextChart()));
    connect(this->rstButton,SIGNAL(clicked()),mainController,SLOT(restart()));
    connect(this->quitButton,SIGNAL(clicked()),mainController,SLOT(quit()));

}

ResultsView::~ResultsView() {}

void ResultsView::displayChartView(QLayout *chartLayout){
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    this->prevChart = new QToolButton();
    this->prevChart->setMinimumWidth(50*factor);
    this->prevChart->setMinimumHeight(50*factor);
    this->prevChart->setStyleSheet("background-color : transparent");
    this->prevChart->setCursor(Qt::PointingHandCursor);
    this->prevChart->setIcon(QIcon(":/icons/left-arrow"));
    this->prevChart->setIconSize(QSize(40*factor, 40*factor));
    chartLayout->addWidget(this->prevChart);

    this->plot = new QChartView;
    chartLayout->addWidget(this->plot);

    this->nextChart = new QToolButton();
    this->nextChart->setMinimumWidth(50*factor);
    this->nextChart->setMinimumHeight(50*factor);
    this->nextChart->setStyleSheet("background-color : transparent");
    this->nextChart->setCursor(Qt::PointingHandCursor);
    this->nextChart->setIcon(QIcon(":/icons/right-arrow"));
    this->nextChart->setIconSize(QSize(40*factor, 40*factor));
    chartLayout->addWidget(this->nextChart);

    this->displayChart();
}

void ResultsView::displayChart(){
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    this->chart = new QChart;

    this->plot->setChart(this->chart);
    this->plot->setRenderHint(QPainter::Antialiasing);
    this->chart->setTitle("Résultats loi Fitts");
    this->chart->setTitleFont(QFont("Arial",16*factor, QFont::Bold));
    this->chart->setAnimationOptions(QChart::AllAnimations);
    this->chart->createDefaultAxes();
    this->chart->legend()->setVisible(true);
    this->chart->legend()->setAlignment(Qt::AlignBottom);

    this->axisXIDT = new QCategoryAxis;
    this->axisX2DL = new QCategoryAxis;

    this->axisXIDT->setMin(0);
    this->axisX2DL->setMin(0);

    this->axisXIDT->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    this->axisX2DL->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    this->axisX2DL->setTitleText("dsitance relative (log(2D/L))");

    this->axisY = new QValueAxis;
    this->axisY->setTitleText("temps (en ms)");
}

void ResultsView::displayFigures(QBoxLayout *resultLayout){
    QHBoxLayout *figuresLayout = new QHBoxLayout();
    resultLayout->addLayout(figuresLayout);

    this->displayConfig(figuresLayout);
    figuresLayout->setStretch(0,1);
    this->displayStats(figuresLayout);
    figuresLayout->setStretch(1,1);
}

void ResultsView::displayConfig(QLayout *figuresLayout){

    QGroupBox *configBox =  new QGroupBox("Configuration");
    figuresLayout->addWidget(configBox);
    configBox->setStyleSheet("background-color : #DCEEFF");
    QGridLayout *configBoxLayout = new QGridLayout(configBox);

    configBoxLayout->addWidget(new QLabel("Valeur de a :"),0,0);
    configBoxLayout->addWidget(new QLabel("Valeur de b :"),1,0);
    configBoxLayout->addWidget(new QLabel("Taille minimum cible :"),0,2);
    configBoxLayout->addWidget(new QLabel("Taille maximal cible :"),1,2);
    configBoxLayout->addWidget(new QLabel("Nombre de cibles :"),0,4);

    this->aLabel = new QLabel("");
    configBoxLayout->addWidget(this->aLabel,0,1);
    this->bLabel = new QLabel("");
    configBoxLayout->addWidget(this->bLabel,1,1);

    this->minSizeLabel = new QLabel("");
    configBoxLayout->addWidget(this->minSizeLabel,0,3);
    this->maxSizeLabel = new QLabel("");
    configBoxLayout->addWidget(this->maxSizeLabel,1,3);
    this->nbCibleLabel = new QLabel("");
    configBoxLayout->addWidget(this->nbCibleLabel,0,5);
}

void ResultsView::displayButtons(QBoxLayout *resultLayout){
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    resultLayout->addLayout(buttonLayout);

    buttonLayout->addItem(new QSpacerItem(0,0));

    this->rstButton = new QPushButton("Recommencer");
    this->rstButton->setStyleSheet("background-color : #32B41F; color : #fff; font-size : "+QString::number(16*factor)+"px; font-weight : bold; padding : "+QString::number(16*factor)+"px");
    buttonLayout->addWidget(this->rstButton);

    this->quitButton = new QPushButton("Quitter");
    this->quitButton->setStyleSheet("background-color : #DC3A3A; color : #fff; font-size : "+QString::number(16*factor)+"px; font-weight : bold; padding : "+QString::number(16*factor)+"px");
    buttonLayout->addWidget(this->quitButton);

    buttonLayout->setStretch(0,2);
}

void ResultsView::displayStats(QLayout *figuresLayout){

    QGroupBox *resultBox =  new QGroupBox("Statistiques");
    figuresLayout->addWidget(resultBox);
    resultBox->setStyleSheet("background-color : #DCEEFF");
    QGridLayout *resultBoxLayout = new QGridLayout(resultBox);

    resultBoxLayout->addWidget(new QLabel("Ecart-Type : "),0,0);
    this->ecartTypeLabel = new QLabel;
    resultBoxLayout->addWidget(this->ecartTypeLabel,0,1);

    resultBoxLayout->addWidget(new QLabel("Erreur-Type : "),1,0);
    this->erreurTypeLabel = new QLabel;
    resultBoxLayout->addWidget(this->erreurTypeLabel,1,1);

    resultBoxLayout->addWidget(new QLabel("Différence moyenne : "),0,2);
    this->diffMoyLabel = new QLabel;
    resultBoxLayout->addWidget(this->diffMoyLabel,0,3);

    resultBoxLayout->addWidget(new QLabel("Intervalle de confiance à 95% : "),1,2);
    this->itc95Label = new QLabel;
    resultBoxLayout->addWidget(this->itc95Label,1,3);

    resultBoxLayout->setColumnStretch(1,10);
    resultBoxLayout->setColumnStretch(3,10);
}

void ResultsView::updateResults(double a, double b, int nbCible, int minSize, int maxSize){
    this->diffMoyLabel->setText(QString::number(this->mainController->getDiffMoy())+" ms");
    this->ecartTypeLabel->setText(QString::number(this->mainController->getEcartType())+" ms");
    this->erreurTypeLabel->setText(QString::number(this->mainController->getErreurType())+" ms");
    this->itc95Label->setText(QString::number(this->mainController->getItc95())+" ms");

    this->aLabel->setText(QString::number(a));
    this->bLabel->setText(QString::number(b));
    this->nbCibleLabel->setText(QString::number(nbCible));
    this->minSizeLabel->setText(QString::number(minSize));
    this->maxSizeLabel->setText(QString::number(maxSize));
}

void ResultsView::updateChart(){
    this->plot->repaint();
}

void ResultsView::restartChart(){
    this->chartIndex = 0;
    this->mainController->updateAxisX();
    this->displayChart();
}
