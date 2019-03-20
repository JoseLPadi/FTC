#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// pin caudalimetro 2
// pin bomba = 23
// pin sirena = 25
// pin fermentador 27
// pin fermentador 29
// pin fermentador 31










const byte pindatostemp = 51;
OneWire oneWireObjeto(pindatostemp);
DallasTemperature sensorDS18B20(&oneWireObjeto);

const byte pindatostemp1 = 50;
OneWire oneWireObjeto1(pindatostemp1);
DallasTemperature sensorDS18B201(&oneWireObjeto1);
/* Create some custom graphic characters to make a simple border */

// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte pinAltaTemperatura = 5;
const byte pinBajaTemperatura = 4;

void setup()
{
  Serial.begin(2000000);

  Serial.println("adsfasdfasdfasdf");
  // initialize the LCD
  //lcd.init();

 // pinMode(pinAltaTemperatura, OUTPUT);
 // pinMode(pinBajaTemperatura, OUTPUT);
  sensorDS18B20.begin();
  sensorDS18B201.begin();
 


}

void loop(){

delay(3000);
  //lcd.backlight();

//lcd.clear();
//lcd.home();
//  lcd.setCursor(0, 0);

 //  lcd.print("TEMPERATURA:::");


 // lcd.clear();
//  lcd.home();

/*  
  lcd.setCursor(0,1);
  lcd.print(val);*/
/*
  if (val > 20){
    digitalWrite(pinAltaTemperatura, HIGH);
    digitalWrite(pinBajaTemperatura, LOW);
  } else {
    digitalWrite(pinBajaTemperatura,HIGH);
    digitalWrite(pinAltaTemperatura,LOW);
    *
  }*/
  sensorDS18B20.requestTemperatures();
  sensorDS18B201.requestTemperatures();
  float val = sensorDS18B20.getTempCByIndex(0);
  float val1 = sensorDS18B201.getTempCByIndex(0);

  Serial.println("leo temperatura");
  Serial.println(val);  Serial.println(val1);

  
  // Do nothing here...
}
