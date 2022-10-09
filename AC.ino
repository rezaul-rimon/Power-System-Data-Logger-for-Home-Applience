#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <PZEM004Tv30.h>


#define DHTPIN1 8
#define DHTPIN2 9
#define DHTTYPE DHT22

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,16,2);

PZEM004Tv30 pzem(11, 10);


int t=1, t2=1, tt=1;
int sw1=12, sw2=13;
int con=0;
int comcnt=0;

int consec=0;
int conmin=0;
int conhour=0;
int loopcnt=0;

int actmp=25;
int pactmp = actmp;

void setup() {
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  lcd.init(); 
  lcd.clear();
  lcd.backlight();
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);

  

}

void loop() {
  float tmp2 = dht1.readTemperature();
  float tmp1 = dht2.readTemperature();

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

//////////////////


  if(actmp == pactmp)
  {
    if(con==1)
    {
      consec+=5;
      if(consec>=60)
      {
        consec=0;
        conmin++;
      }
    }
    
    if(con==1 && tt==1)
  {
    tt=0;
    comcnt++;
  }
  if(con==0)
  {
   tt=1;
  }
  
  }

  else if(actmp!=pactmp)
  {
    consec=0;
    conmin=0;
    comcnt=0;
    tt=1;
    pactmp=actmp;
  }



  ///////////
  

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
  Serial.print(conmin);
  Serial.print(",");
  Serial.println(consec);


  unsigned long int pre=millis();

  while((millis()-pre) < 4900)
  {
     if((digitalRead(sw1)==0) && (t==1))
  {
    t=0;
    actmp++;
  }
  if(digitalRead(sw1)==1)
  {
   t=1;
  }


  if((digitalRead(sw2)==0) && (t2==1))
  {
    t2=0;
    actmp--;
  }
  if(digitalRead(sw2)==1)
  {
   t2=1;
  }

  lcd.setCursor(2,0);
  lcd.print("Set Temp: ");
  lcd.setCursor(11,0);
  lcd.print("  ");
  lcd.print(actmp);
  delay(90);
  }

}
