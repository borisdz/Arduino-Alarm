#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

//--------Display settings:--------//
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int contrast = 75;
//--------RTC settings:--------//
RTC_DS1307 RTC;
int hour_update;
int min_update;
int year_update;
int month_update;
int day_update;

//--------App settings:--------//
int menu = 0;
int setAll = 0;
uint8_t alarmHours = 0, alarmMinutes = 0;
//--------Buttons settings:--------//
int P1 = 7;   // SET MENU
int P2 = 8;   // +
int P3 = 9;   // -
int P4 = 13;  // switch
//--------Music settings:--------//
#define buzzer 10


//--------CODE BEGIN:--------//
void setup()
{
  lcd.begin(16, 2);
  analogWrite(6,contrast);
  pinMode(P1,INPUT_PULLUP); 
  pinMode(P2,INPUT_PULLUP);
  pinMode(P3,INPUT_PULLUP);
  pinMode(P4,INPUT_PULLUP);
  pinMode(buzzer, OUTPUT); // Set buzzer as an output
  printAllOff();
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Set the date and time at compile time
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  int menu=0;
}
 
void loop(){ 
  if(digitalRead(P1)== LOW)   {
   menu=menu+1;
  }

  if((digitalRead(P2)== LOW)&&(digitalRead(P3)== LOW)){
    DisplaySetHourAll();
    DisplaySetMinuteAll();
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("ALARM");
    lcd.setCursor(5,1);
    lcd.print(alarmHours, DEC);
    lcd.print(":");
    lcd.print(alarmMinutes, DEC);
    delay(1000);
    lcd.clear();
  }
  switch(menu){
    case 0:
      default: DisplayDateTime();
      Alarm();
      break;
    case 1:
      DisplaySetHour();
      break;
    case 2:
      DisplaySetMinute();
      break;
    case 3:
      DisplaySetYear();
      break;
    case 4:
      DisplaySetMonth();
      break;
    case 5: 
      DisplaySetDay();
      break;
    case 6:
      SaveVars();
      delay(500);
      menu=0;
      break;
  }
    delay(100);
}

void DisplayDateTime (){
  DateTime now = RTC.now();

  lcd.setCursor(4,0);  
  if (now.hour()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  hour_update=now.hour();
  lcd.print(":");
  if (now.minute()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);
  min_update=now.minute();
  lcd.print(":");
  if (now.second()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.second(), DEC);

  
  lcd.setCursor(3, 1);
  if (now.day()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.day(), DEC);
  day_update=now.day();
  lcd.print("/");
  if (now.month()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);
  month_update=now.month();
  lcd.print("/");
  lcd.print(now.year(), DEC);
  year_update=now.year();
}

void DisplaySetHour(){
  lcd.clear();
  DateTime now = RTC.now();
  if(digitalRead(P2)==LOW)
  {
    if(hour_update==23)
    {
      hour_update=0;
    }
    else
    {
      hour_update=hour_update+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if(hour_update==0)
    {
      hour_update=23;
    }
    else
    {
      hour_update=hour_update-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set time:");
  lcd.setCursor(0,1);
  lcd.print(hour_update,DEC);
  delay(200);
}

void DisplaySetMinute(){
  lcd.clear();
  if(digitalRead(P2)==LOW)
  {
    if (min_update==59)
    {
      min_update=0;
    }
    else
    {
      min_update=min_update+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (min_update==0)
    {
      min_update=59;
    }
    else
    {
      min_update=min_update-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  lcd.print(min_update,DEC);
  delay(200);
}
  
void DisplaySetYear(){
  lcd.clear();
  if(digitalRead(P2)==LOW)
  {    
    year_update=year_update+1;
  }
   if(digitalRead(P3)==LOW)
  {
    year_update=year_update-1;
  }
  lcd.setCursor(0,0);
  lcd.print("Set Year:");
  lcd.setCursor(0,1);
  lcd.print(year_update,DEC);
  delay(200);
}

void DisplaySetMonth(){
  lcd.clear();
  if(digitalRead(P2)==LOW)
  {
    if (month_update==12)
    {
      month_update=1;
    }
    else
    {
      month_update=month_update+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (month_update==1)
    {
      month_update=12;
    }
    else
    {
      month_update=month_update-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Month:");
  lcd.setCursor(0,1);
  lcd.print(month_update,DEC);
  delay(200);
}

void DisplaySetDay(){
  lcd.clear();
  if(digitalRead(P2)==LOW)
  {
    if (day_update==31)
    {
      day_update=1;
    }
    else
    {
      day_update=day_update+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (day_update==1)
    {
      day_update=31;
    }
    else
    {
      day_update=day_update-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Day:");
  lcd.setCursor(0,1);
  lcd.print(day_update,DEC);
  delay(200);
}

void SaveVars(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAVING IN");
  lcd.setCursor(0,1);
  lcd.print("PROGRESS");
  RTC.adjust(DateTime(year_update,month_update,day_update,hour_update,min_update,0));
  delay(200);
}

void DisplaySetHourAll(){
  while(digitalRead(P1)==HIGH){

  lcd.clear();

  if(digitalRead(P2)==LOW)
  {
    if(alarmHours==23)
    {
      alarmHours=0;
    }
    else
    {
      alarmHours=alarmHours+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if(alarmHours==0)
    {
      alarmHours=23;
    }
    else
    {
      alarmHours=alarmHours-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set HOUR Alarm:");
  lcd.setCursor(0,1);
  lcd.print(alarmHours,DEC);
  delay(200);
 }
 delay(200);
}

void DisplaySetMinuteAll(){
  while(digitalRead(P1)==HIGH){ 

  lcd.clear();
  if(digitalRead(P2)==LOW)
  {
    if (alarmMinutes==59)
    {
      alarmMinutes=0;
    }
    else
    {
      alarmMinutes=alarmMinutes+1;
    }
  }
   if(digitalRead(P3)==LOW)
  {
    if (alarmMinutes==0)
    {
      alarmMinutes=59;
    }
    else
    {
      alarmMinutes=alarmMinutes-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set MIN. Alarm:");
  lcd.setCursor(0,1);
  lcd.print(alarmMinutes,DEC);
  delay(200);
 }
 delay(200);
}

void printAllOn(){
  lcd.setCursor(15,0);
  lcd.print("A");
}

void printAllOff(){
  lcd.clear();
  DisplayDateTime(); 
}

void Alarm(){
   if(digitalRead(P4)== LOW)
  {
   setAll=setAll+1;
  }
  if (setAll==0)
    {
     printAllOff();
     noTone (buzzer);
     }
  if (setAll==1)
    {

     printAllOn();    
  
     DateTime now = RTC.now();
     if ( now.hour() == alarmHours && now.minute() == alarmMinutes )
        {
         DateTime now = RTC.now();
         tone(buzzer,50);
         //if(digitalRead(P4)==LOW)
           //noTone(buzzer);
        }
    else{
         noTone (buzzer);
        }
    
    } 
  if (setAll==2)
    {
     setAll=0;
    }
    delay(200);
}

void playSong(){
  
}
