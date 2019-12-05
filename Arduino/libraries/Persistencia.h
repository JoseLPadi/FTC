#ifndef Persistencia_h
#define Persistencia_h
#include "Arduino.h"
#include <EEPROM.h>


class Persistencia {

public:
    Persistencia();
	void setTemperaturaSeteada(int position, float temperaturaSeteada);
	float readTempearutraSteada(int position);
	
	void setFermentadorEncendido(int position, boolean fermentadorEncendido);
	boolean getFermentadorEncendido(int position);
	
	
	void setSistemaEncendido(boolean sistemaEncendido);
	boolean getSistemaEncendido();

private:

	int getRootAddress(int position);

	
	int bloqueFijo = 20;
	int offset = 24;
//EEPROM *lcd;


};

#endif // Persistencia_h
