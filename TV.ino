
#include <Wire.h>
#include <RTC.h>

static DS3231 RTC;

#include "DHT.h"
#define DHTPIN1 9
#define DHTTYPE DHT22

DHT dht1(DHTPIN1, DHTTYPE);

#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(10, 11);


int tvon=0;
int tt=1;
int tvcnt=0;
int f1=1;
int ontime=0, csc=0, psc=0;
int onmn=0, onhr=0;
int pday;

void setup()
{
  Serial.begin(9600);
  RTC.begin();
  dht1.begin();
  pday=RTC.getHours();
  
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

    String d,t, contime;
    d=String(dt)+"/"+String(mt)+"/"+String(yr);
    t=String(hr)+":"+String(mn)+":"+String(sc);
    
    float tmp1 = dht1.readTemperature();
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float pf = pzem.pf();
    float frequency = pzem.frequency();


    if(current>=0.2) 
    {
      tvon=1;
    }
    else if(current<0.2) 
    {
      tvon=0;
    }

    if(tvon==1 && tt==1)
  {
    tt=0;
    tvcnt++;
  }
  if(tvon==0)
  {
   tt=1;
   f1=1;
  }
///////////

    if(tvon==1)
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

    if(pday != hr)
    {
      onhr=0;
      onmn=0;
      tvcnt=0;
      pday=hr;
      ontime=0;
    }

  contime=String(onhr)+":"+String(onmn)+":"+String(ontime);
    /////////////////
    
    Serial.print(d);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(tmp1);
    Serial.print(",");
    Serial.print(voltage);
    Serial.print(",");
    Serial.print(current);
    Serial.print(",");
    Serial.print(power);
    Serial.print(",");
    Serial.print(pf);
    Serial.print(",");
    Serial.print(frequency);
    Serial.print(",");
    Serial.print(tvon);
    Serial.print(",");
    Serial.print(tvcnt);
    Serial.print(",");
//    Serial.print(onhr);
//    Serial.print(",");
//    Serial.print(onmn);
//    Serial.print(",");
    Serial.println(contime);


    unsigned long int pre = millis();;
    while(millis()-pre < 5000)
    {
      delay(490);
    }


}
