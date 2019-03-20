/*
Morse.cpp - Library for flashing Morse code.
Created by David A. Mellis, November 2, 2007.
Released into the public domain.
*/

#include "Arduino.h"
#include "Fermenter.h"
#include "ListFermenter.h"





ListFermenter::ListFermenter(){

	IsCircuitWorking = false;
//Fermenter::Fermenter(int pintTermometer, int pinElectrovalveCold, int temperatureFermenterSetted, bool isFermenterWorking, float timeConnected){
	listFermenters[0] = new Fermenter(51, 31, 20, true, 2.2);
	listFermenters[1] = new Fermenter(52, 33, 20, true, 2.2);
	//listFermenters[2] = new Fermenter(1, 2, 3, true, 2.2);

	totalFermenter = 2;

	pinMode(pinBombaAgua, OUTPUT);
	pinMode(pinSirena, OUTPUT);




}

//DEVUELVE TRUE SI HAY ALGUN CIRCUITO ANDANDO
bool ListFermenter::enableDisableSomeFermenterCircuit(){
	bool isWorking = false;
	for (int i = 0; i < totalFermenter; i++){
		Fermenter aux = getFermenter(i);
		isWorking = isWorking || aux.startStopCircuit();
	}

	if (isWorking){
		digitalWrite(pinBombaAgua, HIGH);
	}
	else {
		digitalWrite(pinBombaAgua, LOW);
	}
	//encender bomba
	return isWorking;
}


bool ListFermenter::isOneFermenterWithLessTemperature(){
	for (int i = 0; i < totalFermenter; i++){
		Fermenter aux = getFermenter(i);
		if (aux.getIslessTemperature()){
			return true;
		}
	}
	return false;
}


void ListFermenter::stopAllFermenters(){
	for (int i = 0; i < totalFermenter; i++){
		Fermenter aux = getFermenter(i);
		aux.stopCircuit();
	}
}



Fermenter ListFermenter::getFermenter(int position){
	Fermenter f = *listFermenters[position];
	return f;
}
int ListFermenter::getTotalFermenter(){
	return 3;
}





/*****************************************************************************/
//Actualizacion del fermentador en una posicion.

bool ListFermenter::getIsCircuitEnabledFromFermenterPosition(int fermenterNumber){
	Fermenter aux = getFermenter(fermenterNumber);

	return aux.getIsCircuitEnabled();
}

bool ListFermenter::getIsFermenterWorkingFromFermenterPosition(int fermenterNumber){
	Fermenter aux = getFermenter(fermenterNumber);

	return aux.getIsFermenterWorking();
}
void ListFermenter::setIsFermenterWorkingFromFermenterPosition(int fermenterNumber, bool fermenterWorking){
	Fermenter aux = getFermenter(fermenterNumber);
	aux.setIsFermenterWorking(fermenterWorking);
}

float ListFermenter::getLastTemperatureReadedFromFermenterPosition(int fermenterNumber){
	Fermenter aux = getFermenter(fermenterNumber);
	return aux.getLastTemperatureReaded();
}
float ListFermenter::getTemperatureSettedFromFermenterPosition(int fermenterNumber){
	Fermenter aux = getFermenter(fermenterNumber);

	return aux.getTemperatureFermenterSetted();
}
void  ListFermenter::setTemperatureSettedFromFermenterPosition(int fermenterNumber, float TS){
	Fermenter aux = getFermenter(fermenterNumber);
	aux.setTemperatureFermenterSetted(TS);

}

float ListFermenter::getTotalTimeInOneStepFromFermenterPosition(int fermenterNumber){
	Fermenter aux = getFermenter(fermenterNumber);

	return aux.getTotalTimeInOneStep();
}


















