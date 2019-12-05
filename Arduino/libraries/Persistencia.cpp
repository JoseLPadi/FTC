#include "Persistencia.h"

Persistencia::Persistencia(){

}


	void Persistencia::setTemperaturaSeteada(int position, float temperaturaSeteada){
		int newTemperaturaSeteada = (temperaturaSeteada * 10);
		byte menosSignificativo=(newTemperaturaSeteada & 0xFF);
		byte masSignificativo =((newTemperaturaSeteada >> 8) & 0xFF);
		
		int addr = getRootAddress(position);
		EEPROM.update(addr, masSignificativo);
		EEPROM.update(addr+1, menosSignificativo);
		Serial.println("setTemperaturaSeteada");
		//EEPROM.commit()
	}
	float Persistencia::readTempearutraSteada(int position){
		int addr = getRootAddress(position);
		byte newVal = EEPROM.read(addr);
		
		int nro = newVal*256;
		nro = nro + (byte)EEPROM.read(addr+1);
		return (float)nro/10;
	}
	
	void Persistencia::setFermentadorEncendido(int position, boolean fermentadorEncendido){
		
		int addr = getRootAddress(position) + 2;
		EEPROM.update(addr, fermentadorEncendido);
				Serial.println("setFermentadorEncendido");

	}
	boolean Persistencia::getFermentadorEncendido(int position){
		int addr = getRootAddress(position) + 2;
		return EEPROM.read(addr);
	}
	
	int Persistencia::getRootAddress(int position){
		return (position * bloqueFijo)  + offset;
	}


		
	void Persistencia::setSistemaEncendido(boolean sistemaEncendido){
				EEPROM.update(1, sistemaEncendido);

	}
	boolean Persistencia::getSistemaEncendido(){
		
				return EEPROM.read(1);

	}
	
	
	
	
	