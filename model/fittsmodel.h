#ifndef FITTSMODEL_H
#define FITTSMODEL_H

#include <QPoint>
#include <QList>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QHorizontalBarSeries>
#include <QCategoryAxis>
#include <QPieSeries>
#include <QPieSlice>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
//Introduire view et controller dans model
class FittsView;
class FittsController;

class FittsModel
{
public:
    FittsModel();//Constructeur
    QJsonObject writeDataJson();
    QJsonObject QPointToQJsonobject(QPoint);
    QPoint QJsonObjectToQPoint(QJsonValue);
    void getDataJSON(int);
    void addHistoJSON();
    QJsonArray getHistoJSON();


private:
//Définir les paramètres initiaux
    int cibleLeft = 0;
    int nbCible = 10;
    int minSize = 10;
    int maxSize = 160;

    double a = 0.20;
    double b = 0.10;

    double ecartType = 0;
    double erreurType = 0;
    double diffMoy = 0;
    double itc95 = 0;

    QList<QPoint> clickPoints;
    QList<QPoint> cercleCenter;
    QList<int> cercleSize;
    QList<qint64> times;


    friend FittsView;
    friend FittsController;
};

#endif // FITTSMODEL_H
