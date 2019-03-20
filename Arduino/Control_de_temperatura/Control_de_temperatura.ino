#include <OneWire.h>

#include <DallasTemperature.h>

#include <Fermenter.h>
#include <ListFermenter.h>
#include <LCD416.h>


// Modulo de apagado/encendido  del equipo
int buttonStartSystem = 20;
int buttonStopSystem = 21;
bool isSystemWorking = false;

// Cambiar al siguiente o anterior Fermentador en pantalla
int buttonFermentadorAnterior = 22;
int buttonFermentadorSiguiente = 23;

// Botones para editar info del mapa o para comenzar a editarlo
int buttonSubirValor = 24;
int buttonBajarValor = 25;
int buttonEditarSiguienteValorOComenarAEditarlo = 26;
bool estamosActualizandoUnFermentador;


int ListSensorTemperatureFermentedEnabled = -1;
ListFermenter *listFermenters;



//Fermenter ff(1, 2, 3, true, 2.2);

int ListSensorTemperaturePin = 0;
int ListSensorTemperatureTemperaureSeted = 18;
/* {PIN , TEMPERATURA SETEADA, ESTA HABILITADO*/
int ListSensorTemperature[][3] = {{50, 18, 1}, {51, 18, 1}, {52, 18, 0}, {53, 18, 0}, {54, 18, 0}, {55, 18, 0}, {56, 18, 0}};
int fermentadoresTotales = 2;
bool updatingSomeFermenter;

int fermenterUpdating ;




 LiquidCrystal_I2C *llccddd;  //

LCD416 *lcdlibrary;
void setup() {
  Serial.begin(2000000);
  Serial.println("adsfasdfasdfasdf");

  // Botones para iniciar/detener sistema.
  pinMode(buttonStartSystem, INPUT_PULLUP);
  pinMode(buttonStopSystem, INPUT_PULLUP);

  // Cambiar datos de Fermentador en pantalla
  pinMode(buttonFermentadorAnterior, INPUT_PULLUP);
  pinMode(buttonFermentadorSiguiente, INPUT_PULLUP);

  // Botones para editar info del mapa o para comenzar a editarlo
  pinMode(buttonSubirValor, INPUT_PULLUP);
  pinMode(buttonBajarValor, INPUT_PULLUP);
  pinMode(buttonEditarSiguienteValorOComenarAEditarlo, INPUT_PULLUP);

  //llccdd.init();

//listFermenters = new ListFermenter();
  llccddd = new LiquidCrystal_I2C(0x27, 20, 4);  //

   lcdlibrary = new LCD416(*llccddd);
   delay(1000);
   lcdlibrary->ActualizarNombreYEncendido("F" , true);
}

void loop() {

//  eventButtons();

  delay(1200);

  // Si el sistema esta funcionando y no estamos editando un fermentador, entonces podemos habilitar/deshabilitar algun fermentador.
  // Tambien podemos actualizar el estado de los fermentadores, lo ideal seria que esta funcion la podamos ejecutar cada 10 o 20 segundos.
  if (!estamosActualizandoUnFermentador) { //
//    if (isSystemWorking) {
   //   listFermenters->enableDisableSomeFermenterCircuit();
    }
 //  actualizarInfoFermentador();
//  }




}


// se llama siempre para cada boton.

long buttonTimer = 0;
long longPressTime = 250;
boolean buttonActive = false;
boolean longPressActive = false;
boolean longPressActiveToFinishEdit = false;
boolean buttonActiveToFinishEdit = false;
/*
void eventButtons() {

  if (!buttonActive) {
    int sensorVal = digitalRead(buttonStartSystem);
    if (sensorVal == HIGH && !isSystemWorking ) {
      isSystemWorking = true;
      // veo si debo hacer algo mas
    }

    sensorVal = digitalRead(buttonStopSystem);
    if (sensorVal == HIGH && !isSystemWorking ) {
      isSystemWorking = false;
      //Detengo todo el sistema.  SI O SI
      listFermenters.stopAllFermenters();
    }

    sensorVal = digitalRead(buttonFermentadorAnterior);
    if (sensorVal == HIGH && !estamosActualizandoUnFermentador ) {
      fermentadorAnterior();
    }
    sensorVal = digitalRead(buttonFermentadorSiguiente);
    if (sensorVal == HIGH && !estamosActualizandoUnFermentador ) {
      fermentadorSiguiente();
    }

    sensorVal = digitalRead(buttonSubirValor);
    if (sensorVal == HIGH && estamosActualizandoUnFermentador ) {
      actualizarValorDeFermentador(true);
      // subo el valor de algo y actualizo la pantalla.
    }

    sensorVal = digitalRead(buttonBajarValor);
    if (sensorVal == HIGH && estamosActualizandoUnFermentador ) {
      actualizarValorDeFermentador(false);
      // Bajo el valor de algo y actualizo la pantalla.
    }

  }
  if (!estamosActualizandoUnFermentador) {
    // Chequeamos si empiezo a editar un fermentador. en caso iniciar la edicion, paso al else hasta finalizar la edicion.

    if (digitalRead(buttonEditarSiguienteValorOComenarAEditarlo) == HIGH) {
      if (!buttonActive) {
        buttonActive = true;
        buttonTimer = millis();
      }
      if (((millis() - buttonTimer) > longPressTime) && (longPressActive == false)) {
        longPressActive = true;

        // entro a editar el fermentador seleccionado.  desde lcd tengo que ver como hacer para editr...
      }
    } else {

      buttonActive = false;
      longPressActive = false;
    }
  } else {  //Comienza o continua la edicion del Fermentador seleccionado.
    if (longPressActive) { //con esto deshabilito lo de comenzar a editar fermentador
      if (digitalRead(buttonEditarSiguienteValorOComenarAEditarlo) == LOW) {
        buttonActive = false;
        longPressActive = false;
      }

    } else {

      if (digitalRead(buttonEditarSiguienteValorOComenarAEditarlo) == HIGH) {   // para finializar edicion o cambiar a siguiente parametro

        if (buttonActiveToFinishEdit == false) {
          buttonActiveToFinishEdit = true;
          buttonTimer = millis();
        }
        if (((millis() - buttonTimer) > longPressTime) && (longPressActiveToFinishEdit == false)) {
          longPressActiveToFinishEdit = true;
          estamosActualizandoUnFermentador = false;

          //*** Finalizo la edicion del Fermentador

        }
      } else {

        if (longPressActiveToFinishEdit) { // se activo el long press, ahora los deshabilito
          longPressActiveToFinishEdit = false;
          buttonActiveToFinishEdit = false;

        } else {
          //***cambio a la siguiete opcion.


        }

        buttonActive = false;
        longPressActive = false;
      }


    }




  }

  // DEBO HACER ALGO CUANDO PRECIONA EL BOTON buttonEditarSiguienteValorOComenarAEditarlo


}

*/


// *******************************************************************************************************
//ACTUALIZO LA INFO EN LA PANTALLA DEL FERMENTADOR.
int fermentadorSeleccionado = 0;
void actualizarInfoFermentador() {
  Serial.println("actualizo fermentad");
//  Serial.println(listFermenters->getIsFermenterWorkingFromFermenterPosition(fermentadorSeleccionado));
  lcdlibrary->ActualizarNombreYEncendido("F" , true);

 // lcdlibrary->ActualizarNombreYEncendido("F" + String(fermentadorSeleccionado) , listFermenters->getIsFermenterWorkingFromFermenterPosition(fermentadorSeleccionado));
 /* lcdlibrary->ActualizarTSyTA(listFermenters->getTemperatureSettedFromFermenterPosition(fermentadorSeleccionado), listFermenters->getIsFermenterWorkingFromFermenterPosition(fermentadorSeleccionado));

  lcdlibrary->actualizarTiempoEnTS((int)listFermenters->getTotalTimeInOneStepFromFermenterPosition(fermentadorSeleccionado));
  lcdlibrary->actualizarBajadaDeTemperaturaAutomatica(false);
  //listFermenters.getTotalFermenter()*/


  
}
void fermentadorSiguiente() {
  fermentadorSeleccionado = fermentadorSeleccionado + 1;
  if (fermentadorSeleccionado == fermentadoresTotales)
    fermentadorSeleccionado = 0;
  actualizarInfoFermentador();
}
void fermentadorAnterior() {
  fermentadorSeleccionado = fermentadorSeleccionado + 1;
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






