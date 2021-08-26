/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include "global.h"
#include "requestmapper.h"
#include "filelogger.h"
#include "staticfilecontroller.h"
#include "fileuploadcontroller.h"
#include "filedeletecontroller.h"

RequestMapper::RequestMapper(QObject* parent) : HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
}

RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}

void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path = request.getPath();
    qDebug("RequestMapper: path=%s", path.data());

    // For the following pathes, each request gets its own new instance of the
    // related controller.

    if (request.getMethod() == "GET")
    {
        staticFileController->service(request, response);
    }
    else if (path.startsWith("/files") &&
             (request.getMethod() == "POST" || request.getMethod() == "PUT"))
    {
        FileUploadController().service(request, response);
    }
    else if (request.getMethod() == "DELETE")
    {
        FileDeleteController().service(request, response);
    }

    qDebug("RequestMapper: finished request");

    // Clear the log buffer
    if (logger)
    {
        logger->clear();
    }
}
