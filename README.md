# RESTCommunication

ENGLISH (for GERMAN resp. DEUTSCH see below): 

Here you find the implementation of a REST API that is implemented in Java (SDK 17) using Spring Boot 3, Docker and postgreSQL as well as an Arduino client that calls this API remotely.
This code is part of a two article series at Heise Developer (https://www.heise.de/developer/Der_Pragmatische_Architekt-2732748.html). Here you'll also find more architectural and technical details.

This API can be remotely accessed by a Arduino Giga R1 (note: other Arduino boards and even other Microcontroller-Boards may be used as well) that integrates a Bosch BME680 sensor, a RTC, and a WiFi module. 

Every preconfigured time interval the Arduino sends a POST call with the read sensor data, date, and time to the server which stores the measurement in the database. Please change your server address in the Arduino code (.ino file). 
You also need to enter the SSID of your WLAN and the password/authorization key.

At the server side you need to set your password and username in application.yml and docker-compose.yml. The entries in both files must be identical. Per default "michael" is used for all these entries.

Start docker by: 'docker compose up' in the directory where the .yml file is located.

Then execute "docker exec -it postgres bash' to enter the container. In the container's shell run "psql -U <username>" (according to application.xml and docker_compose.yml).
  
Now, you are able to create the database: "CREATE DATABASE measurement;". Don't forget the semicolon. With "\l" you may get a list of all databases. Among them you should see a database with name measurement.
  
After that, you may connect to the database through the command "\c measurement". Use "\dt" to see the relations defined in the DB.
  
When you are finished you may build the project using maven or the maven wrapper (mvnw). Or you may use IntelliJ IDEA Ultimate edition for managing respectively building and executing the Java code. Spring Boot 3 (JPA) will take care of creating the required schema for the database measurement. It will also take care that the REST API is generated and provided on port 8080 of the host. You may change the port in application.yml.
  
I recommend to use Postman (https://www.postman.com/) for calling the GET/PUT/DELETE endpoints. For GET and DELETE calls select none for the body. For PUT or POST-Calls under body select raw and then JSON. Fill out the JSON form which should look like:
  
  {
    "temperature" : 12.6,
    "humidity" : 56.98,
    "pressure" : 90000,
    "resistance": 45.87,
    "date" : "2023-06-04",
    "time" : "12:00:13"
  }
  
  
Directories:
  - CallRestAPI: Arduino Code (C++)
  - CallRestAPI_OLED Arduino Code (C++)
  - RestAPIServer: Java Code for server incl. docker compose configuration

 DEUTSCH:
  
Hier finden Sie die Implementierung einer REST-API, die in Java (SDK 17) unter Verwendung von Spring Boot 3, Docker und postgreSQL implementiert ist, sowie einen Arduino-Client, der diese API aus der Ferne aufruft. Dieser Code ist Teil einer Serie von zwei Artikeln bei Heise Developer (https://www.heise.de/developer/Der_Pragmatische_Architekt-2732748.html). Hier finden Sie auch weitere architektonische und technische Details.

Diese API kann von einem Arduino Giga R1 (Hinweis: andere Arduino-Boards und sogar andere Microcontroller-können ebenfalls verwendet werden), in dem ein Bosch BME680-Sensor, eine RTC und ein WiFi-Modul integriert sind, aus der Ferne aufgerufen werden.

In jedem vorkonfigurierten Zeitintervall sendet der Arduino einen POST-Aufruf mit den gelesenen Sensordaten, Datum und Uhrzeit an den Server, der die Messung in der Datenbank speichert. Bitte ändern Sie Ihre Serveradresse im Arduino-Code (.ino-Datei). Außerdem müssen Sie die SSID Ihres WLANs und das Passwort/Autorisierungsschlüssel eingeben.

Auf der Serverseite müssen Sie Ihr Passwort und Ihren Benutzernamen in application.yml und docker-compose.yml festlegen. Die Einträge in beiden Dateien müssen identisch sein. Standardmäßig wird für alle diese Einträge "michael" verwendet.

Starten Sie docker durch: 'docker compose up' in dem Verzeichnis, in dem sich die .yml-Datei befindet.

Führen Sie dann "docker exec -it postgres bash" aus, um den Container zu starten. In der Shell des Containers führen Sie "psql -U " aus (gemäß application.xml und docker_compose.yml).

Nun können Sie die Datenbank erstellen: "CREATE DATABASE measurement;". Vergessen Sie das Semikolon nicht. Mit "\l" können Sie eine Liste aller Datenbanken erhalten. Darunter sollten Sie eine Datenbank mit dem Namen measurement sehen.

Danach können Sie mit dem Befehl "\c measurement" eine Verbindung zu dieser Datenbank herstellen. Verwenden Sie "\dt", um die in der DB definierten Beziehungen anzuzeigen.

Wenn Sie fertig sind, können Sie das Projekt mit Maven oder dem Maven-Wrapper (mvnw) erstellen. Oder Sie können IntelliJ IDEA Ultimate Edition zum Verwalten bzw. Erstellen und Ausführen des Java-Codes verwenden. Spring Boot 3 (JPA) wird sich um die Erstellung des erforderlichen Schemas für die Datenbankmessung kümmern. Es kümmert sich auch darum, dass die REST-API generiert und auf Port 8080 des Hosts bereitgestellt wird. Sie können den Port in der application.yml ändern.

Ich empfehle, Postman (https://www.postman.com/) für den Aufruf der GET/PUT/DELETE-Endpunkte zu verwenden. Für GET- und DELETE-Aufrufe wählen Sie none für den Body. Für PUT oder POST-Aufrufe wählen Sie unter body raw und dann JSON. Füllen Sie das JSON-Formular aus, das wie folgt aussehen sollte:

{ "temperature" : 12.6, "humidity" : 56.98, "pressure" : 90000, "resistance": 45.87, "date" : "2023-06-04", "time" : "12:00:13" }

Verzeichnisse:
  - CallRestAPI: Arduino Code (C++)
    - CallRestAPI_OLED Arduino Code (C++)
  - RestAPIServer: Java Code für den Server  inklusive einer Konfiguration für Docker Compose

  
