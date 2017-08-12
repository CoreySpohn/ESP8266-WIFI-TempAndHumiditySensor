#include "DHT.h"    // Adafruit library, needs DHT-sensor-library and Adafruit_Sensor

#define DHTPIN 2     // Pin D4 on Wemos D1 Mini
#define DHTTYPE DHT11   // Defining DHT type for Adafruit libraryDHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);

  // Read the info
  float h = dht.readHumidity(); // Read the humidity
  float t = dht.readTemperature(true); // Read the temperature (true) for fahrenheit

  // Check to verify it's working
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from sensor");
    return;
    }

  // Compute the heat index in Fahrenheit
  float hi = dht.computeHeatIndex(t, h);

  Serial.println("Humidity: ");
  Serial.print(h);
}
