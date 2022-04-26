#ifndef RESULTSMODEL_H
#define RESULTSMODEL_H

#include <QPoint>
#include <QList>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class MainController;

class ResultsModel
{
public:
    ResultsModel();
    ~ResultsModel();
private:

    double ecartType = 0;
    double erreurType = 0;
    double diffMoy = 0;
    double itc95 = 0;

    QList<QPoint> clickPoints;
    QList<qint64> times;
    QList<double> fittsValues;
    QList<double> DValues;

    QLineSeries *expSeriesIDT;
    QLineSeries *fittsSeriesIDT;

    QLineSeries *expSeries2DL;
    QLineSeries *fittsSeries2DL;

    friend MainController;
};

#endif // RESULTSMODEL_H
