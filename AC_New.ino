
#include <Wire.h>
#include <RTC.h>
#include <LiquidCrystal_I2C.h>
static DS3231 RTC;
#include <PZEM004Tv30.h>

#include "DHT.h"

#define DHTPIN1 8
#define DHTPIN2 9
#define DHTTYPE DHT22

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);


LiquidCrystal_I2C lcd(0x27,16,2);

PZEM004Tv30 pzem(11, 10);


int con=0, comcnt=0;
int t1=1, t2=1, tt=1, f1=0;
int sw1=12, sw2=13;

int actmp=25;
int pactmp = actmp;

int ontime=0, csc=0, psc=0;
int onmn=0, onhr=0;
int pday;

void setup()
{
  
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  lcd.init(); 
  lcd.clear();
  lcd.backlight();

  
  lcd.setCursor(2,0);
  lcd.print("Set Temp: ");
  lcd.setCursor(12,0);
  lcd.print("   ");
  lcd.setCursor(12,0);
  lcd.print(actmp);


  
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);

  RTC.begin();
  //pday=RTC.getDay();
  
  //RTC.setHourMode(CLOCK_H24);
  //RTC.setDateTime(__DATE__, __TIME__);
  //RTC.startClock(); 
  
}

void loop()
{
  int dt=RTC.getDay();
  int mt=RTC.getMonth()+4;
  int yr=RTC.getYear()-2000;
  int hr=RTC.getHours();
  int mn=RTC.getMinutes();
  int sc=RTC.getSeconds();

    String d,t,contime;
    d=String(dt)+"/"+String(mt)+"/"+String(yr);
    t=String(hr)+":"+String(mn)+":"+String(sc);
    
    float tmp1 = dht1.readTemperature();
    float tmp2 = dht2.readTemperature();
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float pf = pzem.pf();
    float frequency = pzem.frequency();


    if(current >= 1.0)
  {
    con=1;
  }
  else if(current <1.0)
  {
    con = 0;
  }
/////////

    if(con==1 && tt==1)
  {
    tt=0;
    comcnt++;
  }
  if(con==0)
  {
   tt=1;
   f1=1;
  }
///////////

  if(con==1)
    {
      if(f1==1)
      {
      f1=0;
      psc=sc;
      }
      
      csc=sc;
      
      if(csc < psc) 
      {
        ontime += (60+csc)-psc;
      }
      else if(csc >= psc)
      {
        ontime += (csc-psc);
      }

      if(ontime >= 60)
      {
        onmn++;
        ontime=ontime-60;
      }

      if(onmn >= 60)
      {
        onhr++;
        onmn=onmn-60;
      }
      
      psc=csc;
    }


    if(actmp!=pactmp)
  {
      onhr=0;
      onmn=0;
      ontime=0;
      comcnt=0;
      pactmp=actmp;
  }

  contime=String(onhr)+":"+String(onmn)+":"+String(ontime);
////////////


    
    
    Serial.print(d);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(actmp);
    Serial.print(",");
    Serial.print(tmp1);
    Serial.print(",");
    Serial.print(tmp2);
    Serial.print(",");
    Serial.print(voltage);
    Serial.print(",");
    Serial.print(current);
    Serial.print(",");
    Serial.print(power);
    Serial.print(",");
    Serial.print(pf);
    Serial.print(",");
    Serial.print(frequency,0);
    Serial.print(",");
    Serial.print(con);
    Serial.print(",");
    Serial.print(comcnt);
    Serial.print(",");
//    Serial.print(onhr);
//    Serial.print(",");
//    Serial.print(onmn);
//    Serial.print(",");
    Serial.println(contime);



  unsigned long int pre=millis();
  while((millis()-pre) < 4900)
  {
     if((digitalRead(sw1)==0) && (t1==1))
  {
    t1=0;
    actmp++;

    lcd.setCursor(12,0);
    lcd.print("   ");
    lcd.setCursor(12,0);
    lcd.print(actmp);
  }
  if(digitalRead(sw1)==1)
  {
   t1=1;
  }


  if((digitalRead(sw2)==0) && (t2==1))
  {
    t2=0;
    actmp--;

    lcd.setCursor(12,0);
    lcd.print("   ");
    lcd.setCursor(12,0);
    lcd.print(actmp);
  }
  if(digitalRead(sw2)==1)
  {
   t2=1;
  }
  
  delay(90);
  }



}
