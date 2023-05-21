#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#define Password_Lenght 5

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; 
byte colPins[COLS] = {A1, A2, A3, A4}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo servo;

char Data[Password_Lenght]; 
char Data2[Password_Lenght];
char Master[Password_Lenght];
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char key;
byte key_state = 0;
char last_press_key;
byte mode = 0;

int change_password_allow_time = 10000;
long time_old = 0;
bool just_allowed_pass =0;

char FistTimePassword[] = {'1','2','3','4'};

void setup() {
  //
}
  
void loop() {
  //
}
  