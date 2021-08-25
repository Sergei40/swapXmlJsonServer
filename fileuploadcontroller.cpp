#include <iostream>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include "fileuploadcontroller.h"
#include "global.h"

#define REQ_FILE_SIZE 2000000;

FileUploadController::FileUploadController() { }

void FileUploadController::service(HttpRequest& request, HttpResponse& response)
{
    //-------------------------------------------------------------------------
    //--POST method processing
    //-------------------------------------------------------------------------
    if (request.getMethod() == "POST")
    {
        QTemporaryFile* jsonFile = request.getUploadedFile("json");
        QTemporaryFile* xmlFile = request.getUploadedFile("xml");
        if (jsonFile)
        {
            response.setHeader("Content-Type", "appcitaion/xml");
            QString fileName = request.getParameter("json");
            QString path = searchStorageDir(fileName) ;

            savePostFile(path + fileName, jsonFile);

        }
        else if (xmlFile)
        {
            response.setHeader("Content-Type", "appcitaion/json");
            QString fileName = request.getParameter("xml");
            QString path = searchStorageDir(fileName);
            std::cout << qPrintable(path) << std::endl;
            savePostFile(path + fileName, xmlFile);
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
        QTemporaryFile* jsonFile = request.getUploadedFile("json");
        QTemporaryFile* xmlFile = request.getUploadedFile("xml");
        if (jsonFile)
        {
            response.setHeader("Content-Type", "appcitaion/xml");
        }
        else if (xmlFile)
        {
            response.setHeader("Content-Type", "appcitaion/json");
        }
        else
        {
            response.write("upload failed");
        }
    }
    else if (request.getParameter("action") == "show")
    {
        response.setHeader("Content-Type", "image/jpeg");
        QTemporaryFile* file = request.getUploadedFile("file1");
        if (file)
        {
            while (!file->atEnd() && !file->error())
            {
                QByteArray buffer = file->read(65536);
                response.write(buffer);
            }
        }
        else
        {
            response.write("upload failed");
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
            return storageDir + "/";
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

QString FileUploadController::savePostFile(QString path, QTemporaryFile* tempFile)
{
    QString fileName = path;
    QFile file(fileName);

    quint64 copyNumber = 1;
    QRegularExpression regCopyNum("([(](?=\\d)\\d{1,}(?<=\\d])[)])(?=[.])");
    while (file.exists())
    {
        if (!fileName.contains(regCopyNum))
        {
            fileName.insert(fileName.lastIndexOf("."),
                            "(" + QString::number(copyNumber) + ")");
            file.setFileName(fileName);
        }
        else
        {
            fileName.replace(regCopyNum,
                             "(" + QString::number(copyNumber) + ")");
            file.setFileName(fileName);
        }
        copyNumber++;
    }
    std::cout << "fileName -------" << qPrintable(fileName) << std::endl;

    file.open(QIODevice::WriteOnly);
    QByteArray buffer = tempFile->read(2000000);

}
