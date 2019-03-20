/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
 
#include "Arduino.h"
#include "Fermenter.h"

 
Fermenter::Fermenter(int pintTermometer, int pinElectrovalveCold, int temperatureFermenterSetted, bool isFermenterWorking, float timeConnected){

	// _oneWireObjeto = new OneWire(pinTermometer);	
	//_sensor(oneWireObjeto);
	pinMode(pinElectrovalveCold, OUTPUT);
	_pinTermometer = pintTermometer;
	_pinElectrovalveCold = pinElectrovalveCold;

	_temperatureFermenterSetted = temperatureFermenterSetted;
	_isFermenterWorking = isFermenterWorking;
	_totalTimeConnected = timeConnected;
	_oneWireObjeto = new OneWire(_pinTermometer);
	_dallasTemperature = new DallasTemperature(_oneWireObjeto);
	_dallasTemperature->begin();
	_isCircuitEnabled = false;
	_lastTemperatureReaded = readSensorTemp();

}


//  rango = 2;     sensor 20       temp seteada 17


bool Fermenter::startStopCircuit(){
	float tempReaded=readSensorTemp();
	_lessTemperature = _temperatureFermenterSetted > tempReaded;
	if (_temperatureFermenterSetted + _rangueValue < tempReaded){
		startCircuit();
	}
	else {
		stopCircuit();
	}
	return getIsCircuitEnabled();
}


//PUBLIC FUNCTIONS
bool Fermenter::startCircuit(){
		digitalWrite(_pinElectrovalveCold, HIGH);
		_isCircuitEnabled = true;
		return true;
}

void Fermenter::stopCircuit(){
	_isCircuitEnabled = false;
	digitalWrite(_pinElectrovalveCold, LOW);
}


float Fermenter::getDifferenceTemperature(){
	_lastTemperatureReaded = readSensorTemp();
	return (abs(_temperatureFermenterSetted - _lastTemperatureReaded));
}

//GETTER AND SETTER

float Fermenter::getTimeValveWorking(){
	return _timeValveWorking;
}
bool Fermenter::getIsCircuitEnabled(){
	return _isCircuitEnabled;
}
float Fermenter::getTotalTimeConnected(){
	return _totalTimeConnected;
}
float Fermenter::getTotalTimeInOneStep(){
	return _totalTimeInOneStep;
}
bool Fermenter::getIsFermenterWorking(){
	return _isFermenterWorking;
}
void Fermenter::setIsFermenterWorking(bool fermenterWorking){
	_isFermenterWorking = fermenterWorking;
}
float Fermenter::getLastTemperatureReaded(){
	return _lastTemperatureReaded;
}
float Fermenter::getTemperatureFermenterSetted(){
	return _temperatureFermenterSetted;
}
void Fermenter::setTemperatureFermenterSetted(bool tf){
	_temperatureFermenterSetted = tf;
	// cuando se setea una nueva temperatura, este valor pasa a 0 para poder registrar eltiempo en el que esta esta temperatura, esto se actualiza cada tanto.
	_totalTimeInOneStep = 0;
}


bool Fermenter::getIslessTemperature(){
	return _lessTemperature;
}









// PRIVATE FUNCTION

float Fermenter::readSensorTemp(){
	_lastTemperatureReaded = _dallasTemperature->getTempCByIndex(0);
	return _lastTemperatureReaded;
}












