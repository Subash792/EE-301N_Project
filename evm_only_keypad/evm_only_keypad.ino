 // C++ code
//
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>

static const uint8_t analog_pins[] = {A0,A1,A2};

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,4,3,2}; //connect to the column pinouts of the keypad
//Create an object of keypad
Keypad keyp = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// O_RDWR

const int rs = 12, en = 11, d4 = 13, d5 = 10, d6 = 1, d7 = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int verified = -1;



struct voter{
  int voter_id;
  int password;
  bool b;
};

struct candidate{
  int candidate_number;
  int votes;
};


void initialise_memory(){
  int address = 0;
  for(int i=1;i<=10;i++){
    voter v;
    v.voter_id = (i*100)+7;
    v.password = (i*100)+9;
    v.b = 0;
    EEPROM.put(address,v);
    address+=sizeof(voter);
  }
  for(int i=1;i<=5;i++){
    candidate c;
    c.candidate_number = i;
    c.votes = 0;
    EEPROM.put(address,c);
    address+=sizeof(candidate);
  }
}


// lcd interface functions
void setup_lcd(int voter_id){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Voter Verified");
  delay(1000);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Welcome");
  lcd.setCursor(0,1);
  lcd.print("voter_id: "+ String(voter_id));
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Party no:");
  delay(1000);
}


// sd card and fingerprint interface functions

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



int check_identity(int username, int password){
    int address = 0;
    for(int i=1;i<=10;i++){
        voter v;
        EEPROM.get(address,v);
        if(v.voter_id == username && v.password == password && v.b ==0){
          v.b = 1;
          EEPROM.put(address,v);
          return 1;                                                                       
        }
        address+=sizeof(voter);
    }
    return -1;
}


void update_memory(int candidate_number){
  int address = 0;
  for(int i=0;i<10;i++){
    address+= sizeof(voter);
  }
  for(int i=0;i<5;i++){
    candidate c;
    EEPROM.get(address,c);
    if(c.candidate_number == candidate_number){
      c.votes+=1;
      EEPROM.put(address,c);
    }
    address+=sizeof(candidate);  
  
}

}

void reset(){
  int address = 0;
  for(int i=0;i<10;i++){
    voter v;
    EEPROM.get(address,v);
    v.b = 0;
    EEPROM.put(address,v);
    address+= sizeof(voter);
  }
  for(int i=0;i<5;i++){
    candidate c;
    EEPROM.get(address,c);
    c.votes = 0;
    EEPROM.put(address,c);
    address+=sizeof(candidate);  
  }  
}

void stop1(){
  while(true){}
  }


// ballot interface functions

int fetch(){
   int pin;
   int num = 0;
   while(num!=1){
   num = 0;
   for(int i=0;i<=2;i++){
     delay(100);
     int c = digitalRead(analog_pins[i]);
     if(c==1){
        num+=1;
        pin = i+1;
     }    
   }
   }
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Hold for: ");
     for(int i=0;i<10;i++){
        lcd.setCursor(11,0);
        lcd.print(String(10-i-1));
        delay(100);
       if(digitalRead(analog_pins[pin-1]) == LOW){
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Hold for 10s");
         delay(1000);
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Press again");
         delay(1000);
         pin = 0;
         break;
       }
       delay(1000);
       
     }
     return pin;  
}

// tally function
void count_votes() {
  int address = 0;
  for(int i=0;i<10;i++){
    address+= sizeof(voter);
  }
  for(int i=0;i<5;i++){
    candidate c;
    EEPROM.get(address,c);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Candidate No:"+String(c.candidate_number));
    lcd.setCursor(0,1);
    lcd.print("No of Votes: "+String(c.votes));
    delay(2000);
    address+= sizeof(candidate);
    
    }
}



void setup() {

pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);

lcd.begin(16, 2);
initialise_memory();

}


void loop() {
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Voter ID:");
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
  
  if(username == 111 && password == 999){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter 1 to stop");
    lcd.setCursor(0,1);
    lcd.print("Enter 2 to tally");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Enter 3 to reset");
    delay(2000);
    String input = "";
    while(input!= "1" && input!="2" && input != "3"){
      lcd.clear();
      lcd.print("Enter:");
      input = get_input();
    }
    if(input == "1"){
      lcd.clear();
      lcd.print("Voting Stoped");
      delay(2000);
      stop1();
    }else if(input == "2")
      count_votes();
      else if(input == "3"){
      reset();
      lcd.clear();
      lcd.print("EVM Reset");
      delay(2000);
      }
      
    
  }
  else{
  verified = check_identity(username, password);

    if(verified == -1){
      lcd.clear();
      lcd.print("INVALID VOTER");
      delay(1000);
    }

    if(verified!=-1){
      setup_lcd(username);
      delay(1000);
      int candidate_number = 0;
        while(true){
        candidate_number = (get_input()).toInt();
        if(candidate_number>=1 && candidate_number<=5)
          break;
        lcd.clear();
        lcd.print("Enter Valid no:");
        }   
      update_memory(candidate_number);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("You chose");
      lcd.setCursor(0,1);
      lcd.print("Candidate no "+String(candidate_number));
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Vote Casted");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Thank you");
      delay(2000);

    }
  } 
  
}
