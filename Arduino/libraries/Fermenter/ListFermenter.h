#ifndef ListFermenter_h
#define ListFermenter_h

#include "Arduino.h"
#include "Fermenter.h"





class ListFermenter  {

public:
	ListFermenter();

	// Recorre el listado de fermentadores para ver si hay que habilitar/deshabilitar algun fermentador.
	// return true si hay algun circuito funcionando.
	bool enableDisableSomeFermenterCircuit();

	// devuelve true si la temperatura sensada es un grado menor a la temperatura que debería tener el fermentador.
	bool isOneFermenterWithLessTemperature();
	
	void stopAllFermenters();


	/*
	// esto es para obtener y guardar distintos valores de los fermentadores
	float getDifferenceTemperature(int fermenterNumber);

	*/


	bool getIsCircuitEnabledFromFermenterPosition(int fermenterNumber);
	bool getIsFermenterWorkingFromFermenterPosition(int fermenterNumber);
	void setIsFermenterWorkingFromFermenterPosition(int fermenterNumber, bool fermenterWorking);
	float getLastTemperatureReadedFromFermenterPosition(int fermenterNumber);
	float getTemperatureSettedFromFermenterPosition(int fermenterNumber);
	void  setTemperatureSettedFromFermenterPosition(int fermenterNumber, float TS);
	//float getTotalTimeConnectedFromFermenterPosition(int fermenterNumber);
	//float getTimeValveWorkingFromFermenterPosition(int fermenterNumber);
	float getTotalTimeInOneStepFromFermenterPosition(int fermenterNumber);


private:
	const int pinFlujoAgua = 2;
	const int pinBombaAgua = 23;
	const int pinSirena = 25;
	const int pinElectrobombaFermentador1 = 27;
	const int pinElectrobombaFermentador2 = 29;

	Fermenter *listFermenters[3];
	int totalFermenter;
	bool IsCircuitWorking;








	Fermenter getFermenter(int position);
	int getTotalFermenter();
};

#endif