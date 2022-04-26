#ifndef RESULTSVIEW_H
#define RESULTSVIEW_H

#include "controller/MainController.h"

#include <QLineSeries>
//#include <QtCharts>
#include <QStackedLayout>
#include <QChart>
#include <QCategoryAxis>
#include <QToolButton>
#include <QSpacerItem>
#include <QLabel>
#include <QChartView>
#include <QPushButton>

QT_CHARTS_USE_NAMESPACE

class MainController;

class ResultsView : public QObject
{
     Q_OBJECT
public:
    ResultsView(QStackedLayout *mainStack, MainController *mainController);
    ~ResultsView();

    void updateResults(double a, double b, int nbCible, int minSize, int maxSize);
    void updateChart();
    void restartChart();
private:

    MainController *mainController;

    QLabel *ecartTypeLabel;
    QLabel *erreurTypeLabel;
    QLabel *diffMoyLabel;
    QLabel *itc95Label;

    QLabel *aLabel;
    QLabel *bLabel;

    QLabel *nbCibleLabel;
    QLabel *minSizeLabel;
    QLabel *maxSizeLabel;

    QChartView *plot;
    QChart *chart;

    QCategoryAxis *axisXIDT;
    QCategoryAxis *axisX2DL;
    QValueAxis *axisY;

    QToolButton *prevChart;
    QToolButton *nextChart;
    QPushButton *rstButton;
    QPushButton *quitButton;

    int chartIndex = 0;

    void displayChartView(QLayout *chartLayout);
    void displayChart();
    void displayFigures(QBoxLayout *resultLayout);
    void displayConfig(QLayout *figuresLayout);
    void displayStats(QLayout *figuresLayout);
    void displayButtons(QBoxLayout *resultLayout);

    friend MainController;
};

#endif // RESULTSVIEW_H
