#include <Fermentador.h>
#include <VistaLCD.h>
#include <Persistencia.h>

/*Pines del relay
  VCC
  23     SIRENA
  25     BOMBA AGUA
  27     F1
  29     F2
  33     F3
  31     F4
  35     F5
  37     F6
  gnd*/

  
Persistencia persistencia;

bool isSystemWorking = true;
bool estamosActualizandoUnFermentador = false;
bool laBombaDeAguaEstaEncendida = false;

const int pinFlujoAgua = 2;
const int pinBombaAgua = 23;

const int pinSirena = 25;
int ledSistemaEncendido = 41;
int ledSistemaApagado = 39;

//Fermentador fer(3,27, 20, true, 0.20);
//Fermentador fer1(4,29, 20, true, 0.20);
int fermentadoresTotales =6;
int fermentadorSeleccionado =0;
Fermentador *listFermentador[6];

VistaLCD *lcd;

/*SENSOR DE FLUJO*/
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int pinSensorFlujo = 2;    //SensorFLUJO conectado en el pin 2
float factor_conversion = 7.5; //para convertir de frecuencia a caudal

volatile uint32_t ul_contador = 0;
ISR(TIMER4_COMPA_vect){
  Serial.println("isr");
 
       // PORTB = PORTB xor ( 1<< PORTB5);
 //   }
}

void InitTimer1(){
    cli();
    
  //  TCCR1A |=();
    OCR4AH = 0xff;
    OCR4AL = 0xc4;// 2500 
    TCCR4B |=(1<< WGM42 | 1<< CS42/* | 1<< CS11 | 1<< CS10*/);  // prescaler a 1024
    /************************
    b) Detalle el modo de funcionamiento del timer y la configuración del prescaler . 
        Configuración del timer:
          el timer es de 16 bits, 
          Modo de operación es WGM1:[0,1,0] CTC reinicia luego de que el counter sea igual al valor declarado en OCR1A.
        Configuración del prescaler:
          Prescaler seteado en CS0:[1,0,1] lo que significa que el timer avanza cada 1024 ciclos de reloj.
    *************************/ 
  
    TIMSK4=(1<< OCIE4A); //0x01;  
    /***********************
    c) Con la interrupción TOIE1  se genera una interrupcion cada vez que el timer counter llega al overflow REVISAR
        interrupcion se ejecuta a cada overflow.
    ************************/
  
    sei();
}

void setup() {
  Serial.begin(9600);
  Serial.println("arranca sistemaaaa");
  listFermentador[0] =new  Fermentador("t-800-1", 3,27, 14, false, 0.20);
  listFermentador[1] =new  Fermentador("t-800-2", 4,29, 12, false, 0.20);
  listFermentador[2] =new  Fermentador("t-800-3", 5,33, 12, false, 0.20);
  listFermentador[3] =new  Fermentador("t-800-4", 6,31, 12, false, 0.20);
  listFermentador[4] =new  Fermentador("t-800-5", 7,35, 12, false, 0.20);
  listFermentador[5] =new  Fermentador("t-800-6", 8,37, 12, false, 0.20);

  isSystemWorking = persistencia.getSistemaEncendido();
  for (int i =0; i<fermentadoresTotales; i++){
   // guardarFermentadorEnMemoria(i);

    listFermentador[i]->setTemperatureFermenterSetted(persistencia.readTempearutraSteada(i));
    listFermentador[i]->setisFermentadorEncendido(persistencia.getFermentadorEncendido(i));
  }

  lcd = new VistaLCD(0x27, 20, 4);
  lcd->bienvenida();
  InicializarBotonera();

  pinMode(ledSistemaEncendido, OUTPUT);
  pinMode(ledSistemaApagado, OUTPUT);
  pinMode(pinBombaAgua, OUTPUT);
  pinMode(pinSirena, OUTPUT);

    /*SENSOR DE FLUJOS */
  pinMode(pinSensorFlujo, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinSensorFlujo), ContarPulsos, RISING); //(Interrupcion 0(Pin2),funcion,Flanco de subida)
  
  InitTimer1();
delay(3000);
  iniciarDetenersistema(isSystemWorking);
      ActualizarFermentadores();
      LCDactualizarInfoFermentador();
}
long TiempoEsperaFermentadores = 8000;
long previousMillisEsperaFermentadores = 0;

long tiempoEsperaSensorFlujo = 3000;
long previousMillisEsperaSensorFlujo = 0;

void loop() {
 //ObtenerFrecuencia();
  long actualTime = millis();
  if  (previousMillisEsperaFermentadores + TiempoEsperaFermentadores < actualTime   && isSystemWorking && !estamosActualizandoUnFermentador) {
      previousMillisEsperaFermentadores = millis();
      ActualizarFermentadores();
      LCDactualizarInfoFermentador();
  }
  if (laBombaDeAguaEstaEncendida){
    if  (previousMillisEsperaSensorFlujo + tiempoEsperaSensorFlujo < actualTime   && isSystemWorking) {
        previousMillisEsperaSensorFlujo = millis();
        int litrosMinuto = ObtenerFrecuencia();
        Serial.println(litrosMinuto);
        if (litrosMinuto ==0){  //no hay flujo de agua
            encenderSirenaYDetenerSistema();
        }
    }
  } else { previousMillisEsperaSensorFlujo = millis();}

    CheckeoBotones();
    sei();
}

void LCDactualizarInfoFermentador(){
    lcd->mostrarDetalleFermentador(listFermentador[fermentadorSeleccionado]->getNombre(), listFermentador[fermentadorSeleccionado]->isFermentadorEncendido(), 
                                   listFermentador[fermentadorSeleccionado]->getLastTemperatureReaded() , listFermentador[fermentadorSeleccionado]->getTemperatureFermenterSetted());
}

void ActualizarFermentadores(){
  boolean cambiarEstadoBomba = false;
  boolean aux = false;
  for (int i =0; i<fermentadoresTotales; i++){
    Fermentador* f = listFermentador[i];
      aux = f->reviewFermenterStatus();
      cambiarEstadoBomba = cambiarEstadoBomba ||aux;
  }
    encenderBomba(cambiarEstadoBomba);
}

void DetenerFermentadores(){
  for (int i =0; i<fermentadoresTotales; i++){
    listFermentador[i]->ParadaDeEmergencia(true);
  }
  encenderBomba(false);
}

void encenderBomba(boolean cambiarEstadoBomba){
  laBombaDeAguaEstaEncendida = cambiarEstadoBomba;
    if (cambiarEstadoBomba){
      digitalWrite(pinBombaAgua, HIGH);
      
    } else {
      digitalWrite(pinBombaAgua, LOW);
    }
          
}

































//*******************************************************************************************************************/
/* LOGICA DE LA BOTONERA */
long buttonbuttonFermentadorAnterior;
long buttonbuttonFermentadorSiguiente;
long buttonbuttonSubirValor;
long buttonbuttonBajarValor;
long buttonbuttonEditarSiguienteValorOComenarAEditarlo;
long actualTime;
long longPressTime = 550;

int buttonStartSystem = 10;
int buttonStopSystem = 11;
int buttonFermentadorAnterior = 13;
int buttonFermentadorSiguiente = 12;
int buttonSubirValor = 9;
int buttonBajarValor = 40;
int buttonEditarSiguienteValorOComenarAEditarlo = 38;
void InicializarBotonera(){

      pinMode(buttonStartSystem, INPUT_PULLUP);
      pinMode(buttonStopSystem, INPUT_PULLUP);
      pinMode(buttonFermentadorAnterior, INPUT_PULLUP);
      pinMode(buttonFermentadorSiguiente, INPUT_PULLUP);
      pinMode(buttonSubirValor, INPUT_PULLUP);
      pinMode(buttonBajarValor, INPUT_PULLUP);
      pinMode(buttonEditarSiguienteValorOComenarAEditarlo, INPUT_PULLUP);
}

void CheckeoBotones(){
  int sensorVal = digitalRead(buttonStartSystem);

  if (sensorVal == LOW && !isSystemWorking ) {
    iniciarDetenersistema(true);
    //guardarIsSystemWorking();
    //encenderSirena(false);
  }
  sensorVal = digitalRead(buttonStopSystem);
  if (sensorVal == LOW && isSystemWorking) {
    iniciarDetenersistema(false);
   // guardarIsSystemWorking();
  }

  sensorVal = digitalRead(buttonFermentadorAnterior);
  if (sensorVal == LOW && !estamosActualizandoUnFermentador ) {
    actualTime = millis();
    if ((actualTime - longPressTime) > buttonbuttonFermentadorAnterior) {
      BotonFermentadorAnterior();
      buttonbuttonFermentadorAnterior = actualTime;
    }
  } else {
    buttonbuttonFermentadorAnterior = 0;
  }

  sensorVal = digitalRead(buttonFermentadorSiguiente);
  if (sensorVal == LOW && !estamosActualizandoUnFermentador ) {
    actualTime = millis();
    if ((actualTime - longPressTime) > buttonbuttonFermentadorSiguiente) {
      BotonFermentadorSiguiente();
      buttonbuttonFermentadorSiguiente = actualTime;
    }
  } else {
    buttonbuttonFermentadorSiguiente = 0;
  }

  sensorVal = digitalRead(buttonSubirValor);

  if (sensorVal == LOW && estamosActualizandoUnFermentador ) {
    actualTime = millis();
    if ((actualTime - longPressTime) > buttonbuttonSubirValor) {
      BotonSubirBajarValor(true);
      buttonbuttonSubirValor = actualTime;
    }
  } else {
    buttonbuttonSubirValor = 0;
  }

  sensorVal = digitalRead(buttonBajarValor);

  if (sensorVal == LOW && estamosActualizandoUnFermentador) {
    actualTime = millis();
    if ((actualTime - longPressTime) > buttonbuttonBajarValor) {
      BotonSubirBajarValor(false);
      buttonbuttonBajarValor = actualTime;
    }
  } else {
    buttonbuttonBajarValor = 0;
  }

  sensorVal = digitalRead(buttonEditarSiguienteValorOComenarAEditarlo);
  if (sensorVal == LOW) {
    actualTime = millis();
    if ((actualTime - longPressTime) > buttonbuttonEditarSiguienteValorOComenarAEditarlo) {
        editarSiguienteOpcion();
            delay(50);
        buttonbuttonEditarSiguienteValorOComenarAEditarlo = actualTime;
    }
  } else {
    buttonbuttonEditarSiguienteValorOComenarAEditarlo = 0;
  }
}

void BotonFermentadorSiguiente(){
    delay(30);
    Serial.println("BotonFermentadorSiguiente");
    fermentadorSeleccionado = fermentadorSeleccionado + 1;
    if (fermentadorSeleccionado == fermentadoresTotales)
        fermentadorSeleccionado = 0;
    LCDactualizarInfoFermentador();
}

void BotonFermentadorAnterior(){
    delay(50);
    Serial.println("BotonFermentadorAnterior");
    fermentadorSeleccionado = fermentadorSeleccionado - 1;
    if (fermentadorSeleccionado < 0)
        fermentadorSeleccionado = fermentadoresTotales - 1;
        LCDactualizarInfoFermentador();
}

void iniciarDetenersistema(bool iniciar) {
  isSystemWorking = iniciar;
      persistencia.setSistemaEncendido(iniciar);
  if (iniciar) {
    Serial.println("INICIAMOS EL SISTEMA MOSTRAMOS LED ENCENDIDO");

    digitalWrite(pinSirena, LOW);
    digitalWrite(ledSistemaEncendido, HIGH);
    digitalWrite(ledSistemaApagado, LOW);
    lcd->iniciarSistema();

  } else {
    Serial.println("DETENEMOS EL SISITEMA MOSTRAMOS LED APAGADO");
    DetenerFermentadores();
    digitalWrite(ledSistemaEncendido, LOW);
    digitalWrite(ledSistemaApagado, HIGH);
    lcd->detenerSistema();

  }

}

void BotonSubirBajarValor(bool tengoQueSubir){
    delay(50);
    subirBajarValor( tengoQueSubir);
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
  //int necesitoAutobajarTemp;
  //int enCuantoTiempoAutobajarTemperatura;
  int totalOptions;
};

//char *pointer[] = { (char*)"Temperatura: ", (char*)"Encender: ",  (char*)"Reiniciar: ", (char*)"AutoBajar: ", (char*)"TiempoAutoBajar: "};
char *pointer[] = { (char*)"Temp: ", (char*)"Encender: ", (char*) "Reiniciar: "};
constexpr  setOptions settingsOptions = {
  0, 1, 2, 3
};
int actualOption = 0;

void editarSiguienteOpcion() {
  if (estamosActualizandoUnFermentador) {
    if (actualOption + 1 == settingsOptions.totalOptions ) {
      finalizarEdicion();
      guardarFermentadorEnMemoria(fermentadorSeleccionado);
    } else {
      actualOption++;
      actualizarPantallaEdicion();
    }
  } else {
    estamosActualizandoUnFermentador = true;
    actualOption = 0;
    actualizarPantallaEdicion();
    DetenerFermentadores();
  }
}


void actualizarPantallaEdicion() {
  int valorActualint;
  bool valorActualbool;
  float valorActualfloat;
  switch (actualOption) {
    case settingsOptions.tempFermentador:
      valorActualfloat  = listFermentador[fermentadorSeleccionado]->getTemperatureFermenterSetted();
      LCDMostrarEditarValor(pointer[actualOption] +  String(valorActualfloat));

      break;
    case settingsOptions.habilitarDeshabilitar:
      valorActualbool =listFermentador[fermentadorSeleccionado]->isFermentadorEncendido();
      String encendido =  valorActualbool? "ON":"OFF";
      LCDMostrarEditarValor(pointer[actualOption] + encendido);

      break;
    case settingsOptions.reiniciarFermentador:   //FERMENTADORrestartFermentador(int posicion)
      LCDMostrarEditarValor(pointer[actualOption] );


      break;

  }
}

void finalizarEdicion() {
  estamosActualizandoUnFermentador = false;
  LCDactualizarInfoFermentador();
}


void subirBajarValor(bool toUP) {
  switch (actualOption) {
    case settingsOptions.tempFermentador:    //void FERMENTADORsetTemperatureFermenterSetted(int posicion, float tf)
      //float FERMENTADORgetTemperatureFermenterSetted(int posicion) {
      if (toUP) {
          listFermentador[fermentadorSeleccionado]->setTemperatureFermenterSetted(listFermentador[fermentadorSeleccionado]->getTemperatureFermenterSetted()+0.5);
        //FERMENTADORsetTemperatureFermenterSetted(fermentadorSeleccionado, FERMENTADORgetTemperatureFermenterSetted(fermentadorSeleccionado) + 0.5);
      } else {
         listFermentador[fermentadorSeleccionado]->setTemperatureFermenterSetted(listFermentador[fermentadorSeleccionado]->getTemperatureFermenterSetted()-0.5);
      //FERMENTADORsetTemperatureFermenterSetted(fermentadorSeleccionado, FERMENTADORgetTemperatureFermenterSetted(fermentadorSeleccionado) - 0.5);
      }
      break;
    case settingsOptions.habilitarDeshabilitar:  //FERMENTADORsetIsFermenterWorking(int posicion, bool fermenterWorking)
      //bool FERMENTADORgetIsFermenterWorking(int posicion) {
      listFermentador[fermentadorSeleccionado]->setisFermentadorEncendido(!listFermentador[fermentadorSeleccionado]->isFermentadorEncendido());
    //FERMENTADORsetIsFermenterWorking(fermentadorSeleccionado, !FERMENTADORgetIsFermenterWorking(fermentadorSeleccionado));
      break;
    case settingsOptions.reiniciarFermentador:   //FERMENTADORrestartFermentador(int posicion)
      //FERMENTADORrestartFermentador(fermentadorSeleccionado);
      listFermentador[fermentadorSeleccionado]->restartFermentador();
      break;
  }

  actualizarPantallaEdicion();

}

void guardarFermentadorEnMemoria(int fermentadorSeleccionado){
  persistencia.setTemperaturaSeteada(fermentadorSeleccionado, listFermentador[fermentadorSeleccionado]->getTemperatureFermenterSetted());
  persistencia.setFermentadorEncendido(fermentadorSeleccionado, listFermentador[fermentadorSeleccionado]->isFermentadorEncendido());
}

void LCDMostrarEditarValor(String Value) {
  lcd->editarCampoFermentador(listFermentador[fermentadorSeleccionado]->getNombre(), Value);
}

void encenderSirenaYDetenerSistema(){
  Serial.println("DETENGO EL SISTEMA");
  iniciarDetenersistema(false);
  digitalWrite(pinSirena, HIGH);
  lcd->errorEnSistema();
}

//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//****************************************************************************************//
//        CODIGO PARA EL SENSOR DE FLUJOS


//---Función que se ejecuta en interrupción---------------
void ContarPulsos ()
{
  NumPulsos++;  //incrementamos la variable de pulsos
}

//---Función para obtener frecuencia de los pulsos--------
int ObtenerFrecuencia(){
  
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  // interrupts();    //Habilitamos las interrupciones
  delay(500);   //muestra de 1 segundo
  //noInterrupts(); //Desabilitamos las interrupciones
  frecuencia = NumPulsos; //Hz(pulsos por segundo)
    float caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m

  Serial.println("DEVUELVO FRECUENCIA");
  Serial.println(caudal_L_m);

  return frecuencia;
}
