#include "Wire.h"
#include <LiquidCrystal.h>

#define DS3231_I2C_ADDRESS 0x68 
#define REDPIN 8
#define GREENPIN 9
#define BLUEPIN 10

int switchPin = 13;

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
  pinMode(switchPin, INPUT_PULLUP);
  
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
  if (hour >= 21 || hour < 1) {
      analogWrite(GREENPIN, 5);
      analogWrite(REDPIN, 5);
      analogWrite(BLUEPIN, 80); 
      lcd.print("Evening");
      lcd.setCursor(0, 0);
    } else if (hour >= 1 && hour < 9) {
      analogWrite(GREENPIN, 0);
      analogWrite(REDPIN, 0);
      analogWrite(BLUEPIN, 0);
      lcd.print("Night  ");   
      lcd.setCursor(0, 0);
    } else {
      analogWrite(GREENPIN, 130);
      analogWrite(REDPIN, 255);
      analogWrite(BLUEPIN, 65);
      lcd.print("Day   ");   
      lcd.setCursor(0, 0);
    }
    
  delay(1000); // every second
}
