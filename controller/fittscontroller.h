#ifndef FITTSCONTROLLER_H
#define FITTSCONTROLLER_H

#include <QObject>
#include <QElapsedTimer>
#include <QApplication>
#include <QDebug>
#include <QtGlobal>
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
#include <algorithm>
#include <QTextBrowser>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
//Nous passons view et model dans controller
class FittsView;
class FittsModel;
//Nous définissons controller comme une classe qui hérite de Qobjet afin d'utiliser toutes les fonctions de Qobjet.
class FittsController : public QObject
{
    Q_OBJECT
public:
    FittsController();//Constructeur
    virtual ~FittsController() {}//Déconstructeur
    void start();

    QList<FittsModel> *histModel;//Stocker une liste de types de model
    QJsonArray getHisto();
    //QJsonArray est une classe implicitement partagée et partage les données avec le document à partir duquel elle a été créée tant qu'il n'est pas modifié.


private:
    void initGame();
    void finish();
    void nextCible();

    void calculateResultHome();
    void addHisto();
    QTextBrowser * textDisplay;

    FittsModel *fittsModel;
    FittsView *fittsView;

    QElapsedTimer *timer;
//Minuterie pour calculer le temps écoulé

public slots:
    void Aide();
private slots://La valeur de retour de toutes les fonctions sous le slot est nulle.
    void quit();
    void startSimulation();
    void backToSettings();
    void cancel();
    void changeGraphHome();
    //void aide(QString filename);
    void aValueChanged(double value);
    void bValueChanged(double value);
    void nbCibleChanged(int value);
    void minSizeChanged(int value);
    void maxSizeChanged(int value);
    void MenuAide();
    void cibleClicked(int x, int y);
    void useSave(int);
    void deleteSave(int);
    void save();


};

#endif // FITTSCONTROLLER_H
