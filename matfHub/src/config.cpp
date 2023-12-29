#include "../include/config.hpp"

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

Config* Config::configPtr = nullptr;

Config::Config(){
    QString currentPath = QDir::currentPath();
    if(!QFile(currentPath+"/.config/save.json").exists()){
        //qDebug() << "ne posotji";
        hubPath = currentPath + "/MATF";
        configPath = currentPath + "/.config"; //const
        QDir(currentPath).mkdir(".config");
        setConfig();
    }else{
        configPath = currentPath + "/.config";
        QFile configFile(configPath + "/save.json");
        if(!configFile.open(QIODevice::ReadOnly)){
            //qDebug() << "nije otvoren??";
        }else{
            //qDebug() << "otvoren";
        }
        QByteArray bytes = configFile.readAll();
        QJsonDocument configDocument = QJsonDocument::fromJson(bytes);
        QJsonObject configObject = configDocument.object();
        hubPath = configObject["hubPath"].toString();
    }
}

Config* Config::getConfig(){
    if(configPtr == nullptr){
        configPtr = new Config();
    }
    return configPtr;
}

void Config::setHubPath(QString newPath){
    hubPath = newPath;
    setConfig();
}

QString Config::getHubPath(){
    return hubPath;
}

void Config::setConfig(){

    QJsonObject configObject;
    configObject.insert("hubPath", hubPath);
//  configObject.insert("ime", vrednost);

    QJsonDocument configDocument;
    configDocument.setObject(configObject);
    QByteArray bytes = configDocument.toJson(QJsonDocument::Indented);

    QFile configFile(configPath+"/save.json");
    configFile.open( QIODevice::WriteOnly | QFile::Text | QIODevice::Truncate );

    QTextStream iStream(&configFile);
    iStream << bytes;
    configFile.close();

}

QString Config::getConfigPath(){
    return configPath;
}
