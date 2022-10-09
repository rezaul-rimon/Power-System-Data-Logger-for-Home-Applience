
#include <Wire.h>
#include <RTC.h>
#include <LiquidCrystal_I2C.h>
static DS3231 RTC;
#include <PZEM004Tv30.h>

#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

LiquidCrystal_I2C lcd(0x27,16,2);
PZEM004Tv30 pzem(9, 8);

int rpm=0, count=0, r1=1;

int fon=0, fcnt=0;
int t1=1, t2=1, tt=1, f1=0;
int sw2=A0, sw1=A1;

int fspd=3;
int pfspd = fspd;

int ontime=0, csc=0, psc=0;
int onmn=0, onhr=0;
//int pday;

void setup()
{
  
  Serial.begin(9600);
  mlx.begin();
  lcd.init(); 
  lcd.clear();
  lcd.backlight();

  
  lcd.setCursor(1,0);
  lcd.print("Set Speed: ");
  lcd.setCursor(12,0);
  lcd.print("    ");
  lcd.setCursor(12,0);
  lcd.print(fspd);


  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);

  RTC.begin();
  //pday=RTC.getDay();
  
  //RTC.setHourMode(CLOCK_H24);
  //RTC.setDateTime(__DATE__, __TIME__);
  //RTC.startClock(); 
  
}

void loop()
{
  int dt=RTC.getDay();
  int mt=RTC.getMonth();
  int yr=RTC.getYear()-2000;
  int hr=RTC.getHours();
  int mn=RTC.getMinutes();
  int sc=RTC.getSeconds();

    String d,t,contime;
    d=String(dt)+"/"+String(mt)+"/"+String(yr);
    t=String(hr)+":"+String(mn)+":"+String(sc);
    
    float tmp1=mlx.readAmbientTempC();
    float tmp2=mlx.readObjectTempC();
    
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float pf = pzem.pf();
    float frequency = pzem.frequency();


    if(current >= 0.15)
  {
    fon=1;
  }
  else if(current < 0.15)
  {
    fon = 0;
  }
/////////

    if(fon==1 && tt==1)
  {
    tt=0;
    fcnt++;
  }
  if(fon==0)
  {
   tt=1;
   f1=1;
  }
///////////

  if(fon==1)
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


    if(fspd!=pfspd)
  {
      onhr=0;
      onmn=0;
      ontime=0;

      if(fon==1) fcnt=1;
      else fcnt=1;
      
      pfspd=fspd;
  }

  contime=String(onhr)+":"+String(onmn)+":"+String(ontime);
////////////


    
    Serial.print("FAN");
    Serial.print(",");
    Serial.print(d);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(fspd);
    Serial.print(",");
    Serial.print(rpm);
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
    Serial.print(fon);
    Serial.print(",");
    Serial.print(fcnt);
    Serial.print(",");
//    Serial.print(onhr);
//    Serial.print(",");
//    Serial.print(onmn);
//    Serial.print(",");
    Serial.println(contime);


///////////
count=0;
  for(int i=0; i<950; i++)
  {
  if((digitalRead(A2)==LOW) && (r1==1))
  {
    r1=0;
    count++;
  }
  
  if(digitalRead(A2)==1)
  {
   r1=1;
  }

  if((digitalRead(sw1)==0) && (t1==1))
  {
    t1=0;
    fspd++;
    if(fspd >= 5) fspd=5;

    lcd.setCursor(12,0);
    lcd.print("   ");
    lcd.setCursor(12,0);
    lcd.print(fspd);
  }
  if(digitalRead(sw1)==1)
  {
   t1=1;
  }

  if((digitalRead(sw2)==0) && (t2==1))
  {
    t2=0;
    fspd--;
    if(fspd <= 0) fspd=0;

    lcd.setCursor(12,0);
    lcd.print("   ");
    lcd.setCursor(12,0);
    lcd.print(fspd);
  }
  if(digitalRead(sw2)==1)
  {
   t2=1;
  }

  delay(5);
  }
  rpm=count*4;

}
