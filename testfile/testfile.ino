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
byte rowPins[ROWS] = {0,1,6,7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8,9,10,13}; //connect to the column pinouts of the keypad
//Create an object of keypad
Keypad keyp = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// O_RDWR

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int verified = -1;



struct voter{
  int finger;
  int voter_id;
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
    v.finger = i;
    v.voter_id = (i*100)+7;
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
  lcd.print("Select Candidate");
  delay(1000);
}





// sd card and fingerprint interface functions

int get_fingerprint(){
   char key = keyp.getKey();
   switch(key){
      case '1':
        return 1;
      case '2':
        return 2;
      case '3':
        return 3;
      case '4':
        return 4;
      case '5':
        return 5;
      case '6':
        return 6;
      case '7':
        return 7;
      case '8':
        return 8;
      case '9':
        return 9;
      case '#':
        return 20;
      case '*':
        return 30;
      default:
        return 0;
   } 
  }


int check_identity(int finger_print){
    int address = 0;
    for(int i=1;i<=10;i++){
        voter v;
        EEPROM.get(address,v);
        if(v.finger == finger_print && v.b ==0){
          v.b = 1;
          EEPROM.put(address,v);
          return v.voter_id;                                                                       
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
    delay(1500);
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

  int fingerprint_identity = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Press and Hold");
    lcd.setCursor(1,1);
    lcd.print("your finger");
  
  while(fingerprint_identity==0){
    fingerprint_identity = get_fingerprint();  
  }

  lcd.clear();
  lcd.print("Lift Finger");
  delay(1000);
  
  if(fingerprint_identity == 20){
    count_votes();
  }
  else{
  verified = check_identity(fingerprint_identity);

if(verified == -1){
  lcd.clear();
  lcd.print("INVALID VOTER");
  delay(1000);
}


if(verified!=-1){
  setup_lcd(verified);
  delay(1000);
  int candidate_number = 0;
    while(true){
    candidate_number = fetch();
      delay(100);
    if(candidate_number!=0){
      break;
    } 
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
