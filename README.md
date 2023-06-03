# RESTCommunication

Here you find the implementation of a REST API that is implemented in Java (SDK 17) using Spring Boot 3, Docker and postgreSQL.

This API is then remotely accessed by a Arduino Giga R1 (note: other Arduino boards may be used as well) that integrates a Bosch BME680 sensor, a RTC, and a WiFi module. 

Every preconfigured time interval the Arduino sends a POST call with the read sensor data, date, and time to the server which stores the measurement in the database. Please change your server address in the Arduino code (.ino file). 
You also need to enter the SSID of your WLAN and the password/authorization key.

At the server side you need to set your password and username in application.xml and docker_compose.yml. The entries in both files must be identical.
Start docker by: 'docker compose up' in the directory where the .yml file is located.
Then execute "docker exec -it postgres bash' to enter the container. In the container's shell run "psql -U <username>" (according to application.xml and docker_compose.yml).
Now, you are able to create the database: "CREATE DATABASE measurement;". Don't forget the semicolon. With "\l" you may get a list of all databases. Among them you should see a database with name measurement.
After that, you may connect to the database through the command "\c measurement". Use "\dt" to see the relations defined in the DB.
  
