/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 16, 2018.
  Released into the public domain.
*/
#ifndef Fermenter_h
#define Fermenter_h
 
#include "Arduino.h"
 #include <OneWire.h> 
#include <DallasTemperature.h>


/* Ls valores de ListSensorTemperature son guardados en memoria, la idea es que en caso de que se corte la luz, estos valores se puedan volver a obtener. 
Luego se deberia verificar que fermentador prender, se enciende de a un fermentador por vez. 
- Siempre se habilita el fermentador que esta mas lejos de la temperatura necesaria.
- a cada fermentador se le setea la temperatura por separado.  */

/*
IMPORTANTE
el rango va a ser... la temperatura seteada + rango
entonces el rango en el que va a mantener la temperatura va a ser...
[temperatura, temperatura + rango]
*/

class Fermenter{
  public:
    Fermenter(int pinTermometer, int pinElectrovalveCold,  int temperatureFermenterSetted, bool isFermenterWorking, float timeConnected);
    

	
	//inicia o detiene el circuito, y devuelve true si esta funcionando o si inicia y false si esta detenido o si se detiene
	bool startStopCircuit();
	void stopCircuit();



//	float getDifferenceTemperature();

	float getDifferenceTemperature();
	///// GETTER AND SETTER
	float getTimeValveWorking();
	bool getIsCircuitEnabled();
	float getTotalTimeConnected();
	float getTotalTimeInOneStep();
	bool getIsFermenterWorking();
	void setIsFermenterWorking(bool fermenterWorking);
	float getLastTemperatureReaded();
	float getTemperatureFermenterSetted();
	void setTemperatureFermenterSetted(bool tf);

	bool getIslessTemperature();


	float readSensorTemp();

  private:

	  bool startCircuit();

	  // stop bot circuit.

	//int _pinElectrovalveHot;
	//int _pinCircuitHot;  // activa la electrobomba para el ingreso de agua caliente (de red) y las llaves para que el agua circule.
//	int _pinColdPump; // activa la bomba de agua   PERO LA ACTIVA OTRA CLASE, PORQUE PUEDEN HABER VARIOS FERMENTADORES ENFRIANDO A LA VEZ.

	int _pinTermometer;  // para leer el termometro
	int _pinElectrovalveCold;

	float _timeValveWorking; // guarda el tiempo que estuvo enfriando, al apagarse la valvula de frio, esto se setea a 0
	bool _isCircuitEnabled;
	float _totalTimeConnected;
	float _totalTimeInOneStep;
	bool _isFermenterWorking;
	float _lastTemperatureReaded;
	float _temperatureFermenterSetted;
	float _rangueValue = 2;

	bool _lessTemperature;




	OneWire *_oneWireObjeto;
	DallasTemperature *_dallasTemperature;
	





};
 
#endif