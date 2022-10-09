#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <RTC.h>

static DS3231 RTC;

#include "DHT.h"
#define DHTPIN1 8
#define DHTPIN2 10

#define DHTTYPE DHT22

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,16,2);

PZEM004Tv30 pzem(12, 11);


int con=0;
int comcnt=0;
int tt1=1;
int tt=1;
int f1=1;
int ontime=0, csc=0, psc=0;
int onmn=0, onhr=0;

int comcntcpy=0, ontimecpy=0, onmncpy=0, onhrcpy=0, dccpy=0;


int sw=0, dc=0;

int t1=1, t2=1;
int sw1=6, sw2=7;
int frtmp=-20;
int ptmp=frtmp;

void setup()
{
  Serial.begin(9600);
  RTC.begin();
  dht1.begin();
  dht2.begin();
  //pday=RTC.getDay();

  lcd.init(); 
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(2,0);
  lcd.print("Set Temp:");
  lcd.setCursor(11,0);
  lcd.print("   ");
  lcd.setCursor(11,0);
  lcd.print(frtmp);
  
  pinMode(4, INPUT_PULLUP); //unused
  pinMode(5, INPUT_PULLUP);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  
//  RTC.setHourMode(CLOCK_H24);
//  RTC.setDateTime(__DATE__, __TIME__);
//  RTC.startClock(); 
  
}

void loop()
{
  int dt=RTC.getDay();
  int mt=RTC.getMonth();
  int yr=RTC.getYear()-2000;
  int hr=RTC.getHours();
  int mn=RTC.getMinutes();
  int sc=RTC.getSeconds();

    String d,t,contime, contimecpy;
    d=String(dt)+"/"+String(mt)+"/"+String(yr);
    t=String(hr)+":"+String(mn)+":"+String(sc);
    
    float tmp1 = dht1.readTemperature();
    float tmp2 = dht2.readTemperature();
    
    
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float pf = pzem.pf();
    float frequency = pzem.frequency();


    if(current>=0.9) 
    {
      con=1;
    }
    else if(current<0.9) 
    {
      con=0;
    }
///////

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

  if(digitalRead(5)==0)
  {
    sw = 1;
  }
  else if(digitalRead(5)==1) 
  { 
    sw = 0; 
    }

if((digitalRead(5)==0) && (tt1==1))
  {
    tt1=0;
    dc++;
  }
  if(digitalRead(5)==1)
  {
   tt1=1;
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
////////////

    if(ptmp != frtmp)
    {
      onhr=0;
      onmn=0;
      ontime=0;
      
      if(con==1) comcnt=1;
      else comcnt=0;
      
      dc=0;
      ptmp=frtmp;
      
    }

    contime=String(onhr)+":"+String(onmn)+":"+String(ontime);

    //////////////

    
    Serial.print("FRIDGE");
    Serial.print(",");
    Serial.print(d);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(frtmp);
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
    Serial.print(sw);
    Serial.print(",");
    Serial.print(dc);
    Serial.print(",");
    Serial.print(con);
    Serial.print(",");
    Serial.print(comcnt);
    Serial.print(",");
    Serial.println(contime);



  unsigned long int pre=millis();
  while((millis()-pre) < 4900)
  {
     if((digitalRead(sw1)==0) && (t1==1))
  {
    t1=0;
    frtmp++;

    lcd.setCursor(11,0);
    lcd.print("   ");
    lcd.setCursor(11,0);
    lcd.print(frtmp);
  }
  if(digitalRead(sw1)==1)
  {
   t1=1;
  }


  if((digitalRead(sw2)==0) && (t2==1))
  {
    t2=0;
    frtmp--;

    lcd.setCursor(11,0);
    lcd.print("   ");
    lcd.setCursor(11,0);
    lcd.print(frtmp);
  }
  if(digitalRead(sw2)==1)
  {
   t2=1;
  }
  
  delay(50);
  }


}
