//Add Library
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define CONNECTION_TIMEOUT 10
#define rstRF D1
#define sdaRF D2

//Pin variable
//DONT EVER USE D8 OR CAUSE WATCHDOG ERROR
const int buzzer = D4, sckRF = D5, mosiRF = D7, misoRF = D6;
int pinOut[] = { buzzer };

//LEDC SETUP
const int ledcChannel = 0, ledcTimer = 8, ledcBaseFreq = 2000;

MFRC522 mfrc522(sdaRF, rstRF);

//Wifi SSID
const char *ssid = "Hurrdurr", *pw = "wumbomumbo";

char server[255];

//WifiManager Object
WiFiManager wm;
WiFiClient espClient;

//MQTT Object
PubSubClient client(espClient);

//Changeable variable
int tryDelay = 500;
String hexID;
char espId[15];
int port;

void scanCard(String& card) {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println("Available Card");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    card.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  card.toUpperCase();
  hexID = card;
}

//MQTT CALLBACK
/*
  May god help you like it help me understand this function
*/
void callback(char* topic, byte* payload, unsigned int length) {
  char response[length + 1];
  memcpy(response, payload, length);
  response[length] = '\0';
  Serial.println(response);
  Serial.println(topic);
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, response);
  if (error) {
    // parsing failed
    Serial.println(error.c_str());
    return;
  }
  JsonArray statusCodeArray = doc["statusCode"];
  if (!statusCodeArray.isNull() && statusCodeArray.size() >= 1) {
    String statusCode = statusCodeArray[0];
    Serial.println(statusCode);
    if (statusCode == "200") {
      confirmTone(buzzer);
    } else {
      deadTone(buzzer);
    }
  }
}

//Piezzo tone
void confirmTone(int pin) {
  tone(pin, 2500, 200);
  delay(200);
  noTone(pin);
  tone(pin, 2500, 200);
  delay(100);
  noTone(pin);
}

void deadTone(int pin) {
  tone(pin, 1000, 500);
  delay(1000);
  noTone(pin);
  tone(pin, 500, 1000);
  delay(1000);
  noTone(pin);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(1000);
  Serial.println("SPI START");
  /*
  for(int x : pinOut){
    pinMode(x, OUTPUT);
  } 
  */
  pinMode(buzzer, OUTPUT);
  WiFi.mode(WIFI_STA);
  bool res;
  wm.setDebugOutput(false);
  wm.resetSettings();
  WiFiManagerParameter mqtt_server("server_address", "Enter MQTT Server Address", "http://0.0.0.0", 255);
  wm.addParameter(&mqtt_server);
  WiFiManagerParameter mqtt_port("port_address", "Enter MQTT Port", "1883", 255);
  wm.addParameter(&mqtt_port);
  res = wm.autoConnect("ESP CONFIG", "siotics0rgConf");
  if (!res) {
    Serial.println("Failed to connect");
    ESP.restart();
    delay(1000);
  } else {
    Serial.print("Connected to Wifi with IP: ");
    Serial.println(WiFi.localIP());
  }
  Serial.print("Server IP: ");
  strcpy(server, mqtt_server.getValue());
  port = atoi(mqtt_port.getValue());
  Serial.println(server);
  delay(tryDelay);
  if (WiFi.status() == WL_CONNECTED) {
    confirmTone(buzzer);
  }
  sprintf(espId, "%06X", ESP.getChipId());
  client.setServer(server, port);
  client.setCallback(callback);
  while (!client.connected()) {
    if (client.connect(espId)) {
      Serial.println("Connected to MQTT broker");
    } else {
      deadTone(buzzer);
      Serial.println("Failed to connect. Retrying...");
      delay(1000);
    }
  }
  String topic = "esp/" + String(espId) + "/response";
  client.subscribe(topic.c_str());
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi disconnected");
    Serial.println("Retrying...");
    WiFi.begin(ssid, pw);
    delay(5000);
  }

  if (!client.connected()) {
    reconnect();
    return;
  }
  client.loop();
  scanCard(hexID);
  Serial.println(hexID);
  delay(500);
  if (hexID.length() != 0) {
    String attendance = "esp/attendance";
    String espIdStr = espId;
    String hexIDStr = hexID;
    String msg = "{\"espId\":\"" + espIdStr + "\",\"hexID\":\"" + hexIDStr + "\"}";
    client.publish(attendance.c_str(), msg.c_str());
    hexID = "";
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(espId)) {
      Serial.println("connected");
      String topic = "esp/" + String(espId) + "/response";
      client.subscribe(topic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}