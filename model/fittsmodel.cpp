#include "fittsmodel.h"
#include "fittsview.h"
//Constructeur
FittsModel::FittsModel() {
}


QJsonObject FittsModel::writeDataJson() {
    QJsonObject dataItem;//Inscription des paramètres dans le modèle

    dataItem["cibleLeft"] = cibleLeft;
    dataItem["nbCible"] = nbCible;
    dataItem["minSize"] = minSize;
    dataItem["maxSize"] = maxSize;
    dataItem["a"] = a;
    dataItem["b"] = b;
    dataItem["ecartType"] = ecartType;
    dataItem["erreurType"] = erreurType;
    dataItem["diffMoy"] = diffMoy;
    dataItem["itc95"] = itc95;

    // Ajout des points cliqués par l'utilisateur
    QJsonArray clickPointsArray;

    for(int i = 0; i < clickPoints.size(); i++){

        clickPointsArray.append(QPointToQJsonobject(clickPoints[i]));

    }

    dataItem["clickPoints"] = clickPointsArray;

    // Ajout des temps de click
    QJsonArray clickPointsTimes;

    for(int i = 0; i < times.size(); i++){
        clickPointsTimes.append(times[i]);

    }

    dataItem["times"] = clickPointsTimes;

    // Ajout des centres des cercles générés
    QJsonArray cercleCenterArray;

    for(int i = 0; i < cercleCenter.size(); i++){

        cercleCenterArray.append(QPointToQJsonobject(cercleCenter[i]));

    }


    dataItem["cercleCenter"] = cercleCenterArray;


    // Ajout des grandeurs des cercles
    QJsonArray cercleSizeArray;

    for(int i = 0; i < cercleSize.size(); i++){

        cercleSizeArray.append(cercleSize[i]);

    }

    dataItem["cercleSize"] = cercleSizeArray;

    // Ajout de la date du test
    QDateTime *currentDate = new QDateTime;

    dataItem["dateTime"] = currentDate->currentDateTime().toString("dd/MM/yyyy - hh:mm");
    delete currentDate;
    //qWarning() << dataItem;

    return dataItem;

}

void FittsModel::getDataJSON(int index) {


    // On écrit dans le répertoire .config de l'utilisateur
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/fitts");

    qDebug() << QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);

    QString jsonPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/fitts/data.json";

    QFile fileReader(jsonPath);
    fileReader.open(QIODevice::ReadOnly);

    QJsonDocument json = QJsonDocument::fromJson(fileReader.readAll());
    QJsonArray array = json.array();
    QJsonValue dataItem = array.at(index);

    QJsonArray clickPointsArray = dataItem["clickPoints"].toArray();
    QJsonArray clickPointsTimes = dataItem["times"].toArray();
    QJsonArray cercleCenterArray = dataItem["cercleCenter"].toArray();
    QJsonArray cercleSizeArray = dataItem["cercleSize"].toArray();

    QList<QPoint> _clickPoints;
    QList<qint64> _times;
    QList<QPoint> _cercleCenter;
    QList<int> _cercleSize;

    nbCible = dataItem["nbCible"].toVariant().toInt();
    maxSize = dataItem["maxSize"].toVariant().toInt();
    minSize = dataItem["minSize"].toVariant().toInt();
    a = dataItem["a"].toVariant().toDouble();
    b = dataItem["b"].toVariant().toDouble();
    ecartType = dataItem["ecartType"].toVariant().toDouble();
    erreurType = dataItem["erreurType"].toVariant().toDouble();
    diffMoy = dataItem["diffMoy"].toVariant().toDouble();
    itc95 = dataItem["itc95"].toVariant().toDouble();

    for(int i = 0; i < clickPointsArray.size();++i) {
        _clickPoints.append(QJsonObjectToQPoint(clickPointsArray.at(i)));
    }
    clickPoints = _clickPoints;

    for(int i = 0; i < clickPointsTimes.size();++i) {
        _times.append(static_cast<qint64>(clickPointsTimes.at(i).toInt()));
    }
    times = _times;

    for(int i = 0; i < cercleCenterArray.size();++i) {
        _cercleCenter.append(QJsonObjectToQPoint(cercleCenterArray.at(i)));
    }
    cercleCenter = _cercleCenter;

    for(int i = 0; i < cercleSizeArray.size();++i) {
        _cercleSize.append(cercleSizeArray.at(i).toInt());
    }
    cercleSize = _cercleSize;

}

QJsonObject FittsModel::QPointToQJsonobject(QPoint point) {

    QJsonObject converter;
    converter.insert("x", point.x());
    converter.insert("y", point.y());

    return converter;

}

QPoint FittsModel::QJsonObjectToQPoint(QJsonValue point) {
    QPoint converter(point["x"].toVariant().toInt(), point["y"].toVariant().toInt());

    return converter;
}

void FittsModel::addHistoJSON() {
    // On écrit dans le répertoire .config de l'utilisateur
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/fitts");

    qDebug() << QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);

    QString jsonPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/fitts/data.json";

    QFile fileReader(jsonPath);
    fileReader.open(QIODevice::ReadOnly);

    QJsonDocument json = QJsonDocument::fromJson(fileReader.readAll());
    QJsonArray array = json.array();
    array.prepend(writeDataJson());
    QJsonDocument newJson(array);

    fileReader.close();

    QFile fileWriter(jsonPath);
    fileWriter.open(QIODevice::WriteOnly);
    fileWriter.write(newJson.toJson());
    fileWriter.close();
}

QJsonArray FittsModel::getHistoJSON() {
    QString jsonPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/fitts/data.json";

    QFile fileReader(jsonPath);
    fileReader.open(QIODevice::ReadOnly);
    QJsonDocument json = QJsonDocument::fromJson(fileReader.readAll());
    QJsonArray array = json.array();

    QString dataToString(json.toJson());
    //cout << dataToString.toStdString() << endl;

    fileReader.close();

    return array;
}


