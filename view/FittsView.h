#ifndef FITTSVIEW_H
#define FITTSVIEW_H

#include "GraphicWidget.h"
#include "controller/MainController.h"

#include <QStackedLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QGroupBox>

class MainController;

class FittsView : public QObject
{
    Q_OBJECT

public:
    FittsView(QStackedLayout *mainStack, MainController *mainController);
    ~FittsView();

    void disableAllSpins();
    void enableAllSpins();
    void updateValMinOfMaxSize();
    void updateValMaxOfMinSize();
    void resizeScene();
private:
    QDoubleSpinBox *aSpin;
    QDoubleSpinBox *bSpin;
    QSpinBox *nbCibleSpin;
    QSpinBox *minSizeSpin;
    QSpinBox *maxSizeSpin;

    QPushButton *saveBtn;

    GraphicWidget *graphicWidget;
    QGraphicsScene *scene;

    MainController *mainController;

    void displayRappel(QLayout *settingsLayout);
    void displayConfig(QLayout *settingsLayout);
    void displayExplain(QLayout *settingsLayout);
    void displaySaveButton(QBoxLayout *settingsLayout);
    void displayTest(QLayout *graphicLayout);

    friend MainController;
};

#endif // FITTSVIEW_H
