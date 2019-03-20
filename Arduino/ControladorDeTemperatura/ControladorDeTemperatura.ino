 #include <OneWire.h>

#include <DallasTemperature.h>

#include <Fermenter.h>
#include <ListFermenter.h>
#include <LiquidCrystal_I2C.h>




//LiquidCrystal_I2C lcd(0x27, 20, 4);
LiquidCrystal_I2C lcd(0x3F, 16, 2);



/*CONFIGURACIÓN*/




typedef struct fermenter
{
  int pinSenor;
  int pinColdValve;
  bool isCircuitEnabled;
  long totalTimeFermenterWorking;
  long totalTimeInOneSteep;  //expresado en Dias
  bool autoBajarTemperatura;
  bool isFermenterWorking;
  float lastTemperatureReaded;
  float TemperatureFermenterSetted;
  float rangueValue;
  bool islessTemperature;
  OneWire *oneWire;
  DallasTemperature *sensorDS18B20;
};
/*
  /****************************************************************************************************************************/
/****************************************************************************************************************************/

/*Pines del relay
  VCC
  23  F6
  25  F5
  27  F4
  29  F3
  31  F2
  33  F1
  35  Sirena
  37  Bomba
  gnd*/
/*
   modulo 2 bootones
   10 y 11    B1
   11 y 13    B2
*/
/*Modulo 3 botones
  40 y 38 y 9  B3
*/

/*LED
  39 41*/

/*Sensor de fermentadores
  F1  3
  F2  4
  F3  5
  F4  6
  F5  7
  F6  8
*/
/*Controlador de flujo de agua
   2
*/

/*typedef struct fermenter
{
  int pinSenor;
  int pinColdValve;
  bool isCircuitEnabled;
  long totalTimeFermenterWorking;
  long totalTimeInOneSteep;  //expresado en Dias
  bool autoBajarTemperatura;
  bool isFermenterWorking;
  float lastTemperatureReaded;
  float TemperatureFermenterSetted;
  float rangueValue;
  bool islessTemperature;
  OneWire *oneWire;
  DallasTemperature *sensorDS18B20;
};*/
//Fermentadores y lista de fermentadores
const int fermentadoresTotales = 2;
fermenter  listFermenter [fermentadoresTotales] = {
  {3, 27, true, 0.0, 0.0, false, false, 000.0, 20.0, 5.2, false },
  {4, 29, false, 0.0, 0.0, true, true, 24.0, 19.0, 5.2, false}
  
};
const int pinFlujoAgua = 2;
const int pinBombaAgua = 23;
const int pinSirena = 25;
bool IsCircuitWorking;







/****************************************************************************************************************************/
/****************************************************************************************************************************/
// Modulo de apagado/encendido  del equipo
//        B1
int buttonStartSystem = 10;
int buttonStopSystem = 11;
bool isSystemWorking = true;
int ledSistemaEncendido = 41;
int ledSistemaApagado = 39;

// Cambiar al siguiente o anterior Fermentador en pantalla
//       B2
int buttonFermentadorAnterior = 13;
int buttonFermentadorSiguiente = 12;

// Botones para editar info del mapa o para comenzar a editarlo
//       B3
int buttonSubirValor = 9;
int buttonBajarValor = 40;
int buttonEditarSiguienteValorOComenarAEditarlo = 38;
bool estamosActualizandoUnFermentador = false;



long previousMillis = 0;



void setup() {
  Serial.begin(2000000);
  Serial.println("setup asldkjf");

  // inicio LCD
  lcd.init();
  lcd.backlight();

  for (int i = 0; i < fermentadoresTotales; i++) {
    listFermenter[i].oneWire = new OneWire(listFermenter[i].pinSenor);
    listFermenter[i].sensorDS18B20 = new DallasTemperature(listFermenter[i].oneWire);
    listFermenter[i].sensorDS18B20->begin();
  }


  pinMode(pinBombaAgua, OUTPUT);
  pinMode(pinSirena, OUTPUT);


  // Botones para iniciar/detener sistema.
  pinMode(buttonStartSystem, INPUT_PULLUP);
  pinMode(buttonStopSystem, INPUT_PULLUP);
  pinMode(ledSistemaEncendido, OUTPUT);
  pinMode(ledSistemaApagado, OUTPUT);


  // Cambiar datos de Fermentador en pantalla
  pinMode(buttonFermentadorAnterior, INPUT_PULLUP);
  pinMode(buttonFermentadorSiguiente, INPUT_PULLUP);

  // Botones para editar info del mapa o para comenzar a editarlo
  pinMode(buttonSubirValor, INPUT_PULLUP);
  pinMode(buttonBajarValor, INPUT_PULLUP);
  pinMode(buttonEditarSiguienteValorOComenarAEditarlo, INPUT_PULLUP);


  iniciarDetenersistema(true);
  LCDbienvenida();
  delay(1000);
}
bool puedoencenderFermentadores  = true;
long lastTimebomba;
long totalTimebomba = 600000;
long tiempobombaapagada = 180000;

long TiempoEsperaFermentadores = 8000;
long TiempoGuardadoDatos = 5000 ;  //debería ser cada 4hs = 4*60*60*1000 = 14.000.000
long temperaturaMinima = 4;


void loop() {
  eventButtons();

  // Si el sistema esta funcionando y no estamos editando un fermentador, entonces podemos habilitar/deshabilitar algun fermentador.
  // Tambien podemos actualizar el estado de los fermentadores, lo ideal seria que esta funcion la podamos ejecutar cada 10 o 20 segundos.

  long actualTime = millis();
  /*
  if (puedoencenderFermentadores && lastTimebomba+ totalTimebomba < actualTime){  
      stopAllFermenters();
      puedoencenderFermentadores= false;
      lastTimebomba = actualTime;
  } 
  if (!puedoencenderFermentadores && tiempobombaapagada+ totalTimebomba < actualTime){
      puedoencenderFermentadores= true;
      lastTimebomba = actualTime;
  }*/
  
  //TiempoEsperaFermentadores
  if  (puedoencenderFermentadores && previousMillis + TiempoEsperaFermentadores < actualTime) {
    previousMillis = millis();    
    if (!estamosActualizandoUnFermentador) { //
      if (isSystemWorking) {
        updateTemperatureOnEachFermenter();  // verifico si es necesario actualizar la temperatura de los fermentadores de forma automatica.
        enableDisableSomeFermenterCircuit();
      }
      actualizarInfoFermentador();
    }
  }
}


// se llama siempre para cada boton.

long buttonTimer = 0;
long longPressTime = 550;
boolean buttonActive = false;
boolean longPressActive = false;
boolean longPressActiveToFinishEdit = false;
boolean buttonActiveToFinishEdit = false;


long buttonbuttonFermentadorAnterior;
long buttonbuttonFermentadorSiguiente;
long buttonbuttonSubirValor;
long buttonbuttonBajarValor;
long buttonbuttonEditarSiguienteValorOComenarAEditarlo;
long actualTime;
void eventButtons() {
  // INICIA EL SISTEMA
  int sensorVal = digitalRead(buttonStartSystem);

  if (sensorVal == LOW && !isSystemWorking ) {
    iniciarDetenersistema(true);
  }
  sensorVal = digitalRead(buttonStopSystem);
  if (sensorVal == LOW ) {
      iniciarDetenersistema(false);      
  }

  sensorVal = digitalRead(buttonFermentadorAnterior);
  if (sensorVal == LOW && !estamosActualizandoUnFermentador ) {
    actualTime= millis(); 
    if ((actualTime - longPressTime) > buttonbuttonFermentadorAnterior){
      fermentadorAnterior();
      buttonbuttonFermentadorAnterior = actualTime;
    }
  } else {
      buttonbuttonFermentadorAnterior = 0;
  }

  sensorVal = digitalRead(buttonFermentadorSiguiente);
  if (sensorVal == LOW && !estamosActualizandoUnFermentador ) {
    actualTime= millis(); 
    if ((actualTime - longPressTime) > buttonbuttonFermentadorSiguiente){
      fermentadorSiguiente();
      buttonbuttonFermentadorSiguiente = actualTime;
    }
  } else {
      buttonbuttonFermentadorSiguiente = 0;
  }

  sensorVal = digitalRead(buttonSubirValor);
  if (sensorVal == LOW && estamosActualizandoUnFermentador ) {
        actualTime= millis(); 
    if ((actualTime - longPressTime) > buttonbuttonSubirValor){
      subirBajarValor(true);
      buttonbuttonSubirValor = actualTime;
    }
  } else {
      buttonbuttonSubirValor = 0;
  }

  sensorVal = digitalRead(buttonBajarValor);
  if (sensorVal == LOW && estamosActualizandoUnFermentador ) {
        actualTime= millis(); 
    if ((actualTime - longPressTime) > buttonbuttonBajarValor){
      subirBajarValor(false);
      buttonbuttonBajarValor = actualTime;
    }
  } else {
    buttonbuttonBajarValor = 0;
  }

  sensorVal = digitalRead(buttonEditarSiguienteValorOComenarAEditarlo);
  if (sensorVal == LOW) {
    actualTime= millis(); 
    if ((actualTime - longPressTime) > buttonbuttonEditarSiguienteValorOComenarAEditarlo){
      editarOnextOption();
      buttonbuttonEditarSiguienteValorOComenarAEditarlo = actualTime;
    }
  } else {
    buttonbuttonEditarSiguienteValorOComenarAEditarlo = 0;
  }
}


void iniciarDetenersistema(bool iniciar) {
  isSystemWorking = iniciar;
  if (iniciar) {
    Serial.println("INICIAMOS EL SISTEMA MOSTRAMOS LED ENCENDIDO");

    digitalWrite(ledSistemaEncendido, HIGH);
    digitalWrite(ledSistemaApagado, LOW);
    // veo si debo hacer algo mas  ES MUY PROBABLE QUE LAS OTRAS FUNCIONES ARRANQUEN SOLAS


  } else {
    Serial.println("DETENEMOS EL SISITEMA MOSTRAMOS LED APAGADO");

    digitalWrite(ledSistemaEncendido, LOW);
    digitalWrite(ledSistemaApagado, HIGH);
    stopAllFermenters();

  }

}

// *******************************************************************************************************
//ACTUALIZO LA INFO EN LA PANTALLA DEL FERMENTADOR.
int fermentadorSeleccionado = 0;
void actualizarInfoFermentador() {

  LCDLimpiarPantalla();

  LCDActualizarNombreYEncendido("T800 - " + String(fermentadorSeleccionado) , getIsFermenterWorkingFromFermenterPosition(fermentadorSeleccionado));
  LCDActualizarTSyTA(getTemperatureSettedFromFermenterPosition(fermentadorSeleccionado), getLastTemperatureReadedFromFermenterPosition(fermentadorSeleccionado));

//  LCDactualizarTiempoEnTS((int)getTotalTimeInOneStepFromFermenterPosition(fermentadorSeleccionado));
//  LCDactualizarBajadaDeTemperaturaAutomatica(FERMENTADORgetautoBajarTemperatura(fermentadorSeleccionado));

    Serial.println ("tiempoFermentadorfuncionando" + String((int)getTotalTimeInOneStepFromFermenterPosition(fermentadorSeleccionado)) + "   ///  " + String(FERMENTADORgetautoBajarTemperatura(fermentadorSeleccionado)));





}
void fermentadorSiguiente() {
  fermentadorSeleccionado = fermentadorSeleccionado + 1;
  if (fermentadorSeleccionado == fermentadoresTotales)
    fermentadorSeleccionado = 0;
  actualizarInfoFermentador();
}
void fermentadorAnterior() {
  fermentadorSeleccionado = fermentadorSeleccionado - 1;
  if (fermentadorSeleccionado < 0)
    fermentadorSeleccionado = fermentadoresTotales - 1;
  actualizarInfoFermentador();

}






















/*
   En un fermentador puedo editar diferentes opciones
   - Fermentador encendido apagado.
   - Temperatura Seteada
   - autobajar a 2 si/no  se activa automaticamente pasados 10 dias o cuando el usuario lo elije
*/




/*
  int actualizoOpcionSeleccionada = 0;
  const  int actualizoFermentadorEncendido = 0;
  const int actualizoTS = 1;
  const int actualizoAutoBajarA0 = 2;
  int actualizoMaxOption = 2;
  const float cadaCuantoAumento = 0.5;
  bool auxBool;
  float auxfloat;
  void actualizarValorDeFermentador(bool haciaArriba) {
  //dependiendo de lo que haga, subo o bajo un valor.  para numeros flotantes subo cada 0.5
  if (haciaArriba) {
    switch (actualizoOpcionSeleccionada) {
      case actualizoFermentadorEncendido:
        auxBool = listFermenters.getIsFermenterWorkingFromFermenterPosition(fermentadorSeleccionado);
        listFermenters.setIsFermenterWorkingFromFermenterPosition(fermentadorSeleccionado, !auxBool);


        break;
      case actualizoTS:
              auxfloat = listFermenters.getTemperatureSettedFromFermenterPosition(fermentadorSeleccionado);
        listFermenters.setTemperatureSettedFromFermenterPosition(fermentadorSeleccionado, auxfloat + cadaCuantoAumento);

        // statements
        break;
      default:  //actualizoAutoBajarA0
        // statements
        break;
    }
  } else {
    switch (actualizoOpcionSeleccionada) {
      case actualizoFermentadorEncendido:
        // statements
        break;
      case actualizoTS:
        // statements
        break;
      default:
        // statements
        break;
    }
  }

  }

*/



//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//         LCD COMMANDS


void LCDbienvenida() {
  // COLUMNA FILA

  LCDShowLineLCD(0, 0, "    Skynet V2.1     ");
  LCDShowLineLCD(1, 0, "    Bienvenido!     " );
  LCDShowLineLCD(2, 0, "    control de     ");
  LCDShowLineLCD(3, 0, "    Temperatura  ");

  // ActualizarNombreYEncendido( "asdfasdf", true);

}

void LCDShowLineLCD(int fila, int columna, String texto) {
  lcd.setCursor(columna, fila);
  // enmascaro el string a 16caracteres asi borro todo
  /*  while (texto.length() < 16){
      texto = texto + " ";
    }*/
  lcd.print(texto);
}

void LCDActualizarNombreYEncendido(String nombre, bool encendido) {
  if (encendido)
    nombre = nombre + "  ENCENDIDO";
  else
    nombre = nombre + "  APAGADO";
  LCDShowLineLCD(0, 0, nombre);
}

void LCDActualizarTSyTA(float ts, float ta) {
  String texto = "TS:" + String(ts) + " TA:" + String(ta);
  LCDShowLineLCD(1, 0, texto);
}

void LCDactualizarTiempoEnTS(int horas) {
  int dias = 2;

  String LCDtexto = String(dias) + "D_" + String(horas) + "H_ en TS";
  LCDShowLineLCD(2, 0, LCDtexto);
}

void LCDactualizarBajadaDeTemperaturaAutomatica(bool bajarTemperaturaAutomatica) {
  if (bajarTemperaturaAutomatica) {
    LCDShowLineLCD(3, 0, "AutoBajarA2 SI");
  }
  else {
    LCDShowLineLCD(3, 0, "AutoBajarA2 NO");
  }
}

void LCDLimpiarPantalla() {
  lcd.clear();
}


void LCDMostrarEditarValor(String Value) {
  //void LCDShowLineLCD(int fila, int columna, String texto) {

  LCDLimpiarPantalla();
  LCDShowLineLCD(0, 0, "Editando T800 - " + String(fermentadorSeleccionado));
  // LCDShowLineLCD(1, 0, "");
  LCDShowLineLCD(2, 0, Value );
  // LCDShowLineLCD(3, 0, "" );


}







//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//        FERMENTADOR

bool FERMENTADORstartStopCircuit(int posicion) {
  bool circuitEnabled = FERMENTADORgetIsCircuitEnabled(posicion);
  
  listFermenter[posicion].lastTemperatureReaded = FERMENTADORreadSensorTemp(posicion);
  listFermenter[posicion].islessTemperature = listFermenter[posicion].TemperatureFermenterSetted > listFermenter[posicion].lastTemperatureReaded;

  if (circuitEnabled){
    // lo dejo encendido hasta que llega a la temperatura sin el valor del rango
    if (listFermenter[posicion].TemperatureFermenterSetted > listFermenter[posicion].lastTemperatureReaded) {
      FERMENTADORstopCircuit(posicion);
    }
  } else { // si no esta encendido
    // lo tengo que encender cuando la temperatura es mayor a la temperatura mas el rango.
    if (listFermenter[posicion].TemperatureFermenterSetted + listFermenter[posicion].rangueValue < listFermenter[posicion].lastTemperatureReaded) {
      FERMENTADORstartCircuit(posicion);
    } 
  }


 /* if (!listFermenter[posicion].isCircuitEnabled ) {
    FERMENTADORstartCircuit(posicion);
  }
  else {
    FERMENTADORstopCircuit(posicion);
  }*/
  return FERMENTADORgetIsCircuitEnabled(posicion);
}





//PUBLIC FUNCTIONS
bool FERMENTADORstartCircuit(int posicion) {
  digitalWrite(listFermenter[posicion].pinColdValve, HIGH);
  listFermenter[posicion].isCircuitEnabled = true;
  return true;
}

void FERMENTADORstopCircuit(int posicion) {
  listFermenter[posicion].isCircuitEnabled = false;
  digitalWrite(listFermenter[posicion].pinColdValve, LOW);
}


float FERMENTADORgetDifferenceTemperature(int posicion) {
  listFermenter[posicion].lastTemperatureReaded = FERMENTADORreadSensorTemp(posicion);
  return (abs(listFermenter[posicion].TemperatureFermenterSetted - listFermenter[posicion].lastTemperatureReaded));
}

//GETTER AND SETTER
/*
  float getTimeValveWorking(int posicion){
  return listFermenter[posicion]._timeValveWorking;
  }*/

bool FERMENTADORgetIsCircuitEnabled(int posicion) {
  return listFermenter[posicion].isCircuitEnabled;
}
/*
  float getTotalTimeConnected(int posicion){
  return listFermenter[posicion]._totalTimeConnected;
  }*/
long FERMENTADORgetTotalTimeInOneStep(int posicion) {
  return listFermenter[posicion].totalTimeInOneSteep;
}
bool FERMENTADORgetIsFermenterWorking(int posicion) {
  return listFermenter[posicion].isFermenterWorking;
}
void FERMENTADORsetIsFermenterWorking(int posicion, bool fermenterWorking) {
  listFermenter[posicion].isFermenterWorking = fermenterWorking;
}
float FERMENTADORgetLastTemperatureReaded(int posicion) {
  return listFermenter[posicion].lastTemperatureReaded;
}
float FERMENTADORgetTemperatureFermenterSetted(int posicion) {
  return listFermenter[posicion].TemperatureFermenterSetted;
}
void FERMENTADORsetTemperatureFermenterSetted(int posicion, float tf) {
  listFermenter[posicion].TemperatureFermenterSetted = tf;
  // cuando se setea una nueva temperatura, este valor pasa a 0 para poder registrar eltiempo en el que esta esta temperatura, esto se actualiza cada tanto.
  listFermenter[posicion].totalTimeInOneSteep = 0;
}

bool FERMENTADORgetautoBajarTemperatura(int posicion) {
  return listFermenter[posicion].autoBajarTemperatura;
}
void FERMENTADORsetautoBajarTemperatura(int posicion, bool autobajar) {
  listFermenter[posicion].autoBajarTemperatura = autobajar;
}


long FERMENTADORgettotalTimeFermenterWorking(int posicion) {
  return listFermenter[posicion].totalTimeFermenterWorking;
}
void FERMENTADORsettotalTimeFermenterWorking(int posicion, long tt) {
  listFermenter[posicion].totalTimeFermenterWorking = tt;
}

bool FERMENTADORgetIslessTemperature(int posicion) {
  return listFermenter[posicion].islessTemperature;
}

void FERMENTADORrestartFermentador(int posicion) {


  //vuelvo todo a 0
}


// PRIVATE FUNCTION

float FERMENTADORreadSensorTemp(int posicion) {

  listFermenter[posicion].sensorDS18B20->requestTemperatures();
  float val = listFermenter[0].sensorDS18B20->getTempCByIndex(0);
  listFermenter[posicion].lastTemperatureReaded = listFermenter[posicion].sensorDS18B20->getTempCByIndex(0);
  return listFermenter[posicion].lastTemperatureReaded;
}





//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//        LISTADO DE FERMENTADORES

//DEVUELVE TRUE SI HAY ALGUN CIRCUITO ANDANDO
bool enableDisableSomeFermenterCircuit() {
  bool isWorking = false;
  for (int posicion = 0; posicion < fermentadoresTotales; posicion++) {
    Serial.println ("checkeando posicion " + String(posicion));
    if (getIsFermenterWorkingFromFermenterPosition(posicion))  // Verifico si el fermentador esta encendido, osea que si esta un fermentador conectado ahi
      isWorking = FERMENTADORstartStopCircuit(posicion) || isWorking  ;
  }
  IsCircuitWorking = isWorking;
  if (isWorking) {
    Serial.println("enciendo bomba");
    digitalWrite(pinBombaAgua, HIGH);
  }
  else {
    Serial.println("apago bomba");
    digitalWrite(pinBombaAgua, LOW);
  }

  //encender bomba
  return isWorking;
}


bool isOneFermenterWithLessTemperature() {
  for (int i = 0; i < fermentadoresTotales; i++) {
    if (FERMENTADORgetIslessTemperature(i)) {
      return true;
    }
  }
  return false;
}


void stopAllFermenters() {
  for (int i = 0; i < fermentadoresTotales; i++) {
    FERMENTADORstopCircuit(i);
  }
  digitalWrite(pinBombaAgua, LOW);

}

//Actualizacion del fermentador en una posicion.

bool getIsCircuitEnabledFromFermenterPosition(int fermenterNumber) {

  return FERMENTADORgetIsCircuitEnabled(fermenterNumber);
}

bool getIsFermenterWorkingFromFermenterPosition(int fermenterNumber) {

  return FERMENTADORgetIsFermenterWorking(fermenterNumber);
}
void setIsFermenterWorkingFromFermenterPosition(int fermenterNumber, bool fermenterWorking) {
  FERMENTADORsetIsFermenterWorking(fermenterNumber, fermenterWorking);
}

float getLastTemperatureReadedFromFermenterPosition(int fermenterNumber) {
  return FERMENTADORgetLastTemperatureReaded(fermenterNumber);
}

float getTemperatureSettedFromFermenterPosition(int fermenterNumber) {
  return FERMENTADORgetTemperatureFermenterSetted(fermenterNumber);
}

void  setTemperatureSettedFromFermenterPosition(int fermenterNumber, float TS) {
  FERMENTADORsetTemperatureFermenterSetted(fermenterNumber, TS);
}

float getTotalTimeInOneStepFromFermenterPosition(int fermenterNumber) {
  return FERMENTADORgetTotalTimeInOneStep(fermenterNumber);
}

void setTotalTimeInOneStepFromFermenterPosition(int fermenterNumber, long time) {
  return FERMENTADORgetTotalTimeInOneStep(fermenterNumber);
}








//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//        EDICION DE FERMENTADORES

typedef struct setOptions
{
  int tempFermentador;
  int habilitarDeshabilitar;
  int reiniciarFermentador;
  int necesitoAutobajarTemp;
  int enCuantoTiempoAutobajarTemperatura;




  int totalOptions;
};

char *pointer[] = { "Temperatura: ", "Encender: ",  "Reiniciar: ", "AutoBajar: ", "TiempoAutoBajar: "};
constexpr  setOptions settingsOptions = {
  0, 1, 2, 3, 4, 5
};

int actualOption = 0;

void editarOnextOption() {


  if (estamosActualizandoUnFermentador) {

    if (actualOption + 1 == settingsOptions.totalOptions ) {
      finalizarEdicion();
    } else {
      actualOption++;

      actualizarPantallaEdicion();
    }
  } else {
    estamosActualizandoUnFermentador = true;
    actualOption = 0;
    actualizarPantallaEdicion();
    stopAllFermenters();

  }

}

void actualizarPantallaEdicion() {
  int valorActualint;
  bool valorActualbool;
  float valorActualfloat;
  switch (actualOption) {
    case settingsOptions.tempFermentador:
      valorActualfloat  = FERMENTADORgetTemperatureFermenterSetted(fermentadorSeleccionado);
      LCDMostrarEditarValor(pointer[actualOption] +  String(valorActualfloat));

      break;
    case settingsOptions.habilitarDeshabilitar:
      valorActualbool = FERMENTADORgetIsFermenterWorking(fermentadorSeleccionado);
      LCDMostrarEditarValor(pointer[actualOption] +  String(valorActualbool));

      break;
    case settingsOptions.reiniciarFermentador:   //FERMENTADORrestartFermentador(int posicion)
      LCDMostrarEditarValor(pointer[actualOption] );


      break;
    case settingsOptions.necesitoAutobajarTemp:
      valorActualbool = FERMENTADORgetautoBajarTemperatura(fermentadorSeleccionado);
      LCDMostrarEditarValor(pointer[actualOption] +  String(valorActualbool));

      break;
    default:
      valorActualint = 13;
      LCDMostrarEditarValor(pointer[actualOption] +  String(valorActualint));

      break;
  }
}

void finalizarEdicion() {
  estamosActualizandoUnFermentador = false;
  actualizarInfoFermentador();
}


void subirBajarValor(bool toUP) {
  switch (actualOption) {
    case settingsOptions.tempFermentador:    //void FERMENTADORsetTemperatureFermenterSetted(int posicion, float tf)
      //float FERMENTADORgetTemperatureFermenterSetted(int posicion) {
      if (toUP) {
        FERMENTADORsetTemperatureFermenterSetted(fermentadorSeleccionado, FERMENTADORgetTemperatureFermenterSetted(fermentadorSeleccionado) + 0.5);
      } else {
        FERMENTADORsetTemperatureFermenterSetted(fermentadorSeleccionado, FERMENTADORgetTemperatureFermenterSetted(fermentadorSeleccionado) - 0.5);
      }
      break;
    case settingsOptions.habilitarDeshabilitar:  //FERMENTADORsetIsFermenterWorking(int posicion, bool fermenterWorking)
      //bool FERMENTADORgetIsFermenterWorking(int posicion) {
      FERMENTADORsetIsFermenterWorking(fermentadorSeleccionado, !FERMENTADORgetIsFermenterWorking(fermentadorSeleccionado));
      break;
    case settingsOptions.reiniciarFermentador:   //FERMENTADORrestartFermentador(int posicion)
      FERMENTADORrestartFermentador(fermentadorSeleccionado);
      break;
    case settingsOptions.necesitoAutobajarTemp:  //FERMENTADORsetautoBajarTemperatura(int posicion, bool autobajar) {
      //bool FERMENTADORgetautoBajarTemperatura(int posicion) {
      FERMENTADORsetautoBajarTemperatura(fermentadorSeleccionado, !FERMENTADORgetautoBajarTemperatura(fermentadorSeleccionado));

      break;
    default:   //enCuantoTiempoAutobajarTemperatura
      if (toUP) {

      } else {

      }
      break;
  }

  actualizarPantallaEdicion();

}



//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//        AUTOACTUALIZAR TEMPERATURA DEL FERMENTADOR


long tiempoParaEmpezarAAutoActualizarFermentador = 518400000;     // Tiempo para empezar a autoactualizar temperatura en fermentador 6d*24h*60m*60s*1000ms
long cadaCuantoAutoActualizarFermentador = 43200000;  //tiempo para que cambie la temperatura   12h*60m*60s*1000ms
void updateTemperatureOnEachFermenter(){

  for (int posicion = 0; posicion < fermentadoresTotales; posicion++) {
    Serial.println ("enableDisableSomeFermenterCircuit checkeando posicion " + String(posicion));
    if (getIsFermenterWorkingFromFermenterPosition(posicion)){


      //// ACA ME FALTARIA VERIFICAR SI PUEDO ACTUALIZAR LA TEMPERATURA, QUE EL AUTO UPDATE ESTE HABILITADO
      
    //  long getTotalTimeInOneStepFromFermenterPosition(int fermenterNumber) {
    //millis()
    //long FERMENTADORgetTotalTimeInOneStep(int posicion) {
    //temperaturaMinima
    
//long FERMENTADORgettotalTimeFermenterWorking(int posicion)
      if (FERMENTADORgettotalTimeFermenterWorking(posicion) + tiempoParaEmpezarAAutoActualizarFermentador < millis()){
          if (FERMENTADORgetTotalTimeInOneStep(posicion) + cadaCuantoAutoActualizarFermentador < millis() ){
            
            if (FERMENTADORgetTemperatureFermenterSetted(posicion) >= temperaturaMinima ){
               FERMENTADORsetTemperatureFermenterSetted(posicion, FERMENTADORgetTemperatureFermenterSetted(posicion) -1);
            }
            // bajo la temperatura 1 grado en caso de que sea mayor a 5 grados.
            
          }
      }
      
      
    }
  }  
  
}


