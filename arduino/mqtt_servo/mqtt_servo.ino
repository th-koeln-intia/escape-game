// Dieses Sketch steuert einen Servo. Dieser Wird in eine Kiste verbaut und somit kann die Kiste über MQTT geöffnet und verschlossen werden.

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <string.h>
#include <ArduinoJson.h>

const char* SSID = "intia";
const char* PSK = "BuntesLicht10";
const char* MQTT_BROKER = "intia.local";
const char* TOPIC = "servo/get"; // Toppic an das gesendet wird

const char* TOPIC_SUB = "servo/set"; // Topic das aboniert wird
//Wird an dieses Topic "open" gesendet so öffnet sich die Kiste. "close" zum schließen.

WiFiClient espClient;
PubSubClient client(espClient);

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;
Servo servo;

const int servoPin = D4;     // Servo-Pin
const int ledPin =  D2;      // LED-Pin


void setup() {
  pinMode(ledPin, OUTPUT);
  servo.attach(servoPin); //D4
  servo.write(0);;
  Serial.begin(115200);

  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
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

  if (strcmp(msg, "on") == 0) {
    digitalWrite(ledPin, HIGH);
  }
  else if (strcmp(msg, "off") == 0) {
    digitalWrite(ledPin, LOW);
  }
  else if (strcmp(msg, "open") == 0) {
    servo.write(768);
  }
  else if (strcmp(msg, "close") == 0) {
    servo.write(98);
  }
  else {
    int val = strtol(msg, NULL, 16);
    Serial.println(val);
    servo.write(val);
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

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
