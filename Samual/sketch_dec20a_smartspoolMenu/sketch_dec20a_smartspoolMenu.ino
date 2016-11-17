#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>

SoftwareSerial Master(11, 12); //RX,TX
LiquidCrystal lcd(8,9,4,5,6,7);

//Menu States:
int currentMenuItem = 0;
int lastState = 0;
byte count = 0;
long first = 0;
int time = 20;

void setup() {
  Serial.begin(9600);
  Master.begin(9600);
  lcd.begin(16, 2);
  clearPrintTitle();
}

void loop() {
  switchPrinter();
  mainMenu();
  }

void mainMenu() {
  int state = 0;
  int x = analogRead(0);
  lcd.setCursor(0,0);
  //LCD keypad shield analog values:
  if (x < 100) {//Right
    state = 1;
  } else if (x < 200) {//Up
    state = 2;
  } else if (x < 400) {//Down
    state = 3;
  } else if (x < 600) {//Left
    state = 4;
  } else if (x < 800) {//Select
    state = 5;
  } else if (x < 1000) {//Reset
    state = 6;
  }
  //Out of bounds? Reset:
  if (currentMenuItem < 0 || currentMenuItem > 7) {
    currentMenuItem = 0;
  }
  //State responce:
  if (state != lastState) {
    if (state == 1) {//If Right
      clearPrintTitle();
      time = time + 10;
      lcd.print (time);
      lcd.print (" sec");
    } else if (state == 4) {//If Left
      clearPrintTitle();
      time = time - 10;
      lcd.print (time);
      lcd.print (" sec");
    } else if (state == 2) {//If Up
      currentMenuItem = currentMenuItem - 1;
      displayMenu(currentMenuItem);
    } else if (state == 3) {//If Down
      currentMenuItem = currentMenuItem + 1;
      displayMenu(currentMenuItem);
    } else if (state == 5) {//Select
      selectMenu(currentMenuItem);
    } else if (state == 6) {//Reset
      currentMenuItem = 0;
    }
    //save last state to compare
    lastState = state;
  }
  delay(5);
}

void switchPrinter(){
  Master.print(time);
  Serial.print(time);
}

void displayMenu(int x) {
     switch (x) {
      case 1:
        clearPrintTitle();
        lcd.print ("=> TIME");
        break;
      case 2:
        clearPrintTitle();
        lcd.print ("-> WEIGHT");
        break;
       case 3:
        clearPrintTitle();
        lcd.print ("=> TEMP");
        break;
      case 4:
        clearPrintTitle();
        lcd.print ("-> HUMIDITY");
        break;
      case 5:
        clearPrintTitle();
        lcd.print ("=> DIAMETER");
        break;
      case 6:
        clearPrintTitle();
        lcd.print ("-> DEHUMID ON/OFF");
        break;
      case 7:
        clearPrintTitle();
        lcd.print ("=> AUDIO ON/OFF");
        break;
    }
}

void clearPrintTitle() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" SmartspooL ");
  lcd.setCursor(0,1); 
}

void selectMenu(int x) {
   switch (x) {
      case 1:
        //Incremented time:
        clearPrintTitle();
        lcd.print ("time = ");
        lcd.print (time);
        lcd.print (" sec"); 
        break;
      case 2:
        clearPrintTitle();
        lcd.print ("Selected: WEIGHT");
        break;
       case 3:
        clearPrintTitle();
        lcd.print ("Selected: TEMP");
        break;
      case 4:
        clearPrintTitle();
        lcd.print ("Selected: HUMIDITY");
        break;
      case 5:
        clearPrintTitle();
        lcd.print ("Selected: DIAMETER");
        break;
      case 6:
        clearPrintTitle();
        lcd.print ("Selected: DEHUMID ON/OFF");
        break;
      case 7:
        clearPrintTitle();
        lcd.print ("Selected: AUDIO ON/OFF");
        break;
    }
    
}
