#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>

class MainWindow;
class Config
{

private:
    Config();
    static Config* configPtr;
    QString configPath;
    QString hubPath;
    //kolor shema
    //smer
    //itd
    void setConfig();

public:
    Config(const Config& obj) = delete;
    Config operator=(const Config& obj) = delete;

    static Config* getConfig();
    void setHubPath(QString newPath);
    QString getHubPath();
    QString getConfigPath();

};

#endif //CONFIG_H
