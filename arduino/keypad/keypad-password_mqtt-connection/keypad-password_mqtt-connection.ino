//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP8266WiFi.h>
//#include <PubSubClient.h>
//#include <string.h>
//#include <ArduinoJson.h>

#define Password_Length 5

int signalPin = 12, i = 0;

char data[Password_Length];
char pass[Password_Length] = "1111";
char master[Password_Length] = "####";
byte data_count = 0;
bool pass_is_correct = false;
char customKey;
char newKey;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {D3, D2, D1, D0};
byte colPins[COLS] = {D7, D6, D5, D4};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// LiquidCrystal_I2C lcd(0x21, 16, 2);

// Configuration of WLAN
const char* SSID = "intia2";
const char* PSK = "BuntesLicht10";

WiFiClient espClient;

//Configuration for MQTT
const char* MQTT_BROKER = "192.168.43.245";
const char* TOPIC = "keypad"; // Publish topic
const char* TOPIC_SUB = "keypad/set"; // Subscribe topic

const int capacity = JSON_OBJECT_SIZE(50);
StaticJsonDocument<capacity> doc;
int waitForACK = 0;

void setup() {
  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
  //lcd.init();
  //lcd.backlight();
  Serial.begin(115200);
  pinMode(signalPin, OUTPUT);
  Serial.println("Starting Keypad...");
  pinMode(LED_BUILTIN, OUTPUT); // For validating connection with broker
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(waitForACK) {
// if connection is still waiting to be acknowledged
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(200);                       // wait for a second
  }
    // if connection was acknowledged
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                       // wait for a second

  //lcd.setCursor(0, 0);
  //lcd.print("Enter password:");
  //Serial.print("Enter password: ");

  customKey = customKeypad.getKey();
  if (customKey) {
    data[data_count] = customKey;
    //lcd.setCursor(data_count, 1);
    //lcd.print(data[data_count]);
    Serial.print(data[data_count]);
    data_count++;
  }
  if (data_count == Password_Length - 1) {
    //lcd.clear();

    if (!strcmp(data, pass))
    {
      //lcd.print(“Correct”);
      Serial.println();
      Serial.println("Correct password");
      pass_is_correct = true;
      sendPasswordVerificationToMQTTBroker(pass_is_correct);
      
      digitalWrite(signalPin, HIGH);
      delay(2000);
      digitalWrite(signalPin, LOW);
    }
    else
    {
      if (!strcmp(data, master))
      {
        //lcd.print(“Incorrect”);
        Serial.println();
        Serial.println("Please input the new password: ");
        for (i = 0; i < 4; i++)
        {
          newKey = customKeypad.waitForKey();
          if (newKey)
          {
            pass[i] = newKey;
            Serial.print(pass[i]);
          }
          //lcd.setCursor(data_count,1);
          //lcd.print(data[data_count]);
        }
        Serial.println();
        Serial.print("New password is: "); 
        Serial.println(pass);
      }
      else
      {
        Serial.println();
        Serial.println("Wrong password");
      }
      delay(1000);
    }
    //lcd.clear();
    cleardata();
  }
}

void cleardata() {
  while (data_count != 0) {
    data[data_count--] = 0;
  }
  return;
}

void sendPasswordVerificationToMQTTBroker(bool isPassCorrect) {
  doc["pass_is_correct"] = isPassCorrect;
  
  char message[256];
  serializeJson(doc, message);
  Serial.println(message);
  client.publish(TOPIC, message);
  waitForACK = 1;
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
