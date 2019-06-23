//Libraries for LCD and I2C Modules
#include <Wire.h>   
#include <LiquidCrystal_I2C.h> 

const byte interruptPin = 2; //Button on pin 2 for unit change
volatile byte mode = LOW; //variable for remembering current unit

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display
  
//variables for magnitude and North South directions
float gauss;
float mT;
char NS; 
int aValue; //for raw analog sensor value

void setup(){
  pinMode(interruptPin, INPUT_PULLUP); //pin 2 is HIGH when nothing connected
  attachInterrupt(digitalPinToInterrupt(interruptPin), change_unit, FALLING); //assigning an ISR for pin 2 state change
  
  //Serial.begin(9600); //Uncomment for serial monitor display
  
  lcd.begin(); // initialize the LCD
  lcd.backlight(); // Turn on the blacklight.
  lcd.home(); //Cursor set at 1st row 1st column
  lcd.print("  Magnetometer  ");
}   


void loop(){
  aValue =analogRead(0); 
  if ((aValue > 490) && (aValue < 540)) //Ananlog value floats between 490 and 540 when no magnet present
  {
    aValue=512;  //ideal value for 0 magnetic field according to datasheet
  }
  
  //x = map(aValue, 0, 1024, 0, XRANGE); //Uncomment for serial monitor display
  
  gauss = map(aValue, 206, 818, -1000, 1000); //scaling according to datasheet (1V=-1000 gauss 4V= 1000 gauss)
  

  //direction according to sign
  if (gauss<0){
    NS = 'S';
    gauss *= -1;} 
  else if(gauss>0){NS='N';}
  else if(gauss==0){NS=' ';}

  mT=gauss/10.0; //gauss to millitesla conversion
  
  //printing the value according to button/unit state
  if (mode==LOW){
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(gauss);
    lcd.print(" ");
    lcd.print(NS);
    lcd.setCursor(9,1);
    lcd.print(" Gauss   ");
    }

    
  //printing the value according to button/unit state
  else if (mode == HIGH){
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(mT);
    lcd.print(" ");
    lcd.print(NS);
    lcd.setCursor(9,1);
    lcd.print(" mT      ");
    }


//Uncomment for serial monitor display
//  Serial.print("|");
//  for (int i=0;i<x;i++){
//    if(i==XRANGE/2-1)Serial.print("|");
//    else Serial.print("-");
//  }
//  Serial.print("O");
//  for (int i=x+1;i<XRANGE;i++){
//    if(i==XRANGE/2-1)Serial.print("|");
//    else Serial.print("-");
//  }
//  Serial.print("|");
//Serial.print(gss);
//  Serial.println("  Gauss");


 
 delay(1000);
} 

//Function to run when interrupt occurs (ISR)
void change_unit() {
  mode = !mode;
}
