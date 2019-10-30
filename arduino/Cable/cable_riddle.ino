#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

// Configuration of WLAN
const char* SSID = "intia";
const char* PSK = "BuntesLicht10";

WiFiClient espClient;

//Configuration for MQTT
const char* MQTT_BROKER = "192.168.43.184";
const char* TOPIC = "cable-riddle"; // Publish topic
const char* TOPIC_SUB = "cable-riddle/set"; // Subscribe topic

PubSubClient client(espClient);

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;
int waitForACK = 0;

// Cable pins
const int cable_amount=7;
const int cables[7] = {D1, D2, D3, D4, D5, D6, D7};
int cable_sum =0;

void setup() {
  
  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
  
  Serial.begin(115200);
  Serial.println("Boot RFID-Reader...");

  // Set cables
  for(int i=0; i<cable_amount;i++){
    pinMode(cables[i],INPUT_PULLUP);
  }
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(waitForACK){
    Serial.println("Waiting.....");
  }
  
  cable_sum = calcCables();
  Serial.println(cable_sum);
  doc["value"]=cable_sum;

  char message[256];
  serializeJson(doc, message);
  Serial.println(message);
  client.publish(TOPIC, message);
  waitForACK = 1;
  delay(1000);
}

int calcCables(){
  int factor = 1;
  int sum=0;
  for(int i=0;i<cable_amount;i++){
    int val = digitalRead(cables[i]);
    sum = factor*val + sum;
    factor*=10;
  }
  return sum;
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
    if (!client.connect("ESP8266Client")) {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
  client.subscribe(TOPIC_SUB);
  Serial.println("MQTT Connected...");
}
