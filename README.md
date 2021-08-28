# swapXmlJsonServer
+ Qt 6.1.2
+ qmake
+ compiler - MinGW 8.1.0

## Примеры запросов из PowerShell

```powershell
curl.exe -i -X POST -F "json=@C:/Users/Sergei/Desktop/test1.json" http://localhost:8080/files/
curl.exe -i -X POST -F "xml=@C:/Users/Sergei/Desktop/test.xml" http://localhost:8080/files/
curl.exe -i -X PUT -F "xml=@C:/Users/Sergei/Desktop/test.xml" http://localhost:8080/files/test%281%29.xml
curl.exe -i -X DELETE http://localhost:8080/files/test1%281%29.json
```
в последних двух запросах символами %28 и %29 закодированы круглые скобки, открывающая и закрывающая соответственно, т.к. использование этих символов в URI напрямую не допускается.
