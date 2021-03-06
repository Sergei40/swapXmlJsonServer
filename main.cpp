#include <QCoreApplication>
#include <QDir>
#include "global.h"
#include "httplistener.h"
#include "requestmapper.h"

using namespace stefanfrings;

/** Search the configuration file */
QString searchConfigFile();

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // Find the configuration file
    QString configFileName = searchConfigFile();

    // Configure logging into a file
    //    QSettings* logSettings=new
    //    QSettings(configFileName,QSettings::IniFormat,&app);
    //    logSettings->beginGroup("logging");
    //    FileLogger* logger=new FileLogger(logSettings,10000,&app);
    Logger* logger = new Logger(&app);
    logger->installMsgHandler();

    // Configure static file controller
    QSettings* fileSettings = new QSettings(configFileName,
                                            QSettings::IniFormat, &app);
    fileSettings->beginGroup("docroot");
    staticFileController = new StaticFileController(fileSettings, &app);

    // Configure and start the TCP listener
    QSettings* listenerSettings = new QSettings(configFileName,
                                                QSettings::IniFormat, &app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings, new RequestMapper(&app), &app);

    qWarning("Application has started");
    app.exec();
    qWarning("Application has stopped");
}

QString searchConfigFile()
{
    QString binDir = QCoreApplication::applicationDirPath();
    QString appName = QCoreApplication::applicationName();
    QString fileName("server1.ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir + "/etc");
    searchList.append(binDir + "/../etc");
    // for development with shadow build (Linux)
    searchList.append(binDir + "/../" + appName + "/etc");
    // for development with shadow build (Windows)
    searchList.append(binDir + "/../../" + appName + "/etc");
    searchList.append(QDir::rootPath() + "etc/opt");
    searchList.append(QDir::rootPath() + "etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir + "/" + fileName);
        if (file.exists())
        {
            fileName = QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s", qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found", qPrintable(dir), qPrintable(fileName));
    }
    qFatal("Cannot find config file %s", qPrintable(fileName));
    return nullptr;
}
