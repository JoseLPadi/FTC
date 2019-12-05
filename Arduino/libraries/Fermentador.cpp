#include "Fermentador.h"

Fermentador::Fermentador(String name,int pinTermometer, int pinElectrovalveCold,  int temperatureFermenterSetted, bool FermentadorEncendido, float timeConnected){
    pinMode(pinElectrovalveCold, OUTPUT);
	_pinTermometer = pinTermometer;
	_pinElectrovalveCold = pinElectrovalveCold;

	_temperatureFermenterSetted = temperatureFermenterSetted;
	_isFermentadorEncendido = FermentadorEncendido;
	_totalTimeConnected = timeConnected;
	_oneWireObjeto = new OneWire(_pinTermometer);
	_dallasTemperature = new DallasTemperature(_oneWireObjeto);
	_dallasTemperature->begin();
	_isCircuitoEnfriadorEncendido = false;
	//_lastTemperatureReaded = readSensorTemp();
	_nombre = name;
	
	
}
void Fermentador::ParadaDeEmergencia(bool parar){
	//paradaDeEmergencia = parar;
	if (parar){
		OpencloseValve(false);
	}
}
bool Fermentador::reviewFermenterStatus(){
     if (paradaDeEmergencia  || !_isFermentadorEncendido ) return false;   // no se puede ejecutar esta funcion
	
	float temp = ReadTemperature();
		//Serial.println(temp);
		//Serial.println(_lastTemperatureReaded);

	if (isCircuitoEnfriadorEncendido()){
		if (getLastTemperatureReaded() < getTemperatureFermenterSetted()){ // la temperatura esta por debajo del rango
			OpencloseValve(false);
			return false;
		} else {
			return true;
		}
	} else {
		if (getLastTemperatureReaded() > getTemperatureFermenterSetted() + getRango()){
			OpencloseValve(true);
			return true;
			
		} else return false;
		
	}
}
float Fermentador::ReadTemperature(){  
    Serial.println("leo temperatura");
    _dallasTemperature->requestTemperatures();
    _lastTemperatureReaded = _dallasTemperature->getTempCByIndex(0);	
    return _lastTemperatureReaded;
}

void Fermentador::OpencloseValve(bool open){
	Serial.println("abrociero valvula");
	if (open){
		setisCircuitoEnfriadorEncendido(true);
		digitalWrite(_pinElectrovalveCold, HIGH);
	} else {
		digitalWrite(_pinElectrovalveCold, LOW);
		setisCircuitoEnfriadorEncendido(false);
	}
}


	bool Fermentador::isFermentadorEncendido(){
		return _isFermentadorEncendido;
	}
	void Fermentador::setisFermentadorEncendido(bool FE){
		_isFermentadorEncendido = FE;
		if (!FE){
		    OpencloseValve(false);
	    }
	}
	bool Fermentador::isCircuitoEnfriadorEncendido(){
		return _isCircuitoEnfriadorEncendido;
	}
	void Fermentador::setisCircuitoEnfriadorEncendido(bool fermenterWorking){
		_isCircuitoEnfriadorEncendido = fermenterWorking;
	}
	float Fermentador::getLastTemperatureReaded(){
		return _lastTemperatureReaded;
	}
	void Fermentador::setLastTemperatureReaded(float nt){
		_lastTemperatureReaded = nt;
	}
	float Fermentador::getTemperatureFermenterSetted(){
		return _temperatureFermenterSetted;
	}
	void Fermentador::setTemperatureFermenterSetted(float tf){
		_temperatureFermenterSetted = tf;
	}
	
	float Fermentador::getRango(){
		return _rango;
	}
	
	String Fermentador::getNombre(){
		return _nombre;
	}
	void Fermentador::restartFermentador(){
		
	}
	
	
	