#ifndef Fermentador_h
#define Fermentador_h

#include "Arduino.h"
#include <OneWire.h> 
#include <DallasTemperature.h>
class Fermentador {

public:
    Fermentador(String name, int pinTermometer, int pinElectrovalveCold,  int temperatureFermenterSetted, bool isFermenterWorking, float timeConnected);
	
	void ParadaDeEmergencia(bool parar); // cuando el sistema se detiene o se edita un fermentador se detienen todos los fermentadores. 
	float ReadTemperature();
	void OpencloseValve(bool open);
	bool reviewFermenterStatus();  // verifica temperatura y si tiene que abrir o cerrar valvula.
	
    bool isFermentadorEncendido();
	void setisFermentadorEncendido(bool FE);
	bool isCircuitoEnfriadorEncendido();
	void setisCircuitoEnfriadorEncendido(bool fermenterWorking);
	float getLastTemperatureReaded();
	void setLastTemperatureReaded(float);
	float getTemperatureFermenterSetted();
	void setTemperatureFermenterSetted(float tf);
	float getRango();
	String getNombre();
	void restartFermentador();
private:


	///// GETTER AND SETTER

	
	
	bool paradaDeEmergencia = false;






    String _nombre;
	int _pinTermometer;  // para leer el termometro
	int _pinElectrovalveCold;
	float _timeValveWorking; // guarda el tiempo que estuvo enfriando, al apagarse la valvula de frio, esto se setea a 0
	bool _isFermentadorEncendido;
	bool _isCircuitoEnfriadorEncendido;
	float _lastTemperatureReaded;
	float _temperatureFermenterSetted;
	float _rango = 2;	
		float _totalTimeConnected;

	
    OneWire *_oneWireObjeto;
	DallasTemperature *_dallasTemperature;

};

#endif // fermentador_h
