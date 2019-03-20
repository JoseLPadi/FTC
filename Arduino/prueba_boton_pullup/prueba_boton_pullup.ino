#include <OneWire.h>
#include <DallasTemperature.h>

int led = 7; // digital
int button = 30;//digital


const byte pindatostemp = 35;
OneWire oneWireObjeto(pindatostemp);
DallasTemperature sensorDS18B20(&oneWireObjeto);



void setup() {
    Serial.begin(9600);

  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  // put your setup code here, to run once:
  sensorDS18B20.begin();
  
}

void loop() {
  int sensorVal = digitalRead(button);
  Serial.println(sensorVal);
  Serial.println("hola");

  if (sensorVal == HIGH) {
    digitalWrite(led, LOW);
  Serial.println("hola");
} else {
Serial.println("hola");
    digitalWrite(led,HIGH);
  }

  sensorDS18B20.requestTemperatures();
  Serial.println( sensorDS18B20.getTempCByIndex(0));
  
  // put your main code here, to run repeatedly:

}
