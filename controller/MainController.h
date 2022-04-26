#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "view/MainView.h"
#include "model/FittsModel.h"
#include "model/ResultsModel.h"

#include <QObject>
#include <QApplication>
#include <QSettings>
#include <QRandomGenerator>
#include <QElapsedTimer>

QT_CHARTS_USE_NAMESPACE

class MainView;
class FittsModel;
class ResultsModel;

class MainController : public QObject
{
    Q_OBJECT
public:
    MainController();
    ~MainController();
    void start();
    void updateAxisX();

    double getA();
    double getB();
    int getNbCible();
    int getMinSize();
    int getMaxSize();

    double getEcartType();
    double getErreurType();
    double getDiffMoy();
    double getItc95();

private:
    void initGame();
    void finish();
    void nextCible();

    void calculateResults();
    void calculateResultIDT();
    void calculateResult2DL();
    void applyConfig();

    FittsModel *fittsModel;
    ResultsModel *resultsModel;
    MainView *mainView;
    QElapsedTimer *timer;

private slots:
    void quit();
    void startSimulation();

    void aSpinChanged(double value);
    void bSpinChanged(double value);
    void nbCibleSpinChanged(int value);
    void minSizeSpinChanged(int value);
    void maxSizeSpinChanged(int value);

    void cibleClicked(int x, int y);

    void prevChart();
    void nextChart();

    void saveConfig();

    void restart();
};

#endif // MAINCONTROLLER_H
