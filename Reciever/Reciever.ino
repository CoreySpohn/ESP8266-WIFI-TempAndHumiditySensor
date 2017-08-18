/* 
 * This is a program for the reciever inside.
 * It will use get the data from the sensor's server and
 * display it on an LCD screen.
 */

#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <math.h>

const int rs = 5, en = 4, d4 = 0, d5 = 2, d6 = 14, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char* ssid = "ANETWORK";
const char* password = "1956ACE126";

IPAddress server(192, 168, 1, 80);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(2000);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello!");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

}

int value = 0;

void loop() {
  value++;

  client.connect(server, 80);
  client.println("Wake the fuck up.\r");
  delay(100);
  String humidity = client.readStringUntil('`');
  //delay(100);
  String temperature = client.readStringUntil(':');
  //delay(100);
  String heatIndex = client.readStringUntil('\r');

  humidity.toFloat();
  temperature.toFloat();
  
  Serial.println("Humidity: " + humidity);
  Serial.println("Temperature: " + temperature);
  client.flush();

  if(humidity.toFloat() == 0.00){
    // This part is used to prevent the LCD from updating if something goes wrong
    // in the transmission or string manipulation
    Serial.println(humidity.toFloat());
    return;
    }
  
  // set the cursor to column 0, line 0
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print("Temp:    ");
  lcd.print(temperature);
  lcd.print((char)178);
  lcd.print("F");

  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.print(humidity);
  lcd.print("%");
  
  delay(2000);
}
