/* 
 * This is a program for the sensor put outside.
 * It will use the sensor and run a server where the data
 * is hosted to be read by the display microcontroller.
 */

#include "DHT.h"    // Adafruit library, needs DHT-sensor-library and Adafruit_Sensor
#include <ESP8266WiFi.h>
#include <SPI.h>

#define DHTPIN 2     // Pin D4 on Wemos D1 Mini
#define DHTTYPE DHT11   // Defining DHT type for Adafruit libraryDHT 11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "ANETWORK";
const char* password = "1956ACE126";

WiFiServer server(80);

IPAddress ip(192, 168, 1, 80);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  dht.begin();

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    
  Serial.println("");
  Serial.println("WiFi Connected");

  // Start the server
  server.begin();
  Serial.println("Server Started");
  
  // Print the IP Address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
}

void loop() { 

  // -----[SENSOR]-----

  float hTotal = 0;
  float tTotal = 0;
  int fail = 0;

  // Check to verify it's working
  float h = dht.readHumidity(); // Read the humidity
  float t = dht.readTemperature(true); // Read the temperature (true) for fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from sensor");
    fail = 1;
    }

  for(int n = 0; n < 100000 ; n++){
    float h = dht.readHumidity(); // Read the humidity
    float t = dht.readTemperature(true); // Read the temperature (true) for fahrenheit

    hTotal += h;
    tTotal += t;
    }

  h = hTotal / 100000.0;
  t = tTotal / 100000.0;

  // Compute the heat index in Fahrenheit
  float hi = dht.computeHeatIndex(t, h);

  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Heat Index: ");
  Serial.println(hi);

  // -----[SERVER]-----
  // Check if a client has connected
  WiFiClient client = server.available(); // Client is a value of 0 = No connection or 1 = Connection

  if (!client) { // If no connection exit the loop
    return;
    }

  // Wait until the client sends some data
  int waitTime = 0;
  Serial.println("New client");
  while(!client.available()){
    delay(1);
    waitTime++;
    if (waitTime > 5000){
      // This will restart the system automatically if the client is taking a long time to connect
      // or does not connect for whatever reason causes the bug.
      return;
      }
    }

  /*
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("<font>");
  client.print(h);
  client.print(";");
  client.print(t);
  client.print(";");
  client.print(hi);
  client.println("</font>");
  client.println("</html>");
  */
  
  if (client) {
    if (client.connected()){
      Serial.println(".");
      String request = client.readStringUntil('\r');
      Serial.println(request);
      
      if (fail == 1) {
        client.flush();
        client.print("Failed to read sensor.");
        }
        
      else{
        client.flush();
        client.print(h);
        client.print("`");
        client.print(t);
        client.print(":");
        client.print(hi);
      }
      
    }
    client.stop();
  }

}
