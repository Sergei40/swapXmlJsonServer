#include "filedeletecontroller.h"
#include "fileuploadcontroller.h"

FileDeleteController::FileDeleteController() { }

void FileDeleteController::service(HttpRequest& request, HttpResponse& response)
{
    response.setHeader("Content-Type", "text/html; charset=UTF-8");

    QString reqPath = request.getPath();
    QString fileName = reqPath.sliced(reqPath.lastIndexOf("."));
    QString path = FileUploadController::searchStorageDir(fileName);

    QString realPath = path + reqPath.sliced(reqPath.lastIndexOf("/"));

    QFile file(realPath);
    if (file.exists())
    {
        file.remove();

        QByteArray uri = "http:://" + request.getHeader("host")
                + realPath.sliced(realPath.indexOf("docroot") + 7).toUtf8();
        response.setHeader("Location", uri);

        response.write("File was deleted from server");
    }
    else
        response.write("Delete failed, file not exists");
}
