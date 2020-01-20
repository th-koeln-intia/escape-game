// Dieses Skript sendet den State eines Buttons mit dauer wie lange dieser gedrückt wurde.
// Zusätzlich gibt es eine LED die ebenfalls über MQTT gesteuert werden kann

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

const char* SSID = "intia";
const char* PSK = "BuntesLicht10";
const char* MQTT_BROKER = "openhabianpi.local";
const char* TOPIC = "/button"; // Toppic an das gesendet wird

const char* TOPIC_SUB = "/button/set"; // Topic das Oboniert wird
//Wird an dieses Topic "on" gesendet so geht die LED an. "off" zum aus schalten.

WiFiClient espClient;
PubSubClient client(espClient);

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;

const int buttonPin = D3;     // Button-Pin
const int ledPin =  D2;      // LED-Pin

int buttonState = 1;         // Status des Buttons

// Variabeln um die Zeit zu berechnen
unsigned long startTime;
unsigned long pressTime;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
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

  if (strcmp(msg, "on") == 0) {
    digitalWrite(ledPin, HIGH);
  }
  else if (strcmp(msg, "off") == 0) {
    digitalWrite(ledPin, LOW);
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

// ToDO: Die Buttonsteuerung in eine eigene Funktion packen.
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  // Überprüfung ob der Zustand des Buttons sich geändert hat
  if (buttonState != digitalRead(buttonPin)) {

    //Butten ist nicht gedrückt. Die Zeit wird berechnet
    if (digitalRead(buttonPin) == HIGH) {
      pressTime = millis() - startTime;
      buttonState = digitalRead(buttonPin);
      digitalWrite(ledPin, LOW);

      // Elemente werden an ein Json übergeben das später gesendet wird
      doc["button"] = "release";
      doc["duration_ms"] = pressTime;
      doc["duration_s"] = int(pressTime / 1000);

      Serial.println(buttonState);
      Serial.print("Time: ");
      Serial.print(pressTime);
      Serial.print(" milliseconds ");
      Serial.print(int(pressTime / 1000));
      Serial.println(" seconds");

      startTime = millis(); // Timer wird "zurückgesetzt"

      // Nachricht wird gepackt und an MQTT gesendet.
      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      client.publish(TOPIC, message);

      delay(100); // Verzögerung um den Button zu entprellen

    } else if (digitalRead(buttonPin) == LOW) {

      startTime = millis(); // Setzt den Presstimmer zurück

      buttonState = digitalRead(buttonPin);
      // ToDo: in der Json stehen immer noch die alten Zeiten beim Drücken des Knopfes. Gehört nur in den Release-State
      doc["button"] = "release";

      // LED aus
      buttonState = LOW;
      Serial.println(buttonState);
      digitalWrite(ledPin, HIGH);

      char message[256];
      serializeJson(doc, message);
      Serial.println(message);
      client.publish(TOPIC, message);

      delay(100); // Verzögerung um den Button zu entprellen
    }
  }
}
