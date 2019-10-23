//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define Password_Length 5

int signalPin = 12, i = 0;

char data[Password_Length];
char pass[Password_Length] = "1111";
char master[Password_Length] = "####";
byte data_count = 0;
bool pass_is_good;
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

void setup() {
  //lcd.init();
  //lcd.backlight();
  Serial.begin(115200);
  pinMode(signalPin, OUTPUT);
}

void loop() {

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
