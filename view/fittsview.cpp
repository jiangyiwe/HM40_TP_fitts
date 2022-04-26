#include "fittsview.h"
#include "graphicwidget.h"
#include "fittscontroller.h"
#include "fittsmodel.h"
#include <iostream>

using namespace std;

QT_CHARTS_USE_NAMESPACE

FittsView::FittsView(FittsController *fittsController, FittsModel *fittsModel) : QMainWindow() {
    this->fittsModel = fittsModel;

    this->initWindows();

    // Btn clicked
    connect(startBtn,SIGNAL(clicked()),fittsController,SLOT(startSimulation()));

    connect(backBtn,SIGNAL(clicked()),fittsController,SLOT(cancel()));
    connect(parametre,SIGNAL(triggered()),fittsController,SLOT(cancel()));
    connect(aide, SIGNAL(triggered()), fittsController, SLOT(MenuAide()));
    connect(quit,SIGNAL(triggered()),fittsController,SLOT(quit()));
    connect(graphicView, SIGNAL(mouseClicked(int,int)), fittsController, SLOT(cibleClicked(int,int)));
    connect(switchGraphHome, SIGNAL(clicked()),fittsController,SLOT(changeGraphHome()));
    connect(sauvegarde,SIGNAL(triggered()), fittsController, SLOT(save()));
    connect(saveButton,SIGNAL(clicked()),fittsController,SLOT(save()));

    // SpinBox values update
    connect(aValue,SIGNAL(valueChanged(double)),fittsController,SLOT(aValueChanged(double)));
    connect(bValue,SIGNAL(valueChanged(double)),fittsController,SLOT(bValueChanged(double)));
    connect(nbCible,SIGNAL(valueChanged(int)),fittsController,SLOT(nbCibleChanged(int)));
    connect(minSize,SIGNAL(valueChanged(int)),fittsController,SLOT(minSizeChanged(int)));
    connect(maxSize,SIGNAL(valueChanged(int)),fittsController,SLOT(maxSizeChanged(int)));

    connect(this, SIGNAL(_useSave(int)), fittsController, SLOT(useSave(int)));
    connect(this, SIGNAL(_deleteSave(int)), fittsController, SLOT(deleteSave(int)));

}

FittsView::~FittsView() {}


void FittsView::sizeWindows(){
    // get the dimension available on this screen
    QSize availableSize = qApp->desktop()->availableGeometry().size();
    int width = availableSize.width();
    int height = availableSize.height();
    width *= 0.9; // 90% of the screen size
    height *= 0.9; // 90% of the screen size
    QSize newSize( width, height );
    setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    newSize,
                    qApp->desktop()->availableGeometry()
                    )
                );
}

void FittsView::initWindows() {

    sizeWindows();

    QWidget *mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);
    this->setStyleSheet("QWidget{background-color:" + color_bg + ";}");

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setMargin(0);

    mainStack = new QStackedLayout;
    mainLayout->addLayout(mainStack);


    QWidget *settingsWidget = new QWidget;
    mainStack->addWidget(settingsWidget);

    QHBoxLayout *settingsLayout = new QHBoxLayout(settingsWidget);
    QVBoxLayout *settingsLayoutLeft = new QVBoxLayout(settingsWidget);
    QVBoxLayout *settingsLayoutRight = new QVBoxLayout(settingsWidget);

    QFrame *frameRight = new QFrame();
    frameRight->setStyleSheet("background-color: " + color_black);
    frameRight->setMinimumWidth(350);
    frameRight->setMaximumWidth(350);
    frameRight->setLayout(settingsLayoutRight);
    settingsLayoutRight->setMargin(0);
    settingsLayoutLeft->setMargin(40);

    settingsLayout->setContentsMargins(QMargins(0,0,0,0));
    settingsLayout->addLayout(settingsLayoutLeft);
    settingsLayout->addWidget(frameRight);

    QLabel *label;

    //Droite

    label = new QLabel("Liste des tests");
    label->setStyleSheet("color: #ffffff; font: bold 30px 'ROBOTO'; padding: 10px");
    label->setMaximumHeight(100);
    label->setMinimumHeight(100);
    label->setMargin(0);
    settingsLayoutRight->addWidget(label);

    saveButton = new QPushButton("Sauvegarder");
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setStyleSheet("QPushButton{color: "+color_white+"; background-color: " + color_blue + "; border-radius:" + button_radius +"; font: bold 20px 'ROBOTO'; padding: 20px; margin: 0px 30px} QPushButton:hover{background-color: " + color_blue_focus + "};");
    settingsLayoutRight->addWidget(saveButton);

    this->scrollAreaLayout = new QVBoxLayout;
    scrollFrame = new QFrame();
    scrollFrame->setLayout(this->scrollAreaLayout);
    scrollFrame->setMinimumWidth(333);
    scrollFrame->setStyleSheet("background-color: " + color_itemList_bg );

    QLabel *imageLabel = new QLabel;
    QImage image(":/icons/test");
    imageLabel->setPixmap(QPixmap::fromImage(image));



    reloadHisto();

    scrollFrame->updateGeometry();

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollFrame);
    scrollArea->setStyleSheet("QScrollArea{border: none} QScrollBar:vertical{background-color: #242424;} QScrollBar::handle:vertical{background-color: #323232;} QScrollBar::add-page:vertical { background-color: #242424; } QScrollBar::sub-page:vertical { background-color: #242424; }");
    settingsLayoutRight->addWidget(scrollArea);

    //Droite - end


    //Gauche

    QHBoxLayout *settingsLayoutLeftTop = new QHBoxLayout();
    QHBoxLayout *settingsLayoutLeftBottom = new QHBoxLayout();
    settingsLayoutLeft->addLayout(settingsLayoutLeftTop);
    settingsLayoutLeft->addSpacing(25);
    QFrame *sepLeft = new QFrame();
    sepLeft->setMinimumHeight(2);
    sepLeft->setMaximumHeight(2);
    sepLeft->setStyleSheet("background-color:" + color_dark_grey);
    settingsLayoutLeft->addWidget(sepLeft);
    settingsLayoutLeft->addSpacing(25);
    settingsLayoutLeft->addLayout(settingsLayoutLeftBottom);


    //Gauche Top

    //Basculer entre les deux graphiques


    /***************************************************************************************************************************************************
     * Switch button (to switch from one law to the other)
     ***************************************************************************************************************************************************/
    switchGraphHome = new QToolButton(this);
    switchGraphHome->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    switchGraphHome->setMinimumWidth(120);
    switchGraphHome->setMaximumWidth(120);
    switchGraphHome->setMinimumHeight(100);
    switchGraphHome->setMaximumHeight(250);
    switchGraphHome->setStyleSheet("QToolButton{color: "+color_light_grey+"; border-radius:" + button_radius +"; font: bold 10px 'ROBOTO'; padding: 10px; margin-right: 40px}");
    switchGraphHome->setCursor(Qt::PointingHandCursor);
    switchGraphHome->setIcon(QIcon(":/icons/switchGraphe_1"));
    switchGraphHome->setIconSize(QSize(130, 130));

    settingsLayoutLeftTop->addWidget(switchGraphHome);

    /***************************************************************************************************************************************************
     * Card where the statistics are displayed
     ***************************************************************************************************************************************************/
    QFrame *cardTop = new QFrame();
    cardTop->setMinimumWidth(175);
    cardTop->setMaximumWidth(275);
    cardTop->setMinimumHeight(100);
    cardTop->setMaximumHeight(380);
    cardTop->setStyleSheet("background-color:" + color_black + "; border-radius: 20px");

    settingsLayoutLeftTop->addWidget(cardTop);

    QVBoxLayout *cardTopLayout = new QVBoxLayout();
    cardTop->setLayout(cardTopLayout);

    label = new QLabel();
    label->setText("Statistiques");
    label->setStyleSheet("font: bold 20px 'ROBOTO'; color:" + color_white);
    label->setAlignment(Qt::AlignCenter);
    cardTopLayout->addWidget(label);

    ecartType = new QLabel();
    ecartType->setStyleSheet("font: 14px 'ROBOTO'; color:" + color_white);
    ecartType->setAlignment(Qt::AlignCenter);
    cardTopLayout->addWidget(ecartType);

    diffMoy = new QLabel();
    diffMoy->setStyleSheet("font: 14px 'ROBOTO'; color:" + color_white);
    diffMoy->setAlignment(Qt::AlignCenter);
    cardTopLayout->addWidget(diffMoy);

    erreurType = new QLabel();
    erreurType->setStyleSheet("font: 14px 'ROBOTO'; color:" + color_white);
    erreurType->setAlignment(Qt::AlignCenter);
    cardTopLayout->addWidget(erreurType);

    itc95 = new QLabel();
    itc95->setStyleSheet("font: 14px 'ROBOTO'; color:" + color_white);
    itc95->setAlignment(Qt::AlignCenter);
    cardTopLayout->addWidget(itc95);


    QVBoxLayout *graphHomeLayout = new QVBoxLayout();
    settingsLayoutLeftTop->addLayout(graphHomeLayout);

    QHBoxLayout *titleLegend = new QHBoxLayout();
    titleLegend->addSpacing(50);
    graphTitleHome = new QLabel(this);
    graphTitleHome->setText("Temps pour atteindre une cible");
    graphTitleHome->setStyleSheet("font: bold 30px 'ROBOTO'; color:" + color_white);
    titleLegend->addWidget(graphTitleHome);

    titleLegend->addStretch();

    QFrame *theoriqueSquare = new QFrame(this);
    theoriqueSquare->setMinimumHeight(20);
    theoriqueSquare->setMinimumWidth(20);
    theoriqueSquare->setMaximumHeight(20);
    theoriqueSquare->setMaximumWidth(20);
    theoriqueSquare->setStyleSheet("background-color: " + color_blue + ";border-radius: 3px;");
    titleLegend->addWidget(theoriqueSquare);

    QLabel *legendTheo = new QLabel(this);
    legendTheo->setText("Théorique");
    legendTheo->setStyleSheet("font: 20 20px 'ROBOTO'; color:" + color_white);
    titleLegend->addWidget(legendTheo);

    titleLegend->addSpacing(30);

    QFrame *expSquare = new QFrame(this);
    expSquare->setMinimumHeight(20);
    expSquare->setMinimumWidth(20);
    expSquare->setMaximumHeight(20);
    expSquare->setMaximumWidth(20);
    expSquare->setStyleSheet("background-color: " + color_purple + ";border-radius: 3px;");
    titleLegend->addWidget(expSquare);

    QLabel *legendExp = new QLabel(this);
    legendExp->setText("Expérimental");
    legendExp->setStyleSheet("font: 20 20px 'ROBOTO'; color:" + color_white);
    titleLegend->addWidget(legendExp);

    graphHomeLayout->addLayout(titleLegend);



    plotHome = new QChartView;
    plotHomeDistance = new QChartView;
    plotHomeDistance->setVisible(false);
    graphHomeLayout->addWidget(plotHome);
    graphHomeLayout->addWidget(plotHomeDistance);

    //Gauche Top - end


    //Gauche Bottom

    /***************************************************************************************************************************************************
     * Card where the a and b variables are displayed
     ***************************************************************************************************************************************************/

    QFrame *cardBottom = new QFrame();
    cardBottom->setMinimumWidth(275);
    cardBottom->setMaximumWidth(275);
    cardBottom->setMinimumHeight(380);
    cardBottom->setMaximumHeight(300);
    cardBottom->setStyleSheet("background-color:" + color_black + "; border-radius: 20px");
    settingsLayoutLeftBottom->addWidget(cardBottom);

    QVBoxLayout *cardBottomLayout = new QVBoxLayout();
    cardBottomLayout->setMargin(20);
    cardBottom->setLayout(cardBottomLayout);
    cardBottomLayout->addSpacing(10);

    label = new QLabel();
    label->setText("Paramètres formule");
    label->setStyleSheet("font: bold 20px 'ROBOTO'; color:" + color_white);
    label->setAlignment(Qt::AlignCenter);
    cardBottomLayout->addWidget(label);

    QFrame *bottomCardSeparator = new QFrame();
    bottomCardSeparator->setMinimumHeight(2);
    bottomCardSeparator->setMaximumHeight(2);
    bottomCardSeparator->setStyleSheet("background-color:" + color_white);
    cardBottomLayout->addWidget(bottomCardSeparator);

    label = new QLabel();
    label->setText("a + b*log2(2D/L)");
    label->setStyleSheet("font: italic 18px 'ROBOTO'; color:" + color_light_grey);
    label->setAlignment(Qt::AlignCenter);
    cardBottomLayout->addWidget(label);
    cardBottomLayout->addSpacing(10);

    label = new QLabel();
    label->setText("Variable A");
    label->setStyleSheet("font: 20px 'ROBOTO'; color:" + color_white);
    label->setAlignment(Qt::AlignCenter);
    cardBottomLayout->addWidget(label);

    aValue = new QDoubleSpinBox;
    aValue->setValue(this->fittsModel->a);
    aValue->setMinimum(0.1);
    aValue->setSingleStep(0.1);
    aValue->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    aValue->setAlignment(Qt::AlignHCenter);
    aValue->setMinimumHeight(30);
    aValue->setCursor(Qt::PointingHandCursor);
    aValue->setStyleSheet("QDoubleSpinBox{font: 26px 'ROBOTO'; color:" + color_white + ";} QDoubleSpinBox::down-button{subcontrol-origin: margin; subcontrol-position: center left; image: url(:/icons/moinsButton); width: 30px; height: 30px; margin-left: 40px} QDoubleSpinBox::up-button{subcontrol-origin: margin; subcontrol-position: center right; image: url(:/icons/plusButton); width: 30px; height: 30px; margin-right: 40px}");
    cardBottomLayout->addWidget(aValue);

    cardBottomLayout->addSpacing(10);

    label = new QLabel();
    label->setText("Variable B");
    label->setStyleSheet("font: 20px 'ROBOTO'; color:" + color_white);
    label->setAlignment(Qt::AlignCenter);
    cardBottomLayout->addWidget(label);

    bValue = new QDoubleSpinBox;
    bValue->setValue(this->fittsModel->b);
    bValue->setMinimum(0.1);
    bValue->setSingleStep(0.1);
    bValue->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    bValue->setAlignment(Qt::AlignHCenter);
    bValue->setMinimumHeight(30);
    bValue->setCursor(Qt::PointingHandCursor);
    bValue->setStyleSheet("QDoubleSpinBox{font: 26px 'ROBOTO'; color:" + color_white + ";} QDoubleSpinBox::down-button{subcontrol-origin: margin; subcontrol-position: center left; image: url(:/icons/moinsButton); width: 30px; height: 30px; margin-left: 40px} QDoubleSpinBox::up-button{subcontrol-origin: margin; subcontrol-position: center right; image: url(:/icons/plusButton); width: 30px; height: 30px; margin-right: 40px}");
    cardBottomLayout->addWidget(bValue);
    cardBottomLayout->addSpacing(40);


    QVBoxLayout *settingsLayoutLeftBottomConfig = new QVBoxLayout();
    settingsLayoutLeftBottom->addLayout(settingsLayoutLeftBottomConfig);

    label = new QLabel();
    label->setText("Configurations de la partie");
    label->setStyleSheet("margin-left: 28px; font: bold 30px 'ROBOTO'; color:" + color_white);
    settingsLayoutLeftBottomConfig->addWidget(label);

    settingsLayoutLeftBottomConfig->addStretch();

    QHBoxLayout *configLayout = new QHBoxLayout();
    settingsLayoutLeftBottomConfig->addLayout(configLayout);

    QVBoxLayout *configLayoutItem = new QVBoxLayout();
    label = new QLabel();
    label->setText("Nombre de cibles");
    label->setStyleSheet("font: 20px 'ROBOTO'; color:" + color_white);
    label->setAlignment(Qt::AlignCenter);
    configLayoutItem->addWidget(label);

    nbCible = new QSpinBox;
    nbCible->setValue(this->fittsModel->nbCible);
    nbCible->setMaximum(100);
    nbCible->setMinimum(5);
    nbCible->setSingleStep(1);
    nbCible->setAlignment(Qt::AlignCenter);
    nbCible->setCursor(Qt::PointingHandCursor);
    nbCible->setStyleSheet("QSpinBox{font: 26px 'ROBOTO'; color:" + color_white + ";} QSpinBox::down-button{subcontrol-origin: margin; subcontrol-position: center left; image: url(:/icons/moinsButtonDark); width: 30px; height: 30px; margin-left: 40px} QSpinBox::up-button{subcontrol-origin: margin; subcontrol-position: center right; image: url(:/icons/plusButtonDark); width: 30px; height: 30px; margin-right: 40px}");;
    configLayoutItem->addWidget(nbCible);
    configLayout->addLayout(configLayoutItem);

    QFrame *separator = new QFrame;
    separator->setMinimumWidth(2);
    separator->setMaximumWidth(2);
    separator->setStyleSheet("background-color: " + color_grid);
    configLayout->addWidget(separator);


    configLayoutItem = new QVBoxLayout();
    label = new QLabel();
    label->setText("Taille minimum");
    label->setStyleSheet("font: 20px 'ROBOTO'; color:" + color_white);
    label->setAlignment(Qt::AlignCenter);
    configLayoutItem->addWidget(label);

    minSize = new QSpinBox;
    minSize->setMaximum(150);
    minSize->setMinimum(10);
    minSize->setSingleStep(10);
    minSize->setValue(this->fittsModel->minSize);
    minSize->setAlignment(Qt::AlignCenter);
    minSize->setCursor(Qt::PointingHandCursor);
    minSize->setStyleSheet("QSpinBox{font: 26px 'ROBOTO'; color:" + color_white + ";} QSpinBox::down-button{subcontrol-origin: margin; subcontrol-position: center left; image: url(:/icons/moinsButtonDark); width: 30px; height: 30px; margin-left: 40px} QSpinBox::up-button{subcontrol-origin: margin; subcontrol-position: center right; image: url(:/icons/plusButtonDark); width: 30px; height: 30px; margin-right: 40px}");
    configLayoutItem->addWidget(minSize);
    configLayout->addLayout(configLayoutItem);

    separator = new QFrame;
    separator->setMinimumWidth(2);
    separator->setMaximumWidth(2);
    separator->setStyleSheet("background-color: " + color_grid);
    configLayout->addWidget(separator);


    configLayoutItem = new QVBoxLayout();
    label = new QLabel();
    label->setText("Taille maximum");
    label->setStyleSheet("font: 20px 'ROBOTO'; color:" + color_white);
    label->setAlignment(Qt::AlignCenter);
    configLayoutItem->addWidget(label);

    maxSize = new QSpinBox;
    maxSize->setMaximum(400);
    maxSize->setMinimum(160);
    maxSize->setSingleStep(10);
    maxSize->setValue(this->fittsModel->maxSize);
    maxSize->setAlignment(Qt::AlignCenter);
    maxSize->setCursor(Qt::PointingHandCursor);
    maxSize->setStyleSheet("QSpinBox{font: 26px 'ROBOTO'; color:" + color_white + ";} QSpinBox::down-button{subcontrol-origin: margin; subcontrol-position: center left; image: url(:/icons/moinsButtonDark); width: 30px; height: 30px; margin-left: 40px} QSpinBox::up-button{subcontrol-origin: margin; subcontrol-position: center right; image: url(:/icons/plusButtonDark); width: 30px; height: 30px; margin-right: 40px}");
    configLayoutItem->addWidget(maxSize);
    configLayout->addLayout(configLayoutItem);

    settingsLayoutLeftBottomConfig->addStretch();

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(15);
    effect->setXOffset(0);
    effect->setYOffset(0);
    effect->setColor(color_blue);
    startBtn = new QPushButton("Démarrer");
    startBtn->setGraphicsEffect(effect);
    startBtn->setCursor(Qt::PointingHandCursor);
    startBtn->setStyleSheet("QPushButton{color: "+color_white+"; background-color: " + color_blue + "; border-radius:" + button_radius +"; font: bold 20px 'ROBOTO'; padding: 20px; margin: 0px 30px} QPushButton:hover{background-color: " + color_blue_focus + "};");
    settingsLayoutLeftBottomConfig->addWidget(startBtn);
    effect->setBlurRadius(15);
    effect->setXOffset(0);
    effect->setYOffset(15);
    effect->setColor(color_blue);
    /*//Ajouter un button menu sur le page final
    menuBtn = new QPushButton("Menu");
    menuBtn->setGraphicsEffect(effect);
    menuBtn->setCursor(Qt::PointingHandCursor);
    menuBtn->setStyleSheet("QPushButton{color: "+color_white+"; background-color: " + color_blue + "; border-radius:" + button_radius +"; font: bold 20px 'ROBOTO'; padding: 20px; margin: 0px 30px} QPushButton:hover{background-color: " + color_blue_focus + "};");
    settingsLayoutLeftBottomConfig->addWidget(menuBtn);*/

    //Gauche Bottom - end

    //Gauche - end

    // Test part

    QWidget *testWidget = new QWidget;
    mainStack->addWidget(testWidget);
    mainStack->setMargin(0);

    QVBoxLayout *testLayout = new QVBoxLayout(testWidget);
    testLayout->setMargin(0);

    graphicView = new GraphicWidget;
    testLayout->addWidget(graphicView);
    graphicView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    graphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicView->setStyleSheet("border: 0px solid; background-color: #323232");

    scene = new QGraphicsScene;
    graphicView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    graphicView->setScene(scene);
    scene->setSceneRect(0,0,graphicView->width(),300);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout = new QHBoxLayout;
    testLayout->addLayout(btnLayout);

    //Ajouter le button Annuler
    backBtn = new QPushButton("Annuler");
    btnLayout->addWidget(backBtn);
    backBtn->setStyleSheet("QPushButton{color: "+color_white+"; background-color: " + color_red + "; border-radius:" + button_radius +"; font: bold 20px 'ROBOTO'; padding: 20px; margin: 30px} QPushButton:hover{background-color: " + color_red_focus + "};");
    backBtn->setMinimumHeight(30);
    backBtn->setMinimumWidth(300);
    backBtn->setMaximumWidth(300);
    backBtn->move(100, 50);
    backBtn->setCursor(Qt::PointingHandCursor);

    // Autre façon d'ajouter menu
    pMenu = new QMenu(this);
    sauvegarde = new QAction(tr("Sauvegarde JSON"), this);
    pMenu->addAction(sauvegarde);
    parametre = new QAction(tr("Paramètres"), this);

    pMenu->addAction(parametre);


    aide = new QAction(tr("Menu aide"), this);
    //aide->setShortcut(tr("Ctrl+o"));
    pMenu->addAction(aide);
    pMenu->addSeparator();
    quit = new QAction(tr("Quit"), this);
    pMenu->addAction(quit);
    // Button
    pButton = new QPushButton(this);
    pButton->setStyleSheet("QPushButton{color: "+color_white+"; background-color: rgb(85,170,255)" + "; border-radius:" + button_radius+"; font: bold 30px 'ROBOTO'; padding: 3px; margin: 0px} QPushButton:hover{background-color: " + color_blue_focus + "};");
    pButton->setText(QString::fromLocal8Bit("Menu"));
    pButton->setMinimumHeight(15);
    pButton->setMinimumWidth(30);
    // Set Menu
    pButton->setMenu(pMenu);
    QStringList list;
    list.append("color:white");
    list.append("background-color:rgb(85,170,255)");
    list.append("border-style:outset");
    list.append("border-width:5px");
    list.append("border-color:rgb(10,45,110)");
    list.append("border-radius:20px");
    list.append("font:bold 30px");
    list.append("padding:4px");


    pMenu->setStyleSheet(list.join(';'));



    testLabel = new QLabel;
    testLabel->setAlignment(Qt::AlignCenter);
    btnLayout->addWidget(testLabel);

    btnLayout->addSpacing(300);

}

void FittsView::updateTestMsg() {
    this->testLabel->setText("<strong style='font-size:100px'>" + QString::number(this->fittsModel->cibleLeft) + "</strong><br>cibles restantes");
    this->testLabel->setStyleSheet("color: "+color_white+"; font: 30px 'ROBOTO'; margin: 30px");

}

void FittsView::displayResults() {
    this->diffMoy->setText("Différence moyenne = " + QString::number(this->fittsModel->diffMoy)+ " ms");
    this->ecartType->setText("Écart type = " + QString::number(this->fittsModel->ecartType) + " ms");
    this->erreurType->setText("Erreur type = " + QString::number(this->fittsModel->erreurType) + " ms");
    this->itc95->setText("Intervalle de conf à 95% = " + QString::number(this->fittsModel->itc95) + " ms");
    aValue->setValue(this->fittsModel->a);
    bValue->setValue(this->fittsModel->b);
    nbCible->setValue(this->fittsModel->nbCible);
    minSize->setValue(this->fittsModel->minSize);
    maxSize->setValue(this->fittsModel->maxSize);
}

void FittsView::reloadHisto(){
    QLayoutItem *child;
    while ((child = this->scrollAreaLayout->layout()->takeAt(0)) != 0) {
        delete child;
    }

    //ITEM LISTE
    QHBoxLayout *scrollItemLayout;
    QFrame *scrollFrameItem;
    QToolButton *deleteButton;
    QFrame *scrollFrameLine;

    QJsonArray histo = this->fittsController->getHisto();

    // Historique des sessions réalisées
    for(int i = 0; i < histo.size(); i++){
        QToolButton *eyeButton;

        QJsonObject current = histo.at(i).toObject();

        scrollItemLayout = new QHBoxLayout;
        scrollFrameItem = new QFrame();
        scrollFrameItem->setLayout(scrollItemLayout);
        scrollFrameItem->setMinimumHeight(100);
        scrollFrameItem->setStyleSheet(".QFrame:hover{background-color:" + color_itemList_bg_focus + "}");

        // Bouton oeil
        eyeButton = new QToolButton();
        eyeButton->setMinimumWidth(40);
        eyeButton->setMinimumHeight(40);
        eyeButton->setMaximumHeight(40);
        eyeButton->setMaximumWidth(40);
        eyeButton->setStyleSheet("QToolButton{border-radius: 14px; background-color: " + color_blue +"} QToolButton:hover{background-color: " + color_blue_focus +"}");
        eyeButton->setCursor(Qt::PointingHandCursor);
        eyeButton->setIcon(QIcon(":/icons/eyeIcon"));
        eyeButton->setIconSize(QSize(28, 28));

        // Affichage de la date de la sauvegarde
        QLabel *label;
        label = new QLabel(current["dateTime"].toString());
        label->setStyleSheet("background-color: transparent; color: #ffffff; font: 20 18px 'ROBOTO';");
        label->setAlignment(Qt::AlignCenter);

        // Bouton de suppression
        deleteButton = new QToolButton();
        deleteButton->setMinimumWidth(25);
        deleteButton->setMinimumHeight(25);
        deleteButton->setMaximumHeight(25);
        deleteButton->setMaximumWidth(25);
        deleteButton->setStyleSheet("QToolButton{background-color: transparent; border-radius: 12px} QToolButton:hover{background-color: " + color_red_focus +"}");
        deleteButton->setCursor(Qt::PointingHandCursor);
        deleteButton->setIcon(QIcon(":/icons/beenIcon"));
        deleteButton->setIconSize(QSize(15, 15));



        scrollFrameLine = new QFrame();
        scrollFrameLine->setMinimumHeight(2);
        scrollFrameLine->setMaximumHeight(2);
        scrollFrameLine->setStyleSheet(".QFrame{background-color:" + color_bg + "}");

        scrollItemLayout->addWidget(eyeButton);
        scrollItemLayout->addWidget(label);
        scrollItemLayout->addWidget(deleteButton);
        this->scrollAreaLayout->addWidget(scrollFrameItem);
        this->scrollAreaLayout->addWidget(scrollFrameLine);

        scrollAreaLayout->update();
        scrollFrame->updateGeometry();

        // Signaux avec une fonction lambda pour passer une valeur au slot
        connect(eyeButton, &QToolButton::pressed,this,[this, i]{emit this->_useSave(i);});
        connect(deleteButton, &QToolButton::pressed,this,[this, i]{emit this->_deleteSave(i);});


    }

    //ITEM LISTE END
}

