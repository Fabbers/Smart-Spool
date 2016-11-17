#include <LiquidCrystal_I2C.h>
#include <Wire.h>  
#include "DHT.h"
#include "pitches.h"

#define DHTTYPE DHT11
#define size_char 8
#define DHTPIN    2
#define buttonPin 3
#define RLPIN     4
#define led       13

//====================================================================================
byte a11[size_char] = {
  B00000,B00000,B00001,B00011,B00111,B00111,B01111,B01111,                        };
byte a21[size_char] = {
  B00000,B00000,B10000,B11000,B11100,B11100,B11110,B11110,                        };
byte a12[size_char] = {
  B01111,B01111,B01111,B01111,B00111,B00111,B00111,B00111,                        };
byte a22[size_char] = {
  B11110,B11110,B11110,B11110,B11100,B11100,B11100,B11100,                        };
byte a13[size_char] = {
  B00011,B00011,B00011,B00011,B00001,B00001,B00001,B00001,                        };
byte a23[size_char] = {
  B11000,B11000,B11000,B11000,B10000,B10000,B10000,B10000,                        };
byte a14[size_char] = {
  B00000,B00000,B00001,B00011,B00011,B00001,B00000,B00000,                        };
byte a24[size_char] = {
  B00000,B00000,100000,B11000,B11000,B10000,B00000,B00000,                        };
//====================================================================================

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int noteDurations[] = { 
  4,8,8,4,4,4,4,4 };
const int analogInPin = A0; //<<<
const int analogInPin1 = A1;
int sensorValue1 = 0;
int sensorValue = 0;
int arr_M[2] = {
  0};
int arr_H[2] = {
  0};
long micros1  = 0;
int rpm = 0;
int wgh = 351;
boolean buttonState = LOW;
boolean lastButtonState = LOW;
int buttonPushCounter = 0;
int c = 1;
int thisNote = 0;
long interval = 1000;
long previousMillis = 0;
boolean flag = LOW;
boolean start_alhoritm = 0;
int h,t;

DHT dht(DHTPIN, DHTTYPE);

void setup()  
{
  Serial.begin(115200);  
  lcd.begin(20,4);  
  dht.begin();  
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); 
  lcd.setCursor(1, 1);
  lcd.write("SMART SPOOL READY");
  lcd.setCursor(4, 2);
  lcd.write("(PUSH START)");
  pinMode(led, OUTPUT);
  pinMode(RLPIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(led, LOW);    
  delay(1000);
  digitalWrite(led, HIGH);   
  delay(1000);               
  digitalWrite(led, LOW);    
  delay(500);
  for (thisNote = 0; thisNote < 8; thisNote++) 
  {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(8, melody[thisNote],noteDuration); //<<<
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
  while (1)
  {
    if(digitalRead(buttonPin) == 1)
    {
      break;
    }
  }
  lcd.setCursor(2, 1);
  lcd.write("                  ");
  lcd.setCursor(2, 2);
  lcd.write("                  ");
  digitalWrite(RLPIN,LOW);
}


void loop()   
{
  sensorValue = analogRead(analogInPin);
  interval = map(sensorValue, 0, 1023, 5000, 200000);
  unsigned long currentMillis = millis();
  disp();
  RPM();
  h = dht.readHumidity();
  /*arr_H[0] = h;
  if(abs(arr_H[0]-arr_H[1])>5 && arr_H[0] != 0 && arr_H[1] != 0)
  {
    while (abs(arr_H[0]-arr_H[1])<5)
    {
    
    }
  
  }
  arr_H[1] = arr_H[0];*/
  t = dht.readTemperature();
  if (h>36)
  {
    digitalWrite(RLPIN,HIGH);
  }
  else
  {
    digitalWrite(RLPIN,LOW);
  }
  if(currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;   
    if (rpm<1) 
    {
      //digitalWrite(but_led, HIGH);
      Serial.println("M25");
      picture();
      music();
    }
  } 
}
void RPM ()
{
  sensorValue1 = analogRead(analogInPin1);
  arr_M[0] = sensorValue1;
  if (arr_M[0] != arr_M[1] && arr_M[0] != 0 && arr_M[1] != 0)
  {
    rpm = 1;
  } 
  else
  {
    rpm = 0;
  }
  arr_M[1] = arr_M[0];
}
void picture()
{
  lcd.createChar(0, a11);
  lcd.createChar(1, a21);
  lcd.createChar(2, a12);
  lcd.createChar(3, a22);
  lcd.createChar(4, a13);
  lcd.createChar(5, a23);
  lcd.createChar(6, a14);
  lcd.createChar(7, a24);
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.write(byte(2));
  lcd.write(byte(3));
  lcd.setCursor(0, 2);
  lcd.write(byte(4));
  lcd.write(byte(5));
  lcd.setCursor(0, 3);
  lcd.write(byte(6));
  lcd.write(byte(7));
  lcd.setCursor(2, 1);
  lcd.write(" Check Print Job!");
  lcd.setCursor(2, 0);
  lcd.write("                  ");
  lcd.setCursor(2, 2);
  lcd.write("                  ");
  lcd.setCursor(2, 3);
  lcd.write("                  ");
}

void clean_disp()
{
  lcd.setCursor(2, 0);
  lcd.write("                     ");
  lcd.setCursor(2, 1);
  lcd.write("                     ");
  lcd.setCursor(2, 2);
  lcd.write("                     ");
  lcd.setCursor(2, 3);
  lcd.write("                     ");

}

void disp()
{
  lcd.setCursor(0, 0);
  lcd.print("Wght:");//<<<<<<<<< connect sensor!!!!!!!!!!!!!
  lcd.setCursor(5, 0);
  lcd.print(wgh);
  lcd.setCursor(8,0);
  lcd.print("g");
  lcd.setCursor(0, 1);
  lcd.print("Diam:");
  lcd.setCursor(5, 1);
  lcd.print(17);
  lcd.setCursor(8,1);
  lcd.print("mm");
  lcd.setCursor(0, 2);
  lcd.print("Temp:");
  //lcd.print("Sp.mont:");
  lcd.setCursor(5, 2);
  lcd.print(t);
  lcd.setCursor(8, 2);
  lcd.print("C");
  //lcd.print("working");
  lcd.setCursor(0, 3);
  lcd.print("Hmdt:");
  lcd.setCursor(5, 3);
  lcd.print( h);
  lcd.setCursor(8, 3);
  lcd.print("%");
  lcd.setCursor(10, 0);
  lcd.print("Time:");
  lcd.setCursor(16, 0);
  int sec = interval / 1000;
  lcd.print(sec);
  if (sec>=5 && sec<10)
  {
    lcd.setCursor(17, 0);
    lcd.print("  ");
  }
  if (sec>=10 && sec<100)
  {
    lcd.setCursor(18, 0);
    lcd.print(" ");
  }
  lcd.setCursor(19, 0);
  lcd.print("s");

}

void b_s()
{
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) 
  {
    if (buttonState == HIGH) 
    {
      buttonPushCounter++;
    }
  }
  lastButtonState = buttonState;
  if (buttonPushCounter % 2 == 1) 
  {
    thisNote = 8;
    flag = HIGH;
    buttonPushCounter++;
    Serial.println("M24");
    clean_disp();
  }
}

void music()
{
  while(1)
  {
    digitalWrite(led, HIGH);
    for (thisNote = 0; thisNote < 8; thisNote++) 
    {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(8, melody[thisNote],noteDuration); //<<<
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(8);
      b_s();
    }
    digitalWrite(led, LOW);
    delay(500);
    if (flag == HIGH)
    {
      flag = LOW;
      break;
    } 
  }
}











