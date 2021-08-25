/**
  @file
  @author Stefan Frings
*/

#ifndef FILEUPLOADCONTROLLER_H
#define FILEUPLOADCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
  This controller displays a HTML form for file upload and recieved the file.
*/


class FileUploadController : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(FileUploadController)
public:

    /** Constructor */
    FileUploadController();

    /** search docroot*/
    QString searchStorageDir(QString fileName);

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    /** creates a new file each time.
     *  If the file names are the same, adds numbering to the name.
     *  And returns the URI.*/
    QString savePostFile(QString path, QTemporaryFile* file);
};

#endif // FILEUPLOADCONTROLLER_H

