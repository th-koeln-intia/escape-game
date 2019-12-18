// Dieses Skript sendet den State eines Buttons mit dauer wie lange dieser gedrückt wurde.
// Zusätzlich gibt es eine LED die ebenfalls über MQTT gesteuert werden kann

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

const char* SSID = "intia";
const char* PSK = "BuntesLicht10";
const char* MQTT_BROKER = "intia.local";
const char* TOPIC = "kitchen"; // Topic an das gesendet wird

const char* TOPIC_SUB = "kitchen/set"; // Topic das Oboniert wird
//Wird an dieses Topic "on" gesendet so geht die LED an. "off" zum aus schalten.

WiFiClient espClient;
PubSubClient client(espClient);

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;

const int buttonPin = D5;     // Button-Pin
const int ledPin =  D2;      // LED-Pin

int buttonState = 1;         // Status des Buttons
int waitForAck=0;
bool resetButton = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
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
  Serial.println();

  msg[length] = '\0';
  Serial.println(msg);
  digitalWrite(ledPin, HIGH);
  buttonState = HIGH;
  waitForAck=1;
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
    if (!client.connect("KitchenRufknopf")) {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
  client.subscribe(TOPIC_SUB);
  Serial.println("MQTT Connected...");
}

// ToDO: Die Buttonsteuerung in eine eigene Funktion packen.
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (buttonState != digitalRead(buttonPin)) {
    //Butten ist nicht gedrückt. Die Zeit wird berechnet
    if (digitalRead(buttonPin) != HIGH) {
      if(waitForAck == 0){
         Serial.println("Request help");
          buttonState = LOW;

        doc["location"] = "kitchen";
        // Nachricht wird gepackt und an MQTT gesendet.
        char message[256];
        serializeJson(doc, message);
        Serial.println(message);
          
        client.publish(TOPIC, message);
        Serial.println("Anleiterin ist auf dem Weg.");
          
        delay(200); // Verzögerung um den Button zu entprellen
      }else{
        Serial.println("Reset Button");
        waitForAck = 1;
        digitalWrite(ledPin,LOW);
        buttonState = HIGH;
      }
    }
  }
  if(buttonState==LOW && resetButton==false){
     for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 20 milliseconds to see the dimming effect    
    delay(20);                            
    } 
    delay (300);
    // fade out from not-quite-max to min in increments of 5 points:
    for(int fadeValue = 200 ; fadeValue >= 0; fadeValue -=5) { 
      // sets the value (range from 0 to 200):
      analogWrite(ledPin, fadeValue);         
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);                            
    } 
  }
}
