#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

#define passwordLimit 5

// lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// keypad
const byte rows = 4; 
const byte cols = 4; 
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[rows] = {6, 7, 8, 9}; 
byte colPins[cols] = {A1, A2, A3, A4}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

Servo servo;

char currentData[passwordLimit];
char tempData[passwordLimit];
char master[passwordLimit];
char firstTimePassword[] = {'1','2','3','4'};
char key;
char lastPressedKey;
byte inputCount = 0;
byte keyState = 0;
byte mode = 0; // 0: Enter Password (LOCKED), 1: Set New Password, 2: Password Again, 3: UNLOCKED
long duration = 0;

void setup() {
  Serial.begin(9600);
  checkEEPROM();
  servo.attach(13);
  lcd.begin(16, 2);
  servo.write(0);  
}
  
void loop() {
  key = keypad.getKey(); 
  keyState = keypad.getState();

  // saving the last pressed key
  if (key) {
    lastPressedKey = key;
    Serial.println(key);
  }

  if (mode == 0) {
    lcd.setCursor(1,0);
    lcd.print("Enter Password");
  }
  else if (mode == 1) {
    lcd.setCursor(0,0);
    lcd.print("Set New Password");    
  }
  else if (mode == 2) {
    lcd.setCursor(0,0);
    lcd.print("Password Again");
  }
  else if (mode == 3) {
    lcd.setCursor(4,0);
    lcd.print("UNLOCKED");         
  }

  // in UNLOCKED mode to change password or lock the safe
  if (mode == 3) {
      if (lastPressedKey == '#' && keyState == 2) {
        mode = 1; // Set New Password
      }
      if (lastPressedKey == '*' && keyState == 2) {
        mode = 0; // Enter Password (LOCKED)
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("LOCKED"); 
        servo.write(0);
        delay(2000);          
      }
  }

  // '*' and '#' characters are not allowed in password  
  if ((key && key != '*' && key != '#') && mode != 3) {
    collectKey();
  }

  // after 4 pressed character, check the mode
  if (inputCount == passwordLimit-1) {
    if (mode == 0) {
      lcd.clear();
      if (!strcmp(currentData, master)) {
        lcd.setCursor(2, 0);
        lcd.print("WELCOME BACK");
        duration = millis();
        servo.write(90);
        delay(2000);
        mode = 3;
      }
      else {
        lcd.setCursor(4, 0);
        lcd.print("INCORRECT");
        lcd.setCursor(4, 1);
        lcd.print("PASSWORD!");
        delay(2000);
      }
      delay(1000);// 
      lcd.clear();
      clearCurrentData(); 
    }
    else if (mode == 1) {
      lcd.clear();
      mode = 2;
      for (int i = 0; i < passwordLimit; i++) {
        tempData[i] = currentData[i];
      }
      clearCurrentData(); 
    }
    else if (mode == 2) {
      if (!strcmp(currentData, tempData)) {
        lcd.clear();
        lcd.setCursor(0, 0); 
        lcd.print("New Password is ");  
        lcd.setCursor(4, 1);   
        lcd.print(currentData);  
        delay(2000);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Saving...");
        for (int i = 0; i <= 100; i = i+10) {
          lcd.setCursor(4, 1);
          lcd.print(i);
          lcd.setCursor(7, 1);
          lcd.print("%");
          delay(200);
        }
        EEPROM.put(0, currentData);
        EEPROM.get(0, master);
        delay(500);
      }
      else {
        lcd.clear();
        lcd.setCursor(4, 0); 
        lcd.print("PASSWORD");  
        lcd.setCursor(3, 1);   
        lcd.print("NOT MATCH!");  
        delay(2000);        
      }
      mode = 3;
      clearCurrentData();
      lcd.clear();
    }
  }
}

void collectKey() {
  currentData[inputCount] = key;
  lcd.setCursor(4+inputCount, 1); 
  lcd.print("*"); 
  inputCount++;
}

void clearCurrentData() {
  while (inputCount != 0) {  
    currentData[inputCount--] = 0; 
  }
}

void checkEEPROM() {
  EEPROM.get(0, master);
  if (master[0] == 0 && master[1] == 0 && master[2] == 0 && master[3] == 0) {
    Serial.println("EEPROM is in default settings");
    EEPROM.put(0, firstTimePassword);
    EEPROM.get(0, master);
  }
}