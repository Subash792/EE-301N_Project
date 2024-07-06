#include <Keypad.h>
#include <LiquidCrystal.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad keyp = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//
const int rs = 12, en = 11, d4 = 13, d5 = 10, d6 = 1, d7 = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


String get_input(){
  String input = "";
  lcd.setCursor(0,1);
  int l = 0;
  while(true){
    char key = keyp.getKey();
    if(key == 'A')
      break;
    else if(key == 'B'){
      input.remove(input.length()-1);
      l--;
      lcd.setCursor(l,1);
      lcd.print(" ");
      lcd.setCursor(l,1);
    }
    else if(key){
      input+=key;
      lcd.print(key);
      l++;
    }
  }
  return input;
}



void setup(){
  lcd.begin(16,2);
  pinMode(4,INPUT);
  
}


void loop(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Username:");
  int username = (get_input()).toInt();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Password:");
  int password = (get_input()).toInt();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Entering...");
  delay(1000);
  lcd.clear();
  if(username == 107 && password == 171){
    lcd.print("Verified");
     delay(2000); 
  }else{
    lcd.print("Invalid");
    delay(2000);
  }
}
