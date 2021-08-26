#include <iostream>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include "fileuploadcontroller.h"
#include "global.h"
#include "jsonxx.h"
#include "xml2json.hpp"
#include <QJsonDocument>

#define REQ_FILE_SIZE 2000000

FileUploadController::FileUploadController() { }

void FileUploadController::service(HttpRequest& request, HttpResponse& response)
{
    //-------------------------------------------------------------------------
    //--POST method processing
    //-------------------------------------------------------------------------
    if (request.getMethod() == "POST")
    {
        QTemporaryFile* jsonTmpFile = request.getUploadedFile("json");
        QTemporaryFile* xmlTmpFile = request.getUploadedFile("xml");
        if (jsonTmpFile)
        {
            response.setHeader("Content-Type", "appcitaion/xml");

            QString fileName = request.getParameter("json");
            QString path = searchStorageDir(fileName) ;
            QString realPath = savePostFile(path + "/" + fileName, jsonTmpFile);
            QByteArray uri = "http:://" + request.getHeader("host") +
                           realPath.sliced(realPath.indexOf("docroot")+7).toUtf8();
            response.setHeader("Location", uri);

            QFile jsonFile(realPath);
            jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);

            std::string jsonStr = jsonFile.readAll().toStdString();
            jsonFile.close();

            jsonxx::Object json;
            json.parse(jsonStr);
            std::string buffer = json.xml(jsonxx::TaggedXML);
            response.write(QByteArray::fromStdString(buffer));
        }
        else if (xmlTmpFile)
        {
            response.setHeader("Content-Type", "appcitaion/json");

            QString fileName = request.getParameter("xml");
            QString path = searchStorageDir(fileName);
            QString realPath = savePostFile(path  + "/" + fileName, xmlTmpFile);
            QByteArray uri = "http:://" + request.getHeader("host") +
                    realPath.sliced(realPath.indexOf("docroot")+7).toUtf8();
            response.setHeader("Location", uri);

            QFile xmlFile(realPath);
            xmlFile.open(QIODevice::ReadOnly | QIODevice::Text);

            const auto xmlStr = xml2json(xmlFile.readAll().constData());
            xmlFile.close();
            response.write(QByteArray::fromStdString(xmlStr));
        }
        else
        {
            response.write("upload failed");
        }
    }
    //-------------------------------------------------------------------------
    //--PUT method processing
    //-------------------------------------------------------------------------
    else if (request.getMethod() == "PUT")
    {
        QTemporaryFile* jsonTmpFile = request.getUploadedFile("json");
        QTemporaryFile* xmlTmpFile = request.getUploadedFile("xml");
        if (jsonTmpFile)
        {
            response.setHeader("Content-Type", "appcitaion/xml");

            QString fileName = request.getParameter("json");
            QString path = searchStorageDir(fileName);
            QString reqPath = request.getPath();
            QString realPath = path + reqPath.sliced(reqPath.lastIndexOf("/"));

            QFile jsonFile(realPath);
            if (jsonFile.exists())
            {
                jsonFile.open(QIODevice::WriteOnly);
                jsonFile.write(jsonTmpFile->read(REQ_FILE_SIZE));
                jsonFile.close();

                QByteArray uri = "http:://" + request.getHeader("host") +
                        realPath.sliced(realPath.indexOf("docroot")+7).toUtf8();
                response.setHeader("Location", uri);

                jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);

                std::string jsonStr = jsonFile.readAll().toStdString();
                jsonFile.close();

                jsonxx::Object json;
                json.parse(jsonStr);
                std::string buffer = json.xml(jsonxx::TaggedXML);
                response.write(QByteArray::fromStdString(buffer));
            }
            else response.write("upload failed, file not exists");
        }
        else if (xmlTmpFile)
        {
            response.setHeader("Content-Type", "appcitaion/json");

            QString fileName = request.getParameter("xml");
            QString path = searchStorageDir(fileName);
            QString reqPath = request.getPath();
            QString realPath = path + reqPath.sliced(reqPath.lastIndexOf("/"));

            QFile xmlFile(realPath);
            if (xmlFile.exists())
            {
                xmlFile.open(QIODevice::WriteOnly);
                xmlFile.write(xmlTmpFile->read(REQ_FILE_SIZE));
                xmlFile.close();

                QByteArray uri = "http:://" + request.getHeader("host") +
                        realPath.sliced(realPath.indexOf("docroot")+7).toUtf8();
                response.setHeader("Location", uri);

                xmlFile.open(QIODevice::ReadOnly | QIODevice::Text);

                const auto xmlStr = xml2json(xmlFile.readAll().constData());
                xmlFile.close();
                response.write(QByteArray::fromStdString(xmlStr));
            }
            else response.write("upload failed, file not exists");
        }
        else
        {
            response.write("upload failed, file not found");
        }
    }
    else
    {
        response.setHeader("Content-Type", "text/html; charset=UTF-8");
        response.write("Upload failed. Wrong URI");
    }
}

QString FileUploadController::searchStorageDir(QString fileName)
{
    QString binDir = QCoreApplication::applicationDirPath();
    QString appName = QCoreApplication::applicationName();

    QStringList searchList;
    searchList.append(binDir + "/docroot/files/");
    searchList.append(binDir + "/../docroot/files/");
    searchList.append(binDir + "/../" + appName + "/docroot/files/");
    searchList.append(binDir + "/../../" + appName + "/docroot/files/");
    searchList.append(binDir + "/../../../" + appName + "/docroot/files/");

    QString storageDir;
    foreach (QString path, searchList)
    {
        QDir dir(path);
        if (dir.exists())
        {
            storageDir = QDir(dir).canonicalPath();
            qDebug("Using storage directory %s", qPrintable(storageDir));
            return storageDir;
        }
    }

    // not found
    foreach (QString path, searchList)
    {
        qWarning("%s/%s not found", qPrintable(path), qPrintable(fileName));
    }
    qFatal("Cannot find storage directory %s", qPrintable(fileName));
    return nullptr;
}

QString FileUploadController::savePostFile(QString path,
                                           QTemporaryFile* tempFile)
{
    QString fileName = path;
    QFile file(fileName);

    quint64 copyNumber = 1;
    QRegularExpression copyNum("([(](?=[0-9])[0-9]{1,}(?<=[0-9])[)])(?=[.])");
    while (file.exists())
    {
        if (!fileName.contains(copyNum))
        {
            fileName.insert(fileName.lastIndexOf("."),
                            "(" + QString::number(copyNumber) + ")");
            file.setFileName(fileName);
        }
        else
        {
            fileName.replace(copyNum,
                             "(" + QString::number(copyNumber) + ")");
            file.setFileName(fileName);
        }
        copyNumber++;
    }

    file.open(QIODevice::WriteOnly);
    file.write(tempFile->read(REQ_FILE_SIZE));
    file.close();
    return fileName;
}
