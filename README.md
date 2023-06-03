# RESTCommunication

Here you find the implementation of a REST API that is implemented in Java (SDK 17) using Spring Boot 3, Docker and postgreSQL as well as an Arduino client that calls this API remotely.
This code is part of a two article series at Heise Developer (https://www.heise.de/developer/Der_Pragmatische_Architekt-2732748.html). Here you'll also find more architectural and technical details.

This API can be remotely accessed by a Arduino Giga R1 (note: other Arduino boards may be used as well) that integrates a Bosch BME680 sensor, a RTC, and a WiFi module. 

Every preconfigured time interval the Arduino sends a POST call with the read sensor data, date, and time to the server which stores the measurement in the database. Please change your server address in the Arduino code (.ino file). 
You also need to enter the SSID of your WLAN and the password/authorization key.

At the server side you need to set your password and username in application.xml and docker_compose.yml. The entries in both files must be identical.

Start docker by: 'docker compose up' in the directory where the .yml file is located.

Then execute "docker exec -it postgres bash' to enter the container. In the container's shell run "psql -U <username>" (according to application.xml and docker_compose.yml).
  
Now, you are able to create the database: "CREATE DATABASE measurement;". Don't forget the semicolon. With "\l" you may get a list of all databases. Among them you should see a database with name measurement.
  
After that, you may connect to the database through the command "\c measurement". Use "\dt" to see the relations defined in the DB.
  
When you are finished you may build the project using maven or the maven wrapper (mvnw). Or you may use IntelliJ IDEA Ultimate edition for managing respectively building and executing the Java code.
  
I recommend tu use Postman (https://www.postman.com/) for calling the GET/PUT/DELETE endpoints. For GET and DELETE calls select none for the body. For PUT or POST-Calls under body select raw and then JSON. Fill out the JSON form which should look like:
  
  {
    "temperature" : 12.6,
    "humidity" : 56.98,
    "pressure" : 90000,
    "resistance": 45.87,
    "date" : "2023-06-04",
    "time" : "12:00:13"
  }

  
