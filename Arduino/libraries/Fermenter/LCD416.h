#ifndef LCD416_h
#define LCD416_h

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>





class LCD416  {
public:

	LCD416(LiquidCrystal_I2C i);
	void ShowLine(int fila, int columna, String texto);

	void ActualizarNombreYEncendido(String nombre, bool encendido);
	void ActualizarTSyTA(float ts, float ta);
	void actualizarTiempoEnTS(int horas);
	void actualizarBajadaDeTemperaturaAutomatica(bool bajarTemperaturaAutomatica);




private:

	LiquidCrystal_I2C *_lcd;  //


};

#endif