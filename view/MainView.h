#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FittsView.h"
#include "ResultsView.h"
#include <QGuiApplication>
#include <QPolarChart>
#include <QScreen>
#include <QMainWindow>

class FittsView;
class ResultsView;

class MainView : public QMainWindow
{
    Q_OBJECT
public:
    MainView(MainController *mainController);
    ~MainView();

    void initWindows(MainController *mainController);
private:
    void displayTitle(QLayout *mainLayout);

    FittsView *fittsView;
    ResultsView *resultsView;

    QStackedLayout *mainStack;

    friend MainController;
};

#endif // MAINWINDOW_H
