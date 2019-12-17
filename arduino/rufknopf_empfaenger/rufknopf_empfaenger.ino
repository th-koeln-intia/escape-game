// Dieses Skript sendet den State eines Buttons mit dauer wie lange dieser gedrückt wurde.
// Zusätzlich gibt es eine LED die ebenfalls über MQTT gesteuert werden kann

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

const char* SSID = "intia";
const char* PSK = "BuntesLicht10";
const char* MQTT_BROKER = "intia.local";
const char* TOPIC = "rufempfaenger"; // Toppic an das gesendet wird

const char* TOPIC_SUB = "rufempfaenger/set"; // Topic das Oboniert wird
//Wird an dieses Topic "on" gesendet so geht die LED an. "off" zum aus schalten.

WiFiClient espClient;
PubSubClient client(espClient);

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;

int waitForAck=0;

int led1 = D2; // orange LED - Hausreinigung
int led2 = D3; // red LED - Waescherei
int led3 = D4; // green LED - Kueche
int button = D5; 

int empfangKnopf = 0;
bool hausreinigungRuf = false;
bool kuechenRuf = false;
bool waeschereiRuf = false;
int counter = 0;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  // initialize the digital pin as an output.
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.println("Initialise...");

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

  kuechenRuf = true;
  waeschereiRuf = false;
  // hausreinigungRuf = true;

  msg[length] = '\0';
  Serial.println(msg);
  Serial.println("Jemand ruft aus Küche!");
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
    if (!client.connect("Rufknopf_Empfaenger")) {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
  client.subscribe(TOPIC_SUB);
  Serial.println("MQTT Connected...");
}

// the loop routine runs over and over again forever:
void loop() {

    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  
    if(digitalRead(button) == LOW) {
        empfangKnopf = 1;
        Serial.println("Anleiterin hat Ruf bestätigt und ist auf dem Weg.");
        Serial.println(digitalRead(button));
    }

  // Anleiterin wird von der Kueche gerufen
  if (hausreinigungRuf == true && empfangKnopf == 3) {
    // Anleiterin bestaetigt Ruf-Empfang Kueche
    digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
  } else if (hausreinigungRuf == true) {
    digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
    // Serial.println("Von Küche wird gerufen!!");
  }

  // Anleiterin wird von der Waescherei gerufen
  if (waeschereiRuf == true && empfangKnopf == 2) {
    // Anleiterin bestaetigt Ruf-Empfang Waescherei
    digitalWrite(led2, HIGH);
  } else if (waeschereiRuf == true) {
    digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(led2, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  }

  // Anleiterin wird von der Küche gerufen
  if (kuechenRuf == true && empfangKnopf == 1) {

    Serial.println("Call confirmed");
    // Anleiterin bestaetigt Ruf-Empfang Hausreinigung
    digitalWrite(led3, LOW);
    
    doc["location"] = "kitchenrufempfangenundbestaetigt";
    // Nachricht wird gepackt und an MQTT gesendet.
    char message[256];
    serializeJson(doc, message);
    Serial.println(message);
          
    client.publish(TOPIC, message);
    kuechenRuf = false;
    delay(100);
    
  } else if (kuechenRuf == true) {
    digitalWrite(led3, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(led3, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  } 
//  else if (kuechenRuf == false) {
//    digitalWrite(led3, LOW);    // turn the LED off by making the voltage LOW
//    delay(100);
//  }
}
