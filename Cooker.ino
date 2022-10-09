#include <Wire.h>
#include <RTC.h>
static DS3231 RTC;

#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(10, 11);

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int m=1, t1=1, t2=1, tt=1, f1=0;
int sw1=8, sw2=9;
String mode;

int con=0, ckcnt=0;
int pm = m;

int ontime=0, csc=0, psc=0;
int onmn=0, onhr=0;
//int pday;

void setup() {
  Serial.begin(9600);
  RTC.begin();
  mlx.begin(); 
  lcd.init();
  lcd.clear();
  lcd.backlight();

  if(m==1) mode="COOCK";

  lcd.setCursor(0,0);
  lcd.print("Mode= ");
  lcd.setCursor(7,0);
  lcd.print("          ");
  lcd.setCursor(7,0);
  lcd.print(mode);

  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
}

void loop() {
  
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

  if(current >= 0.03)
  {
    con=1;
  }
  else if(current <0.03)
  {
    con = 0;
  }
  ////////////
  if(con==1 && tt==1)
  {
    tt=0;
    ckcnt++;
  }
  if(con==0)
  {
   tt=1;
   f1=1;
  }
  //////////
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


    if(m!=pm)
  {
      onhr=0;
      onmn=0;
      ontime=0;

      if(con==1) ckcnt=0;
      else ckcnt=0;
      
      pm=m;
  }

  contime=String(onhr)+":"+String(onmn)+":"+String(ontime);

  //////////////

  
  Serial.print("Cooker");
  Serial.print(",");
  Serial.print(d);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.print(mode);
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
  Serial.print(ckcnt);
  Serial.print(",");
//  Serial.print(onhr);
//  Serial.print(",");
//  Serial.print(onmn);
//  Serial.print(",");
  Serial.println(contime);


/////////////////
  unsigned long int pre = millis();;
  while(millis()-pre < 5000)
  {
    if((digitalRead(sw1)==0) && (t1==1))
  {
    t1=0;
    m++;

    if(m>6) m=1;
    if(m<1) m=6;

    if(m==1) mode="COOCK";
    else if(m==2) mode="WARM";
    else if(m==3) mode="SOUP";
    else if(m==4) mode="FRY";
    else if(m==5) mode="MILK/TEA";
    else if(m==6) mode="ROTI/DOSA";
    
    lcd.setCursor(7,0);
    lcd.print("          ");
    lcd.setCursor(7,0);
    lcd.print(mode);
  }
  
  if(digitalRead(sw1)==1)
  {
   t1=1;
  }


  if((digitalRead(sw2)==0) && (t2==1))
  {
    t2=0;
    m--;

    if(m>6) m=1;
    if(m<1) m=6;

    if(m==1) mode="COOCK";
    else if(m==2) mode="WARM";
    else if(m==3) mode="SOUP";
    else if(m==4) mode="FRY";
    else if(m==5) mode="MILK/TEA";
    else if(m==6) mode="ROTI/DOSA";
    
    lcd.setCursor(7,0);
    lcd.print("          ");
    lcd.setCursor(7,0);
    lcd.print(mode);
  }
  
  if(digitalRead(sw2)==1)
  {
   t2=1;
  }
    delay(45);
  }
///////////////////// 
} 
