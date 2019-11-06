// Dieses Skript sendet den State eines Buttons mit dauer wie lange dieser gedrückt wurde.
// Zusätzlich gibt es eine LED die ebenfalls über MQTT gesteuert werden kann

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

const char* SSID = "intia";
const char* PSK = "BuntesLicht10";
const char* MQTT_BROKER = "192.168.1.102";
const char* TOPIC = "simon"; // Toppic an das gesendet wird

const char* TOPIC_SUB = "simon/set"; // Topic das Oboniert wird
//Wird an dieses Topic "on" gesendet so geht die LED an. "off" zum aus schalten.

WiFiClient espClient;
PubSubClient client(espClient);

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;


const int ledStatus =  D8;      // LED-Pin

const int ledRed =  D0;      // LED-Pin
const int ledYellow =  D1;   // LED-Pin
const int ledBlue =  D2;     // LED-Pin
const int ledGreen =  D3;    // LED-Pin

const int buttonRed = D4;     // Button-Pinconst
int buttonYellow = D5;     // Button-Pin
const int buttonBlue = D6;     // Button-Pin
const int buttonGreen = D7;     // Button-Pin

int buttonStateR = 1;        // Status des Buttons
int buttonStateY = 1;
int buttonStateB = 1;
int buttonStateG = 1;

// Variabeln um die Zeit zu berechnen
unsigned long startTime;
unsigned long pressTime;

void setup() {
  pinMode(ledStatus, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buttonRed, INPUT_PULLUP);
  pinMode(ledYellow, OUTPUT);
  pinMode(buttonYellow, INPUT_PULLUP);
  pinMode(ledBlue, OUTPUT);
  pinMode(buttonBlue, INPUT_PULLUP);
  pinMode(ledGreen, OUTPUT);
  pinMode(buttonGreen, INPUT_PULLUP);
  Serial.begin(115200);

  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
}

// Diese Funktion Empfängt Topics und printet diese in an Serial. Zusätzlich schaltet diese die LED.
// ToDO: LED Status kann noch nicht gezielt abgerufen werden.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message [");
  Serial.print(topic);
  Serial.print("] ");
  char msg[length + 1];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i] = (char)payload[i];
  }
  String msg_payload = msg;
  Serial.println();

  msg[length] = '\0';
  Serial.println(msg);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
  delay(500);
   if (strcmp(msg, "green") == 0) {
    digitalWrite(ledGreen, HIGH);
     delay(1000);
     digitalWrite(ledGreen, LOW);
  }else if (strcmp(msg, "green blue") == 0) {
    digitalWrite(ledGreen, HIGH);
     delay(1000);
     digitalWrite(ledGreen, LOW);
     delay(500);
     digitalWrite(ledBlue, HIGH);
     delay(1000);
     digitalWrite(ledBlue, LOW);
  }else if (strcmp(msg, "green blue blue") == 0) {
   digitalWrite(ledGreen, HIGH);
     delay(1000);
     digitalWrite(ledGreen, LOW);
     delay(500);
     digitalWrite(ledBlue, HIGH);
     delay(1000);
     digitalWrite(ledBlue, LOW);
     delay(500);
     digitalWrite(ledBlue, HIGH);
     delay(1000);
     digitalWrite(ledBlue, LOW);
  }else if (strcmp(msg, "green blue blue yellow") == 0) {
    digitalWrite(ledGreen, HIGH);
     delay(1000);
     digitalWrite(ledGreen, LOW);
     delay(500);
     digitalWrite(ledBlue, HIGH);
     delay(1000);
     digitalWrite(ledBlue, LOW);
     delay(500);
     digitalWrite(ledBlue, HIGH);
     delay(1000);
     digitalWrite(ledBlue, LOW);
     delay(500);
     digitalWrite(ledYellow, HIGH);
     delay(1000);
     digitalWrite(ledYellow, LOW);
  }else if (strcmp(msg, "green blue blue yellow red") == 0) {
      digitalWrite(ledGreen, HIGH);
     delay(1000);
     digitalWrite(ledGreen, LOW);
     delay(500);
     digitalWrite(ledBlue, HIGH);
     delay(1000);
     digitalWrite(ledBlue, LOW);
     delay(500);
     digitalWrite(ledBlue, HIGH);
     delay(1000);
     digitalWrite(ledBlue, LOW);
     delay(500);
     digitalWrite(ledYellow, HIGH);
     delay(1000);
     digitalWrite(ledYellow, LOW);
     delay(500);
     digitalWrite(ledRed, HIGH);
     delay(1000);
     digitalWrite(ledRed, LOW);
  }else if (strcmp(msg, "solved") == 0) {
      delay(300);
     digitalWrite(ledGreen, HIGH);
     digitalWrite(ledBlue, HIGH);
     digitalWrite(ledYellow, HIGH);
     digitalWrite(ledRed, HIGH);
     delay(300);
      digitalWrite(ledGreen, LOW);
     digitalWrite(ledBlue, LOW);
     digitalWrite(ledYellow, LOW);
     digitalWrite(ledRed, LOW);
     delay(300);
     digitalWrite(ledGreen, HIGH);
     digitalWrite(ledBlue, HIGH);
     digitalWrite(ledYellow, HIGH);
     digitalWrite(ledRed, HIGH);
     delay(300);
      digitalWrite(ledGreen, LOW);
     digitalWrite(ledBlue, LOW);
     digitalWrite(ledYellow, LOW);
     digitalWrite(ledRed, LOW);
     delay(300);
       digitalWrite(ledGreen, HIGH);
     digitalWrite(ledBlue, HIGH);
     digitalWrite(ledYellow, HIGH);
     digitalWrite(ledRed, HIGH);
     delay(300);
      digitalWrite(ledGreen, LOW);
     digitalWrite(ledBlue, LOW);
     digitalWrite(ledYellow, LOW);
     digitalWrite(ledRed, LOW);
     delay(300);
       digitalWrite(ledGreen, HIGH);
     digitalWrite(ledBlue, HIGH);
     digitalWrite(ledYellow, HIGH);
     digitalWrite(ledRed, HIGH);
     delay(300);
      digitalWrite(ledGreen, LOW);
     digitalWrite(ledBlue, LOW);
     digitalWrite(ledYellow, LOW);
     digitalWrite(ledRed, LOW);
     delay(300);
       digitalWrite(ledGreen, HIGH);
     digitalWrite(ledBlue, HIGH);
     digitalWrite(ledYellow, HIGH);
     digitalWrite(ledRed, HIGH);
     delay(300);
      digitalWrite(ledGreen, LOW);
     digitalWrite(ledBlue, LOW);
     digitalWrite(ledYellow, LOW);
     digitalWrite(ledRed, LOW);
  }

  
  /*
  if (strcmp(msg, "on") == 0) {
    digitalWrite(ledStatus, HIGH);
  }
  else if (strcmp(msg, "off") == 0) {
    digitalWrite(ledStatus, LOW);
  }
  else if (strcmp(msg, "onRed") == 0) {
    digitalWrite(ledRed, HIGH);
  }
  else if (strcmp(msg, "offRed") == 0) {
    digitalWrite(ledRed, LOW);
  }
  else if (strcmp(msg, "onYellow") == 0) {
    digitalWrite(ledYellow, HIGH);
  }
  else if (strcmp(msg, "offYellow") == 0) {
    digitalWrite(ledYellow, LOW);
  }
  else if (strcmp(msg, "onBlue") == 0) {
    digitalWrite(ledBlue, HIGH);
  }
  else if (strcmp(msg, "offBlue") == 0) {
    digitalWrite(ledBlue, LOW);
  }
  else if (strcmp(msg, "onGreen") == 0) {
    digitalWrite(ledGreen, HIGH);
  }
  else if (strcmp(msg, "offGreen") == 0) {
    digitalWrite(ledGreen, LOW);
  }
  */
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

void button() {

  if (buttonStateR != digitalRead(buttonRed) || buttonStateY != digitalRead(buttonYellow) || buttonStateB != digitalRead(buttonBlue) || buttonStateG != digitalRead(buttonGreen)) {
    //Butten ist nicht gedrückt. Die Zeit wird berechnet
    if (digitalRead(buttonRed) != HIGH) {

      buttonStateR = digitalRead(buttonRed);
      doc["id"] = "red";
      doc["action"] = "pressed";

      buttonStateR = LOW;
      Serial.println(buttonRed);
      digitalWrite(ledRed, HIGH);

      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      client.publish(TOPIC, message);

      delay(100); // Verzögerung um den Button zu entprellen
    } else if (digitalRead(buttonYellow) != HIGH) {

      buttonStateY = digitalRead(buttonYellow);
      doc["id"] = "yellow";
      doc["action"] = "pressed";

      buttonStateY = LOW;
      Serial.println(buttonYellow);
      digitalWrite(ledYellow, HIGH);

      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      client.publish(TOPIC, message);

      delay(100); // Verzögerung um den Button zu entprellen

    } else if (digitalRead(buttonBlue) != HIGH) {

      buttonStateB = digitalRead(buttonBlue);
      doc["id"] = "blue";
      doc["action"] = "pressed";

      buttonStateB = LOW;
      Serial.println(buttonBlue);
      digitalWrite(ledBlue, HIGH);

      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      client.publish(TOPIC, message);
      delay(100); // Verzögerung um den Button zu entprellen

    } else if (digitalRead(buttonGreen) != HIGH) {

      buttonStateG = digitalRead(buttonGreen);
      doc["id"] = "green";
      doc["action"] = "pressed";

      buttonStateG = LOW;
      Serial.println(buttonGreen);
      digitalWrite(ledGreen, HIGH);

      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      client.publish(TOPIC, message);

      delay(100); // Verzögerung um den Button zu entprellen
    } else {
      buttonStateR = HIGH;
      buttonStateY = HIGH;
      buttonStateB = HIGH;
      buttonStateG = HIGH;

      // Elemente werden an ein Json übergeben das später gesendet wird
      /*
      doc["id"] = "off";
      doc["action"] = "release";
      // Nachricht wird gepackt und an MQTT gesendet.
      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      
      client.publish(TOPIC, message);
      */
      delay(100); // Verzögerung um den Button zu entprellen
      digitalWrite(ledRed, LOW);
      digitalWrite(ledYellow, LOW);
      digitalWrite(ledBlue, LOW);
      digitalWrite(ledGreen, LOW);

    }
  }
}

// ToDO: Die Buttonsteuerung in eine eigene Funktion packen.
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Überprüfung ob der Zustand des Buttons sich geändert hat
  button();
}
