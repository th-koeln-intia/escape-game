#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

// Configuration of WLAN
const char* SSID = "intia";
const char* PSK = "BuntesLicht10";

WiFiClient espClient;

//Configuration for MQTT
//const char* MQTT_BROKER = "intia.local";
const char* MQTT_BROKER = "intia.local";
const char* TOPIC = "rfid-reader"; // Publish topic
const char* TOPIC_SUB = "rfid-reader/set"; // Subscribe topic

PubSubClient client(espClient);

// Define PINS for MFRC522
#define SS_PIN D8
#define RST_PIN D3
const int ledPin = D4;
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;
int waitForACK = 0;

void setup(void){
  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
  
  SPI.begin();
  rfid.PCD_Init();
   
  Serial.begin(115200);
  Serial.println("Boot RFID-Reader...");
   
  pinMode(ledPin, OUTPUT);
}

void loop(void){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(waitForACK){
    digitalWrite(ledPin,HIGH);
    delay(200);
  }
  digitalWrite(ledPin,LOW);  
  handleRFID();
}
 
void handleRFID() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;
  Serial.println(printHex(rfid.uid.uidByte, rfid.uid.size));
  doc["id"]=printHex(rfid.uid.uidByte, rfid.uid.size);

  char message[256];
  serializeJson(doc, message);
  Serial.println(message);
  client.publish(TOPIC, message);
  waitForACK = 1;
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
 
String printHex(byte *buffer, byte bufferSize) {
  String id = "";
  for (byte i = 0; i < bufferSize; i++) {
    id += buffer[i] < 0x10 ? "0" : "";
    id += String(buffer[i], HEX);
  }
  return id;
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message [");
  Serial.print(topic);
  Serial.print("] ");
  char msg[length + 1];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i] = (char)payload[i];
  }
  Serial.println();

  msg[length] = '\0';
  Serial.println(msg);

  if (strcmp(msg, "{\"ack\":1}") == 0) {
     Serial.print("Acknowledged");
     waitForACK=0;
    //digitalWrite(ledPin, HIGH);
  }
  else if (strcmp(msg, "{\"ack\":0}") == 0) {
    Serial.print("not Acknowledged");
    waitForACK=0;
    //digitalWrite(ledPin, LOW);
  }
}
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.hostname("RFID-reader");
  WiFi.begin(SSID, PSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting MQTT...");
    if (!client.connect("rfid-reader")) {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
  client.subscribe(TOPIC_SUB);
  Serial.println("MQTT Connected...");
}
