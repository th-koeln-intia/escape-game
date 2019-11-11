// Dieses Skript sendet den State eines Buttons mit dauer wie lange dieser gedrückt wurde.
// Zusätzlich gibt es eine LED die ebenfalls über MQTT gesteuert werden kann

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

const char* SSID = "intia";
const char* PSK = "BuntesLicht10";
const char* MQTT_BROKER = "192.168.43.184";
const char* TOPIC = "morse"; // Toppic an das gesendet wird

const char* TOPIC_SUB = "morse/set"; // Topic das Oboniert wird
//Wird an dieses Topic "on" gesendet so geht die LED an. "off" zum aus schalten.

WiFiClient espClient;
PubSubClient client(espClient);

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;


const int ledStatus =  D8;      // LED-Pin

const int ledButton =  D2;      // LED-Pin
const int button = D5;          // Button-Pinconst
int buttonState = 1;            // Status des Buttons

// Variabeln um die Zeit zu berechnen
unsigned long startTime;
unsigned long pressTime;

void setup() {
  pinMode(ledStatus, OUTPUT);
  pinMode(ledButton, OUTPUT);
  pinMode(button, INPUT_PULLUP);
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
  
  resetLeds();
  
  msg[length] = '\0';
  char *token=strtok(msg," ");
   while( token != 0 ) {
      printf( "Next color to show: %s\n", token );
      if(strcmp(token, "green") == 0) {
        showColor(ledButton);
      }else if (strcmp(token, "blue") == 0) {
          showColor(ledButton);
      }
      token = strtok(0, " ");
   }
}
void resetLeds(){
  //Alle leds ausschalten uns kurz warten
  digitalWrite(ledButton, LOW);
}
void showColor(int buttonColor){
  delay(500);
  digitalWrite(buttonColor, HIGH);
  delay(1000);
  digitalWrite(buttonColor, LOW);
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

void checkButton() {
  if (buttonState != digitalRead(button)) {
    //Butten ist nicht gedrückt. Die Zeit wird berechnet
    if (digitalRead(button) != HIGH) {

      buttonState = digitalRead(button);
      doc["id"] = "button";
      doc["action"] = "pressed";

      buttonState = LOW;
      Serial.println(button);
      digitalWrite(ledButton, HIGH);

      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      client.publish(TOPIC, message);

      delay(100); // Verzögerung um den Button zu entprellen
    }else {
      buttonState = HIGH;

      // Elemente werden an ein Json übergeben das später gesendet wird
      
      doc["id"] = "off";
      doc["action"] = "release";
      // Nachricht wird gepackt und an MQTT gesendet.
      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      
      client.publish(TOPIC, message);
      
      delay(100); // Verzögerung um den Button zu entprellen
      digitalWrite(ledButton, LOW);
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
  checkButton();
}
