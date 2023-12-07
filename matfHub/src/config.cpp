#include "../include/config.hpp"

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

Config* Config::configPtr = nullptr;

Config::Config(){
    QString currentPath = QDir::currentPath();
    if(!QDir(currentPath+"./config").exists()){
        qDebug() << "plsno";
        hubPath = "MATF";
        configPath = currentPath + "/.config"; //const
        QDir(currentPath).mkdir(".config");
    }else{
        qDebug()<<"pls";
        configPath = currentPath + "/.config";
        QFile configFile(configPath);
        configFile.open(QIODevice::ReadOnly);
        QByteArray bytes = configFile.readAll();
        QJsonDocument configDocument = QJsonDocument::fromJson(bytes);
        QJsonObject configObject = configDocument.object();
        auto test = configObject.value("hubPath");
        qDebug() << test;
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
    qDebug() << configPath;
    configFile.open( QIODevice::WriteOnly | QFile::Text | QIODevice::Truncate );
    qDebug() << configFile.isOpen();

    QTextStream iStream(&configFile);
    iStream << bytes;
    configFile.close();

}
