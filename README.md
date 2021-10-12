## swapXmlJsonServer
+ Qt 6.1.2
+ qmake
+ compiler - MinGW 8.1.0

## Описание
http-серверо бработывает четыре основных запросов (POST, PUT, GET, DELETE). Сервер принимает файл JSON возвращает XML и наоборот.

# POST-запрос
При поступлении POST-запроса, файл сохраняется на сервере коневертируется в другой формат. Конфертированный файл тоже сохраняется и отправляется клиенту. При нескольких одинаковых POST-запросов, каждый раз создается новая копия как входящего, так и конвертируемого файла, при этом к именам копий добавляется нумерация по порядку.

# PUT-запрос
Файл отправленный в PUT-запросе перезаписывается в файл с таким же названием на сервере. Если такого файла нет, клиенту отправляется ошибка. Перезаписанный файл конвертируется, сохраняется и отправляется клиенту.

# GET-запрос
При поступлении GET-запроса, клиенту возвращается файл, имя которого указано в конце запроса. Если такого файла не существует, клиенту возвращается ошибка.

# DELETE-запрос
При поступлении DELETE-запроса, клиенту возвращается сообщение о удалении файла, имя которого указано в конце запроса. Если такого файла не существует, клиенту возвращается ошибка.

## Примеры запросов из PowerShell

```powershell
curl.exe -i -X POST -F "json=@C:/Users/Sergei/Desktop/test1.json" http://localhost:8080/files/
curl.exe -i -X POST -F "xml=@C:/Users/Sergei/Desktop/test.xml" http://localhost:8080/files/
curl.exe -i -X PUT -F "xml=@C:/Users/Sergei/Desktop/test.xml" http://localhost:8080/files/test%281%29.xml
curl.exe -i -X DELETE http://localhost:8080/files/test1%281%29.json
curl.exe -i -X GET http://localhost:8080/files/test1.json
```
В последних двух запросах символами %28 и %29 закодированы круглые скобки, открывающая и закрывающая соответственно, т.к. использование этих символов в URI напрямую не допускается.
