#ifndef VistaLCD_h
#define VistaLCD_h
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>


class VistaLCD {

public:
    VistaLCD(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
	void mostrarDetalleFermentador(String nombre, bool FermentadorEncendido, float ultimaTemperatura, float temperaturaSeteada);
	void editarCampoFermentador(String nombre, String campo);
	void iniciarSistema();
	void detenerSistema();
	void errorEnSistema();
	void bienvenida();
	
private:

LiquidCrystal_I2C *lcd;


};

#endif // VistaLCD_h
