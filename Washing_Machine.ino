#include <Wire.h>
#include <RTC.h>
static DS3231 RTC;

#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(9, 8);

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

String modesl, mode;
int pm=0, ms=1, m=0; 
int sw1=A3, sw2=A2, sw3=A1, sw4=A0;
int t1=1, t2=1, t3=1, t4=1;
int rpm=0, tmp=25;


int wmon=0;
int tt=1;
int wmcnt=0;
int f1=1;
int ontime=0, csc=0, psc=0;
int onmn=0, onhr=0;
int pday;


double water;
unsigned long pulse_freq;
///////////////

void pulse() // Interrupt function
{
  pulse_freq++;
}
/////////////////
void setup() {
  pinMode(2, INPUT);
  attachInterrupt(0, pulse, RISING); // Setup Interrupt
  
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(sw4, INPUT_PULLUP);

  Serial.begin(9600);
  
  RTC.begin(); 
  lcd.init();
  lcd.clear();
  lcd.backlight();

  if(ms==1) modesl="Wash Mode Select";
  if(m==0) mode="STD_B";

  lcd.setCursor(0,0);
  lcd.print(modesl);

  lcd.setCursor(0,1);
  lcd.print("Mode= ");
  lcd.setCursor(6,1);
  lcd.print(mode);

  lcd.setCursor(0,2);
  lcd.print("Temperature= ");
  lcd.setCursor(13,2);
  lcd.print(tmp);

  lcd.setCursor(0,3);
  lcd.print("Speed/RPM= ");
  lcd.setCursor(11,3);
  lcd.print(rpm);

}
/////////////////////

void loop() {

  int dt=RTC.getDay();
  int mt=RTC.getMonth();
  int yr=RTC.getYear()-2000;
  int hr=RTC.getHours();
  int mn=RTC.getMinutes();
  int sc=RTC.getSeconds();

    String d,t, contime;
    d=String(dt)+"/"+String(mt)+"/"+String(yr);
    t=String(hr)+":"+String(mn)+":"+String(sc);

    water = .00225 * pulse_freq;
    
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float pf = pzem.pf();
    float frequency = pzem.frequency();
    


    if(current>=0.15) 
    {
      wmon=1;
      if(current>0.2) rpm=current*300;
      else rpm=0;
    }
    else if(current<0.15) 
    {
      rpm=0;
      wmon=0;     
    }

    if(wmon==1 && tt==1)
  {
    tt=0;
    wmcnt++;
  }
  if(wmon==0)
  {
   tt=1;
   f1=1;
  }
///////////

    if(wmon==1)
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

    if(pm != m)
    {
      pulse_freq=0;
      onhr=0;
      onmn=0;
      
      if(wmon==1) wmcnt=1;
      else wmcnt=0;
      
      pm=m;
      ontime=0;
    }

  contime=String(onhr)+":"+String(onmn)+":"+String(ontime);
    /////////////////
    
    Serial.print("WM");
    Serial.print(",");
    Serial.print(d);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(mode);
    Serial.print(",");
    Serial.print(tmp);
    Serial.print(",");
    Serial.print(rpm);
    Serial.print(",");
    Serial.print(water);
    Serial.print("L");
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
    Serial.print(wmon);
    Serial.print(",");
    Serial.print(wmcnt);
    Serial.print(",");
//    Serial.print(onhr);
//    Serial.print(",");
//    Serial.print(onmn);
//    Serial.print(",");
    Serial.println(contime);



  
  ///////

unsigned long int pre=millis();
while((millis()-pre) < 4900)
{
    if((digitalRead(sw1)==0) && (t1==1))
  {
    t1=0;
    ms++;

    if(ms>3) ms=1;

    if(ms==1) modesl="Wash Mode Select";
    else if(ms==2) modesl="Temperature Set ";
    else if(ms==3) modesl="RMP Set         ";
   
    lcd.setCursor(0,0);
    lcd.print(modesl);
  }
  
  if(digitalRead(sw1)==1)
  {
   t1=1;
  }


  if((digitalRead(sw2)==0) && (t2==1))
  {
    t2=0;
    ms--;

    if(ms<1) ms=3;

    if(ms==1) modesl="Wash Mode Select";
    else if(ms==2) modesl="Temperature Set ";
    else if(ms==3) modesl="RMP Set         ";
   
    lcd.setCursor(0,0);
    lcd.print(modesl);
  }
  
  if(digitalRead(sw2)==1)
  {
   t2=1;
  }
////////////


if(ms == 1)
{
  if((digitalRead(sw3)==0) && (t3==1))
  {
    t3=0;
    m++;

    if(m>16) m=0;

    if(m==0) mode="STD_B";
    else if(m==1) mode="MIX_W";
    else if(m==2) mode="COT_E";
    else if(m==3) mode="BCK_P";
    else if(m==4) mode="INT_W";
    else if(m==5) mode="WOOL ";
    else if(m==6) mode="SLK_W";

    else if(m==7) mode="DWN_J";
    else if(m==8) mode="DRM_C";
    else if(m==9) mode="SPIN ";
    else if(m==10) mode="RNS+S";
    else if(m==11) mode="DRY  ";

    else if(m==12) mode="BBY_W";
    else if(m==13) mode="SHIRT";
    else if(m==14) mode="UND_W";
    else if(m==15) mode="DRK_C";
    else if(m==16) mode="QCK_W";

    lcd.setCursor(6,1);
    lcd.print(mode);
  }
  if(digitalRead(sw3)==1)
  {
   t3=1;
  }

  if((digitalRead(sw4)==0) && (t4==1))
  {
    t4=0;
    m--;

    if(m<0) m=16;

    if(m==0) mode="STD_B";
    else if(m==1) mode="MIX_W";
    else if(m==2) mode="COT_E";
    else if(m==3) mode="BCK_P";
    else if(m==4) mode="INT_W";
    else if(m==5) mode="WOOL ";
    else if(m==6) mode="SLK_W";

    else if(m==7) mode="DWN_J";
    else if(m==8) mode="DRM_C";
    else if(m==9) mode="SPIN ";
    else if(m==10) mode="RNS+S";
    else if(m==11) mode="DRY  ";

    else if(m==12) mode="BBY_W";
    else if(m==13) mode="SHIRT";
    else if(m==14) mode="UND_W";
    else if(m==15) mode="DRK_C";
    else if(m==16) mode="QCK_W";

    lcd.setCursor(6,1);
    lcd.print(mode);
  }
  if(digitalRead(sw4)==1)
  {
   t4=1;
  }
  
}
//////////


else if(ms == 2)
{
  if((digitalRead(sw3)==0) && (t3==1))
  {
    t3=0;
    tmp++;

    if(tmp>70) tmp=70;

    lcd.setCursor(13,3);
    lcd.print("   ");
    lcd.setCursor(13,2);
    lcd.print(tmp);
  }
  if(digitalRead(sw3)==1)
  {
   t3=1;
  }

  if((digitalRead(sw4)==0) && (t4==1))
  {
    t4=0;
    tmp--;
    
    if(tmp<10) tmp=10;

    lcd.setCursor(13,3);
    lcd.print("   ");
    lcd.setCursor(13,2);
    lcd.print(tmp);
  }
  if(digitalRead(sw4)==1)
  {
   t4=1;
  }
}
//////////////


else if(ms == 3)
{
  if((digitalRead(sw3)==0) && (t3==1))
  {
    t3=0;
    rpm+=5;

    if(rpm>500) rpm=500;

    lcd.setCursor(11,3);
    lcd.print("   ");
    lcd.setCursor(11,3);
    lcd.print(rpm);
  }
  if(digitalRead(sw3)==1)
  {
   t3=1;
  }

  if((digitalRead(sw4)==0) && (t4==1))
  {
    t4=0;
    rpm-=5;
    
    if(rpm<0) rpm=0;

    lcd.setCursor(11,3);
    lcd.print("   ");
    lcd.setCursor(11,3);
    lcd.print(rpm);
  }
  if(digitalRead(sw4)==1)
  {
   t4=1;
  }
}
 /////////////////
  
 delay(50);
  }

}
