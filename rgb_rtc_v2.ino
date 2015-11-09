#include "Wire.h"
#include <LiquidCrystal.h>

#define DS3231_I2C_ADDRESS 0x68 
#define REDPIN 8
#define GREENPIN 9
#define BLUEPIN 10
#define WHITEPIN 6

unsigned long setSecond = 0; // stores current time value 

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void setup()
{
  Wire.begin();
  Serial.begin(9600);
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(WHITEPIN, OUTPUT);
  
  // set the time of the DS3231 here once. Then comment out and reupload the script! 
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(50,12,16,7,31,10,15);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  if (hour<10) {
    lcd.print("0");
  }
  lcd.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  lcd.print(":");
  if (minute<10)
  {
    lcd.print("0");
  }
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second<10)
  {
    lcd.print("0");
  }
  lcd.print(second, DEC);
  lcd.setCursor(0, 1);
}

void loop()
{
  displayTime(); // display the real-time clock data on the Serial Monitor,
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  if (hour == 23 && minute == 59 && second == 59) {
    setSecond = 0;
  }
  if (hour == 7 && minute == 29 && second == 59) {
    setSecond = 0;
  }
  if (hour == 19 && minute == 29 && second == 59) {
    setSecond = 0;
  }
  
  if (hour >= 20) {
    
      analogWrite(GREENPIN, 5);
      analogWrite(REDPIN, 5);
      analogWrite(BLUEPIN, 80); 
      analogWrite(WHITEPIN, 0);
      lcd.print("Evening    ");
      lcd.setCursor(0, 0);
      
    } else if (hour < 1 && minute < 30) {
      
      lightsOut();
      lcd.print("Lights out!");
      lcd.setCursor(0, 0);
      
    } else if (hour > 1 && hour < 7) {
      
      analogWrite(GREENPIN, 0);
      analogWrite(REDPIN, 0);
      analogWrite(BLUEPIN, 0);
      analogWrite(WHITEPIN, 0);
      lcd.print("Night      ");   
      lcd.setCursor(0, 0);
      
    } else if (hour == 7 && minute < 30) {
      
      analogWrite(GREENPIN, 0);
      analogWrite(REDPIN, 0);
      analogWrite(BLUEPIN, 0);
      analogWrite(WHITEPIN, 0);
      lcd.print("Night      ");   
      lcd.setCursor(0, 0);        
      
    } else if (hour == 7 && minute >= 30) {

      lightsOn();
      lcd.print("Wake up!   ");
      lcd.setCursor(0, 0);
         
    } else if (hour == 19 && minute >= 30){

      sunSet();
      lcd.print("Sunset!    ");
      lcd.setCursor(0, 0);
        
    } else if (hour >= 8 && hour < 19) {     
       
    
      analogWrite(GREENPIN, 130);
      analogWrite(REDPIN, 255);
      analogWrite(BLUEPIN, 65);
      analogWrite(WHITEPIN, 255);
      
      lcd.print("Day   ");   
      lcd.setCursor(0, 0);
      
    } else if (hour == 19 && minute < 30) {      
      analogWrite(GREENPIN, 130);
      analogWrite(REDPIN, 255);
      analogWrite(BLUEPIN, 65);
      analogWrite(WHITEPIN, 255);
      
      lcd.print("Day   ");   
      lcd.setCursor(0, 0);
    }
  delay(1000); // every second
  
  setSecond = setSecond + 1; 
}

void lightsOut () {
  
  float decrementGreen = (float) 5/(30*60);
  float decrementRed = (float) 5/(30*60);
  float decrementBlue = (float) 80/(30*60);
  //float incrementWhite = (float) 0/(30*60);

   float greenVal = (float)(5 - (setSecond * decrementGreen));
   float redVal = (float)(5 - (setSecond * decrementRed));
   float blueVal = (float)(80 - (setSecond * decrementBlue));
   //float whiteVal = (float)sunsetSecond * incrementWhite; 
   
   if(setSecond < 1800)   {
    
     analogWrite(REDPIN, (int) redVal);
     analogWrite(GREENPIN, (int) greenVal);
     analogWrite(BLUEPIN, (int) blueVal);
   //  analogWrite(WHITEPIN,whiteVal);
     
     }  
}

void lightsOn () {
  
  float incrementGreen = (float) 130/(30*60);
  float incrementRed = (float) 255/(30*60);
  float incrementBlue = (float) 65/(30*60);
  float incrementWhite = (float) 255/(30*60);

   //sunrise begins! 
   float greenVal = (float)(setSecond * incrementGreen);
   float redVal = (float)(setSecond * incrementRed);
   float blueVal = (float)(setSecond * incrementBlue);
   float whiteVal = (float)(setSecond * incrementWhite); 
   
   if(setSecond < 1800){
    
     analogWrite(REDPIN, (int) redVal);
     analogWrite(GREENPIN, (int) greenVal);
     analogWrite(BLUEPIN, (int) blueVal);
     analogWrite(WHITEPIN, (int) whiteVal);
     
  }  
}

void sunSet () {
  
  float decrementGreen = (float) 125/(30*60); //5
  float decrementRed = (float) 250/(30*60); //5
  float incrementBlue = (float) 15/(30*60); //80
  float incrementWhite = (float) 255/(30*60); //0

  float greenVal = (float)(130 - (setSecond * decrementGreen));
  float redVal = (float)(255 - (setSecond * decrementRed));
  float blueVal = (float)(65 + (setSecond * incrementBlue));
  float whiteVal = (float)(255 - (setSecond * incrementWhite)); 
     
   if(setSecond < 1800){
    
     analogWrite(REDPIN, (int) redVal);
     analogWrite(GREENPIN, (int) greenVal);
     analogWrite(BLUEPIN, (int) blueVal);
     analogWrite(WHITEPIN, (int) whiteVal);
       
     }
  }  
