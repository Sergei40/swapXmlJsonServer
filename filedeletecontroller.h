#ifndef FILEDELETECONTROLLER_H
#define FILEDELETECONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
  This controller displays a HTML form for file upload and recieved the file.
*/
class FileDeleteController : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(FileDeleteController)
public:
    /** Constructor */
    FileDeleteController();

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // FILEDELETECONTROLLER_H
