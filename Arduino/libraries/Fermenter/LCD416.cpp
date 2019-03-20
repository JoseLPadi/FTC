#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include "LCD416.h"






LCD416::LCD416(LiquidCrystal_I2C i){
// COLUMNA FILA
	_lcd = &i;
	_lcd->init();

	_lcd->setCursor(0, 0);
	_lcd->backlight();

	ShowLine(0, 0, "  O'Padi V1.0   ");
	ShowLine(1,0,"Bienvenido!" );
	ShowLine(2,0, "putamadre");
	ShowLine(3, 0, "Temperatura");
	ShowLine(3, 0, "Temperatura0");
	ShowLine(3, 0, "Temperatura01");
	ShowLine(3, 0, "Temperatura012");
	ShowLine(3, 0, "Temperatura0123");


	ActualizarNombreYEncendido( "asdfasdf", true);

}


void LCD416::ShowLine(int fila, int columna, String texto){
	_lcd->setCursor(columna, fila);
	// enmascaro el string a 16caracteres asi borro todo
/*	while (texto.length() < 16){
		texto = texto + " ";
	}*/
	_lcd->print(texto);
}

void LCD416::ActualizarNombreYEncendido(String nombre, bool encendido){
/*	if (encendido)
		nombre = nombre + "  ENCENDIDO";
	else 
		nombre = nombre + "  APAGADO";*/
	String paaaaa = "putamadre";
	_lcd->print(paaaaa);
}

void LCD416::ActualizarTSyTA(float ts, float ta){
	String texto = "TS:" + String(ts) + " TA:" + String(ta);
	ShowLine(1, 0, texto);
}

void LCD416::actualizarTiempoEnTS(int horas){
	int dias = 2;
	
	String texto = String(dias) + "D_" + String(horas) + "H_ en TS";
	ShowLine(2, 0, texto);
}

void LCD416::actualizarBajadaDeTemperaturaAutomatica(bool bajarTemperaturaAutomatica){
	if (bajarTemperaturaAutomatica){
		ShowLine(3, 0, "AutoBajarA2 SI");
	}
	else {
		ShowLine(3, 0, "AutoBajarA2 NO");
	}
}




