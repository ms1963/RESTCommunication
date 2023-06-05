/*
CallRestAPI
Dieser Arduino-Sketch läuft auf einem Arduino Giga Board, lässt sich aber auch
auf anderen Arduinos nutzen, wenn ein paar Änderungen erfolgen. Das gilt auch,
sollte eine andere Art bzw. ein anderer Sensor angeschlossen werden.
#include <WiFi.h> und #include <WifiUdp.h> müssen durch ihre Pendants auf der
Zielplattform ausgetauscht werden.
Achtung: Die Echtzeituhr wird hier auf die UTC-Zeit (GMT) gesetzt. 
*/

#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>      // notwendig für NTP-Kommunikation
#include <mbed_mktime.h> // ""
#include <Wire.h> // Annahme: Nutzen des primären I2C-Ports (Pins 20, 21 auf Giga, Due, Mega)
#include <SPI.h> // Nur für den SPI benötigt
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "arduino_secrets.h"
#include <sstream>

// für SPI-basierte BME680-Breakout-Boards
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define DELAY_TIME (30 * 1000) // 30 Sekunden Pause zwischen den Messungen

#define SEALEVELPRESSURE_HPA (1016.3) // Beispielsort: München

Adafruit_BME680 bme; // I2C: Ändern, falls SPI benutzt wird
//Adafruit_BME680 bme(BME_CS); // Hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

int status = WL_IDLE_STATUS;

char ssid[] = SECRET_SSID; // Passwort und SSID werden aus arduino_secrets.h entnommen
char pass[] = SECRET_PASS;

char serverAddress[] = "192.168.178.173";  // Serveradresse: auf eigene Server IP ändern!!!
int port = 8080;
String APIPath = "/measurements/api"; // z.B. wird daraus serverIP:port/measurements/api

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port); // HTTP-Verbindung zur Zieladresse des REST-API-Servers

unsigned int localPort = 2390; // Lokaler Port, um auf UdP-Pakete zu warten
constexpr auto timeServer { "dptbtime1.ptb.de" };  // Genutzter NTP-Server
const int NTP_PACKET_SIZE = 48; // NTP Zeitstempelbefindet sich in den ersten 48 Bytes der Nachricht
byte packetBuffer[NTP_PACKET_SIZE]; // Buffer für ein- und ausgehende  Pakete
// Eine UDP Instanz erlaubt das Senden/Empfangen von Paketen über UDP
WiFiUDP Udp;

void setup() {
  // Seriellen Stream starten
  Serial.begin(9600);

  // Verbindungsaufbau WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Versuche Verbindungsaufbau zum WLAN-Netz: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Verbinden mit WPA/WPA2 Netzwerk:
    status = WiFi.begin(ssid, pass);
  }

  // Name des WLANs nach erfolgtem Verbindungsaufbau:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Welche IP-Adresse wurde dem Board bzw. WiFi-Modul zugewiesen:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Adresse: ");
  Serial.println(ip);

  // Nun lässt sich die RTC (Real Time Clock) konfigurieren:
  Serial.println("Setze über einen NTP-Server die aktuelle UTC-Zeit");
  setNtpTime(); // Korrekte Zeit einstellen
  Serial.print("Aktuelles Datum/aktuelle Zeit: ");
  Serial.println(getLocaltime());
  // Verbindung mit Sensor BME680 etablieren
  if (!bme.begin()) {
    Serial.println(F("Konnte keinen Sensor finden. Bitte Schaltung überprüfen!"));
    while (1);
  }

  // Oversampling-Werte setzen und IIR-Filter initialisieren
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C für 150 ms
}

void loop() {
  // Sensordaten vom BME680 holen
  if (!bme.endReading()) {
    Serial.println(F("Konnte Sensordaten nicht einlesen :("));
    return;
  }
 
  // Datum und Zeit ermitteln
  String contentType = "application/json";
  String dateTime = getLocaltime(); // z.B.: 2023-06-03 12:30:00
  String date = dateTime.substring(0, 10); // Datum extrahieren z.B.: 2023-06-03
  String time = dateTime.substring(11,20); // Zeit extrahieren  z.B.: 12:30:00

  // Daten (Werte aus RTC und Sensor) zum Durchführen eines POST-Aufrufs übergeben
  // Druck (pressure) in hP, Widerstand in kOhm
  callAPIPost(bme.temperature, bme.humidity, bme.pressure / 100.0, bme.gas_resistance / 1000.0, date, time);

  // Wartezeit zwischen Sensormessungen
  Serial.print(DELAY_TIME / 1000.0);
  Serial.println(" Sekunden warten");
  delay(DELAY_TIME);
}

// Hier erfolgt die Vorbereitung und die eigentliche Durchführung des POST-Aufrufes / REST-PUSH 
void callAPIPost(double temperature, double humidity, double pressure, double resistance, const String& date, const String& time){
  Serial.println("Durchführung eines neuen REST API POST Aufrufs:");
  String contentType = "application/json"; // Wir übergeben ein JSON-Objekt

  // JSON Nutzlast (Body) kreieren
  String postData = createJSONString(temperature, humidity, pressure, resistance, date, time);
  Serial.println(postData); // ... und ausgeben

  // Per WiFi-Verbindung POST aufrufen und dabei "application/json" und das JSON-Objekt übergeben
  client.post("/measurements/api", contentType, postData);

  // Status und Body aus Antwort extrahieren
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  // Ausgabe der Ergebnisse:
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Antwort: ");
  Serial.println(response);
}


// Hier wird ein String generiert, der ein JSON-Objekt enthält
String createJSONString(double temp, double humi, double pres, double resi, String date, String time) {
  String result = "{";
  const String up = "\"";
  const String delim = ",";
  const String colon = ":";

  result += up + "temperature" + up + colon + String(temp,2) + delim;
  result += up + "humidity"    + up + colon + String(humi,2) + delim;
  result += up + "pressure"    + up + colon + String(pres,2) + delim;
  result += up + "resistance"  + up + colon + String(resi,2) + delim;
  result += up + "date"        + up + colon + up + date + up + delim;
  result += up + "time"        + up + colon + up + time + up;
  result += "}";
  return result;
}




// NTP-Anfrage senden, Antwort erhalten und parsen
void setNtpTime()
{
    status = WL_IDLE_STATUS;
    Udp.begin(localPort);
    sendNTPpacket(timeServer);
    delay(1000);
    parseNtpPacket();
}

// Rufe einen Zeitserver auf 
unsigned long sendNTPpacket(const char * address)
{
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011; // LI, Version, Modus
    packetBuffer[1] = 0; // Stratum, Art der Uhr
    packetBuffer[2] = 6; // Abfrageintervall
    packetBuffer[3] = 0xEC; // Präzision der Uhr
    // 8 Bytes von Nullen für Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    Udp.beginPacket(address, 123); // NTP Aufrufe erfolgen über Port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

// Hier wird das NTP-Antwortobjekt empfangen und geparst:
unsigned long parseNtpPacket()
{
    if (!Udp.parsePacket())
        return 0;

    Udp.read(packetBuffer, NTP_PACKET_SIZE); // Paket vom NTP-Server empfangen
    const unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    const unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    const unsigned long secsSince1900 = highWord << 16 | lowWord;
    constexpr unsigned long seventyYears = 2208988800UL;
    const unsigned long epoch = secsSince1900 - seventyYears;
    set_time(epoch);

// Folgende ausführliche Beschreibung lässt sich ausgeben,
// sobald man DETAILS definiert
#if defined(DETAILS)
    Serial.print("Sekunden seit Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // NTP time in "echte" Zeit umwandeln:
    Serial.print("Unix Zeit = ");
    // Ausgabe der Unix time:
    Serial.println(epoch);

    // Stunde, Minute, Sekunde ausgeben:
    Serial.print("Die UTC Zeit ist "); // UTC entspricht Greenwich Meridian (GMT)
    Serial.print((epoch % 86400L) / 3600); // Stunde ausgeben (86400 sind die Sekunden pro Tag)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
        // In den ersten 10 Minuten einer Stunde brauchen wir eine führende Null
        Serial.print('0');
    }
    Serial.print((epoch % 3600) / 60); // Minute ausgeben (3600 = Sekunden pro Stunde)
    Serial.print(':');
    if ((epoch % 60) < 10) {
        // In den ersten 10 Minuten einer Stunde brauchen wir eine führende Null
        Serial.print('0');
    }
    Serial.println(epoch % 60); // Sekunde ausgeben
#endif

    return epoch;
}

// Lokale Zeit mittels RTC (Real Time Clock) ermitteln:
String getLocaltime()
{
    char buffer[32];
    tm t;
    _rtc_localtime(time(NULL), &t, RTC_FULL_LEAP_YEAR_SUPPORT);
    strftime(buffer, 32, "%Y-%m-%d %k:%M:%S", &t);
    return String(buffer);
}




