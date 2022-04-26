#ifndef FITTSMODEL_H
#define FITTSMODEL_H

#include <QList>
#include <QPoint>

class MainController;

class FittsModel
{
public:
    FittsModel();
    ~FittsModel();
private:
    int cibleLeft = 0;
    int nbCible = 10;
    int minSize = 30;
    int maxSize = 150;

    double a = 0.20;
    double b = 0.10;

    QList<int> cercleSize;
    QList<QPoint> cercleCenter;


    friend MainController;
};

#endif // FITTSMODEL_H
