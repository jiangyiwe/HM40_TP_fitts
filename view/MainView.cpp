#include "MainView.h"

MainView::MainView(MainController *mainController) : QMainWindow() {
    this->initWindows(mainController);
}

MainView::~MainView() {}

void MainView::initWindows(MainController *mainController) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    this->setWindowState(Qt::WindowMaximized);
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;

    QWidget *mainWidget = new QWidget;
    mainWidget->setFixedHeight(screenGeometry.height()-30*factor);
    mainWidget->setFixedWidth(screenGeometry.width());
    this->setCentralWidget(mainWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setMargin(0);

    this->displayTitle(mainLayout);

    this->mainStack = new QStackedLayout;
    mainLayout->addLayout(mainStack);

    this->fittsView = new FittsView(mainStack, mainController);
    this->resultsView = new ResultsView(mainStack, mainController);
}

void MainView::displayTitle(QLayout *mainLayout){
    QLabel *label = new QLabel("Tester la loi Fitts");
    label->setAlignment(Qt::AlignCenter);
    qreal factor = QGuiApplication::primaryScreen()->logicalDotsPerInch()/120;
    label->setFont(QFont("Arial",20*factor));
    mainLayout->addWidget(label);
    label->setStyleSheet("background-color : #98C8FF;");
    label->setFixedHeight(90*factor);
}
