#include "VistaLCD.h"

VistaLCD::VistaLCD(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows){
  lcd = new LiquidCrystal_I2C(lcd_addr, lcd_cols, lcd_rows);
  lcd->init();
  lcd->backlight();
  
}

void  VistaLCD::mostrarDetalleFermentador(String nombre, bool FermentadorEncendido, float ultimaTemperatura, float temperaturaSeteada){
	lcd->setCursor(0,0);	
	lcd->print("                    ");
	lcd->setCursor(0,0);	
	lcd->print(nombre);
	lcd->print(" ");
	if (FermentadorEncendido){
		lcd->print("ON");
	} else {
		lcd->print("OFF");
	}
	lcd->setCursor(0,1);
	lcd->print("                    ");
	lcd->setCursor(0,1);
	lcd->print("UT:");
	lcd->print(ultimaTemperatura);
	lcd->print(" TS:");
	lcd->print(temperaturaSeteada);
}


void VistaLCD::editarCampoFermentador(String nombre, String campo){
    lcd->setCursor(0,0);	
	lcd->print("                    ");
	lcd->setCursor(0,0);	
	lcd->print(nombre);
	lcd->setCursor(0,1);
    lcd->print("                    ");
	lcd->setCursor(0,1);
	lcd->print(campo);
	//lcd->print(":: ");
	//lcd->print(valor);
}


void VistaLCD::detenerSistema(){
	lcd->setCursor(0,2);	
	lcd->print("                    ");
	lcd->setCursor(0,2);	
	lcd->print("Sistema ");
	lcd->setCursor(0,3);
	lcd->print("                    ");
	lcd->setCursor(0,3);
	lcd->print("detenido");	
}
void VistaLCD::iniciarSistema(){
	//lcd->clear();
	lcd->setCursor(0,2);	
	lcd->print("                    ");
	lcd->setCursor(0,2);	
	lcd->print("Sistema ");
	lcd->setCursor(0,3);
	lcd->print("                    ");
	lcd->setCursor(0,3);
	lcd->print("en marcha");	
}

void VistaLCD::errorEnSistema(){
	lcd->setCursor(0,2);	
	lcd->print("                    ");
	lcd->setCursor(0,2);	
	lcd->print("Error en el sensor");
	lcd->setCursor(0,3);
	lcd->print("                    ");
	lcd->setCursor(0,3);
	lcd->print("de flujo.");	
	
}


	void VistaLCD::bienvenida(){
		lcd->clear();
			    lcd->print("       SKYNET ");

	    lcd->setCursor(0,1);
	    lcd->print("   Bienvenido!!!");
	    lcd->setCursor(0,2);
	    lcd->print(" Iniciando Sistema");
	    lcd->setCursor(0,3);
	    lcd->print(" Aguarde por favor");
	}



