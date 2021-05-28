#include <Esp32MQTTClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "time.h"

#define DHT_PIN 21
#define DHT_TYPE DHT11

int interval = 6 * 1000;
const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
int ledPin = 5;
static char* connectionString = "HostName=Camilo-hubb.azure-devices.net;DeviceId=ESP32;SharedAccessKey=fN1qx5y8xiOnl8Oq82KbMvbS0+pX6hrhRRytkyJTivs=";
static bool _connected = false;

float temp = 0;
float prev = 0;
float diff = 1;



void initIotHub() {
  if (!Esp32MQTTClient_Init((const uint8_t * ) connectionString)) {
    _connected = false;

    return;
  }
  _connected = true;
}
char* _id = "40:EC:99:EC:28:0E";
char* ssid = "ASUS-RT";
char* pass = "nwmhcxpc";

bool messagePending = false;

char msg[512];
DHT dht(DHT_PIN, DHT_TYPE);

void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result) {
  if (result == IOTHUB_CLIENT_CONFIRMATION_OK) {
    Serial.println("Confirmed");
    messagePending = false;
  }
  else {
    Serial.println("Not confirmed");
  }
}


void sendIotMessage(char* msg) {
  EVENT_INSTANCE* message = Esp32MQTTClient_Event_Generate(msg, MESSAGE);
  Esp32MQTTClient_Event_AddProp(message, "Name", "Johane Camilo Forsberg Reyes");
  Esp32MQTTClient_Event_AddProp(message, "School", "Nackademin");
  Esp32MQTTClient_SendEventInstance(message);
}
unsigned long printLocalTime()
{
  time_t now;

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return 0;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  time(&now);
  return now;
}


void setup()
{

  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  initIotHub();
  dht.begin();



  if (Esp32MQTTClient_Init((const uint8_t *) connectionString))
  {
    Serial.println("Connected to MQTT Client");
  }
  Esp32MQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);
  configTime(0, 0, ntpServer);

  pinMode (ledPin, OUTPUT);


}
bool difference() {
  if (temp > (prev + diff) || temp < (prev - diff)) {
    prev = temp;
    return true;
  }
  return false;
}

void loop() {
  digitalWrite(ledPin, LOW);
  temp = dht.readTemperature();
  float hum = dht.readHumidity();
  delay(10);

  DynamicJsonDocument doc(512);
  JsonObject root = doc.to<JsonObject>();
  if (difference()) {
    //root["DeviceName"] = "ESP32";
    root["DeviceId"] = _id;
    root["TimeSent"] = printLocalTime();
    root["Temperature"] = temp;
    root["Humidity"] = hum;
    //root["Longitude"] = "17.912470";
    //root["Latitude"] = "59.411518";
    


    serializeJson(root, msg);


  //  if (!messagePending)
  //  {
  //    messagePending = true;
      sendIotMessage(msg);
      Serial.print(msg);
      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
    //  delay(6000);

 //   }

  }
}
