#include "ResultsModel.h"

ResultsModel::ResultsModel()
{
    this->expSeriesIDT = new QLineSeries;
    this->fittsSeriesIDT = new QLineSeries;

    this->expSeries2DL = new QLineSeries;
    this->fittsSeries2DL = new QLineSeries;

    this->expSeriesIDT->setName("Courbe expérimentale");
    this->fittsSeriesIDT->setName("Courbe théorique");
    this->expSeries2DL->setName("Courbe expérimentale");
    this->fittsSeries2DL->setName("Courbe théorique");
}

ResultsModel::~ResultsModel(){

}
