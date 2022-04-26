#include "FittsView.h"

FittsView::FittsView(QStackedLayout *mainStack, MainController *mainController){

    QWidget *testWidget = new QWidget;
    mainStack->addWidget(testWidget);
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    testWidget->setStyleSheet("font-size : "+QString::number(16*factor)+"px");

    QHBoxLayout *bodyLayout = new QHBoxLayout(testWidget);

    this->mainController = mainController;

    QVBoxLayout *settingsLayout = new QVBoxLayout();
    bodyLayout->addLayout(settingsLayout,0);

    QHBoxLayout *graphicLayout = new QHBoxLayout();
    bodyLayout->addLayout(graphicLayout,3);

    this->displayTest(graphicLayout);

    this->displayRappel(settingsLayout);

    this->displayConfig(settingsLayout);

    this->displayExplain(settingsLayout);

    this->displaySaveButton(settingsLayout);

    connect(this->saveBtn,SIGNAL(clicked()),mainController,SLOT(saveConfig()));
    connect(this->graphicWidget, SIGNAL(mouseClicked(int,int)), mainController, SLOT(cibleClicked(int,int)));
    connect(this->aSpin,SIGNAL(valueChanged(double)),mainController,SLOT(aSpinChanged(double)));
    connect(this->bSpin,SIGNAL(valueChanged(double)),mainController,SLOT(bSpinChanged(double)));
    connect(this->nbCibleSpin,SIGNAL(valueChanged(int)),mainController,SLOT(nbCibleSpinChanged(int)));
    connect(this->minSizeSpin,SIGNAL(valueChanged(int)),mainController,SLOT(minSizeSpinChanged(int)));
    connect(this->maxSizeSpin,SIGNAL(valueChanged(int)),mainController,SLOT(maxSizeSpinChanged(int)));
}

FittsView::~FittsView() {}

void FittsView::disableAllSpins(){
    this->aSpin->setDisabled(true);
    this->bSpin->setDisabled(true);
    this->nbCibleSpin->setDisabled(true);
    this->minSizeSpin->setDisabled(true);
    this->maxSizeSpin->setDisabled(true);
}

void FittsView::enableAllSpins(){
    this->aSpin->setDisabled(false);
    this->bSpin->setDisabled(false);
    this->nbCibleSpin->setDisabled(false);
    this->minSizeSpin->setDisabled(false);
    this->maxSizeSpin->setDisabled(false);
}

void FittsView::displayRappel(QLayout *settingsLayout){
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    QGroupBox *rappelBox = new QGroupBox("Rappel :");
    settingsLayout->addWidget(rappelBox);
    rappelBox->setStyleSheet("background-color : #DCEEFF;");

    QGridLayout *rappelGridLayout = new QGridLayout(rappelBox);
    QLabel *label = new QLabel("Formule utilisée:");
    label->setMaximumHeight(20*factor);
    rappelGridLayout->addWidget(label,0,0);
    rappelGridLayout->addWidget(new QLabel("Variable a :"),1,0);
    rappelGridLayout->addWidget(new QLabel("Variable b :"),2,0);

    rappelGridLayout->addWidget(new QLabel("a+b*log2(2D/L)"),0,1);

    this->aSpin = new QDoubleSpinBox;
    this->aSpin->setMinimum(0.2);
    this->aSpin->setValue(this->mainController->getA());
    this->aSpin->setStyleSheet("background-color : #fff");
    this->aSpin->setFixedHeight(30*factor);
    this->aSpin->setSingleStep(0.05);
    rappelGridLayout->addWidget(this->aSpin,1,1);

    this->bSpin = new QDoubleSpinBox;
    this->bSpin->setMinimum(0.1);
    this->bSpin->setValue(this->mainController->getB());
    this->bSpin->setStyleSheet("background-color : #fff");
    this->bSpin->setFixedHeight(30*factor);
    this->bSpin->setSingleStep(0.05);
    rappelGridLayout->addWidget(this->bSpin,2,1);
}

void FittsView::displayConfig(QLayout *settingsLayout){
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    QGroupBox *configBox = new QGroupBox("Configuration du test :");
    settingsLayout->addWidget(configBox);
    configBox->setStyleSheet("background-color : #DCEEFF;");

    QGridLayout *configLayout = new QGridLayout(configBox);

    configLayout->addWidget(new QLabel("Nombre de cibles :"),1,0);
    configLayout->addWidget(new QLabel("Taille minimum cible :"),2,0);
    configLayout->addWidget(new QLabel("Taille maximal cible :"),3,0);

    this->nbCibleSpin = new QSpinBox;
    this->nbCibleSpin->setValue(this->mainController->getNbCible());
    this->nbCibleSpin->setMinimum(5);
    this->nbCibleSpin->setMaximum(100);
    this->nbCibleSpin->setStyleSheet("background-color : #fff");
    this->nbCibleSpin->setFixedHeight(30*factor);
    configLayout->addWidget(this->nbCibleSpin,1,1);

    int max = 2000;

    if(this->graphicWidget->height() > this->graphicWidget->width()){
        max = this->graphicWidget->width()/2;
    }else{
        max = this->graphicWidget->height()/2;
    }

    this->minSizeSpin = new QSpinBox;
    this->minSizeSpin->setMaximum(max);
    this->minSizeSpin->setValue(this->mainController->getMinSize());
    this->minSizeSpin->setStyleSheet("background-color : #fff");
    this->minSizeSpin->setFixedHeight(30*factor);
    this->minSizeSpin->setSingleStep(5);
    this->minSizeSpin->setMinimum(30);
    configLayout->addWidget(this->minSizeSpin,2,1);

    this->maxSizeSpin = new QSpinBox;
    this->maxSizeSpin->setMaximum(max);
    this->maxSizeSpin->setValue(this->mainController->getMaxSize());
    this->maxSizeSpin->setStyleSheet("background-color : #fff");
    this->maxSizeSpin->setFixedHeight(30*factor);
    this->maxSizeSpin->setSingleStep(5);
    configLayout->addWidget(this->maxSizeSpin,3,1);

    this->updateValMaxOfMinSize();
    this->updateValMinOfMaxSize();
}

void FittsView::displayExplain(QLayout *settingsLayout){
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    QGroupBox *explainBox = new QGroupBox("Déroulement du test :");
    settingsLayout->addWidget(explainBox);
    explainBox->setStyleSheet("background-color : #DCEEFF;");

    QHBoxLayout *explainLayout = new QHBoxLayout(explainBox);

    QLabel *explain = new QLabel("Pour commencer cliquez sur le <font color='green' size = '"+QString::number(20*factor)+"'>●</font>. Puis cliquez sur les <font color='red' size = '"+QString::number(20*factor)+"'>●</font> qui apparaissent sur votre éran");
    explain->setWordWrap(true);
    explainLayout->addWidget(explain);
}

void FittsView::displaySaveButton(QBoxLayout *settingsLayout){
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    settingsLayout->addLayout(btnLayout);

    this->saveBtn = new QPushButton("Sauvegarder configuration");
    this->saveBtn->setStyleSheet("background-color : #3F68EE; color : #fff; font-size : "+QString::number(16*factor)+"px; font-weight : bold; padding : "+QString::number(16*factor)+"px");
    btnLayout->addWidget(this->saveBtn);
}

void FittsView::updateValMinOfMaxSize(){
    this->maxSizeSpin->setMinimum(this->minSizeSpin->value());
}

void FittsView::updateValMaxOfMinSize(){
    this->minSizeSpin->setMaximum(this->maxSizeSpin->value());
}

void FittsView::displayTest(QLayout *graphicLayout){
    this->graphicWidget = new GraphicWidget;
    graphicLayout->addWidget(this->graphicWidget);
    this->graphicWidget->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    this->graphicWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->scene = new QGraphicsScene;
    this->graphicWidget->setScene(this->scene);
}

void FittsView::resizeScene(){
    this->scene->setSceneRect(0, 0, this->graphicWidget->width(), this->graphicWidget->height());
    this->graphicWidget->fitInView(this->scene->sceneRect());
}
