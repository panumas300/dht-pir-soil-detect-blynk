#define BLYNK_TEMPLATE_ID "TMPL12sjAv7o"
#define BLYNK_DEVICE_NAME "DHT22"
#define BLYNK_AUTH_TOKEN "fGJoQ5D2H_qCEuZgUIuYKAE383WAJ9AY"
#define PIR_MOTION_SENSOR D2//Use pin 2 to receive the signal from the module
#include "DHT.h"
#define DHTPIN D4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent = 0;

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "B4_03";
char pass[] = "Appletv403";
BlynkTimer timer;



void sendSensor() {
soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent >= 100)
{
  Blynk.virtualWrite(V2, 100);
  Serial.println("100 %");
}
else if(soilmoisturepercent <=0)
{
  Blynk.virtualWrite(V2, 0);
  Serial.println("0 %");
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
  Blynk.virtualWrite(V2, soilmoisturepercent);
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  
}
  delay(250);

if(digitalRead(PIR_MOTION_SENSOR)){//if it detects the moving people?
        Serial.println("Hi,people is coming");
        Blynk.virtualWrite(V3, "Hi, people is coming");
        Blynk.virtualWrite(V4, 100);
}
    else{
        Serial.println("Watching");
        Blynk.virtualWrite(V3, "Watching");
        Blynk.virtualWrite(V4, 0);
    }
 delay(1000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V4, t);
}



void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  Serial.println(F("DHTxx test!"));
  dht.begin();
  pinMode(PIR_MOTION_SENSOR, INPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
}


void loop()
{
  Blynk.run();
  timer.run();

}
