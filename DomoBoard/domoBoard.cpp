/*
 * domoBoard.cpp
 *
 *      Author: jctejero
 *
  *      Modified: 12/04/2021
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "domoBoard.h"

/****************************************************************************/
/***                          PROGMEN Messages                            ***/
/****************************************************************************/
const char NAME_BTN1[] PROGMEM = {"BOTÓN 1"};
const char NAME_BTN2[] PROGMEM = {"BOTÓN 2"};
const char NAME_BTNOPT[] PROGMEM = {"Pulsador Optocoplado"};
const char NAME_PIR[] PROGMEM = {"PIR (Sensor de Movimiento)"};
const char NAME_POT1[] PROGMEM = {"POTENCIÓMETRO 1"};
const char NAME_POT2[] PROGMEM = {"POTENCIÓMETRO 2"};
const char NAME_PRES[] PROGMEM = {"PHOTORESISTOR"};
const char NAME_PTTOR[] PROGMEM = {"PHOTO TRANSISTOR (Op800WSL)"};
const char NAME_TEMP[] PROGMEM = {"SENSOR TEMPERATURA (TMP36)"};
const char NAME_GASMQ2[] PROGMEM = {"DETECTOR DE GAS MQ-2"};
const char NAME_FASE[] PROGMEM = {"DETECTOR DE FASE"};

// Constructors ////////////////////////////////////////////////////////////////

DomoBoard::DomoBoard()
{
	//Definimos pin's DomoBoard.

	BOTON1.Sensor.pin 			= BUTTON1_P;
	BOTON1.Sensor.valor 		= LOW;
	BOTON1.Sensor.valor_Df 		= LOW;
	BOTON1.Sensor.Activo 		= true;
	BOTON1.Sensor.eSensor  		= S_DIGITAL;
	BOTON1.Sensor.name			= (char *)NAME_BTN1;
	BOTON1.SensorEvent			= NULL;				//Inicializamos el evento del sensor para
													//evitar sensores descontrolados.
	pinMode(BUTTON1_P, INPUT);

	BOTON2.Sensor.pin 			= BUTTON2_P;
	BOTON2.Sensor.valor 		= LOW;
	BOTON2.Sensor.valor_Df 		= LOW;
	BOTON2.Sensor.Activo 		= true;
	BOTON2.Sensor.eSensor  		= S_DIGITAL;
	BOTON2.Sensor.name			= (char *)NAME_BTN2;
	BOTON2.SensorEvent			= NULL;
	pinMode(BUTTON2_P, INPUT);

	BTN_OPT.Sensor.pin 			= BTN_OPT_P;
	BTN_OPT.Sensor.valor 		= HIGH;
	BTN_OPT.Sensor.valor_Df		= HIGH;
	BTN_OPT.Sensor.Activo 		= true;
	BTN_OPT.Sensor.eSensor 		= S_DIGITAL;
	BTN_OPT.Sensor.name			= (char *)NAME_BTNOPT;
	BTN_OPT.SensorEvent			= NULL;
	pinMode(BTN_OPT_P, INPUT);

	//Inicialización del sensor de movimiento
	PIR_MOV.Sensor.pin 			= PIR_P;
	PIR_MOV.Sensor.Activo 		= true;
	PIR_MOV.Sensor.eSensor 		= S_DIGITAL;
	PIR_MOV.Sensor.name			= (char *)NAME_PIR;
	PIR_MOV.Sensor.valor		= LOW;
	PIR_MOV.Sensor.valor_Df		= LOW;  //HIGH; para el sensor original
	pinMode(PIR_P, INPUT);


	FASE.Sensor.pin			= FASE_P;
	FASE.Sensor.Activo 		= true;
	FASE.Sensor.eSensor 	= S_DIGITAL;
	FASE.Sensor.name		= (char *)NAME_FASE;
	FASE.SensorEvent		= NULL;
	FASE.Sensor.valor		= LOW;
	FASE.Sensor.valor_Df	= LOW;
	pinMode(FASE_P, INPUT);

	//==================================
	//Inicialización Sensores Analógicos
	//==================================
	POT1.Sensor.pin				= POT1_P;
	POT1.Sensor.Activo 			= true;
	POT1.Sensor.eSensor 		= S_ANALOGICO;
	POT1.Sensor.name			= (char *)NAME_POT1;
	POT1.SensorEvent			= NULL;

	POT2.Sensor.pin				= POT2_P;
	POT2.Sensor.Activo 			= true;
	POT2.Sensor.eSensor 		= S_ANALOGICO;
	POT2.Sensor.name			= (char *)NAME_POT2;
	POT1.SensorEvent			= NULL;

	PHOTORES.Sensor.pin			= PHOTORES_P;
	PHOTORES.Sensor.Activo 		= true;
	PHOTORES.Sensor.eSensor 	= S_ANALOGICO;
	PHOTORES.Sensor.name		= (char *)NAME_PRES;
	PHOTORES.SensorEvent		= NULL;

	TEMPSEN.Sensor.pin 			= TEMPSEN_P;
	TEMPSEN.Sensor.Aux			= -1;
	TEMPSEN.Sensor.Activo 		= true;
	TEMPSEN.Sensor.eSensor 		= S_ANALOGICO;
	TEMPSEN.Sensor.name			= (char *)NAME_TEMP;
	TEMPSEN.SensorEvent			= NULL;

	PHOTOTTOR.Sensor.pin		= PHOTOTTOR_P;
	PHOTOTTOR.Sensor.Activo 	= true;
	PHOTOTTOR.Sensor.eSensor 	= S_ANALOGICO;
	PHOTOTTOR.Sensor.name		= (char *)NAME_PTTOR;
	PHOTOTTOR.SensorEvent		= NULL;

	GASMQ2.Sensor.pin			= GASMQ2_P;
	GASMQ2.Sensor.Activo 		= true;
	GASMQ2.Sensor.eSensor 		= S_ANALOGICO;
	GASMQ2.Sensor.name			= (char *)NAME_GASMQ2;
	GASMQ2.SensorEvent			= NULL;



	//==========================
	//Inicialización Actuadores
	//==========================
	RELE.pin 					= RELE_P;
	RELE.estado 				= LOW;
	pinMode(RELE_P, OUTPUT);

	TRIAC.pin 					= TRIAC_P;
	TRIAC.estado 				= LOW;
	pinMode(TRIAC_P, OUTPUT);

	// PERSIANAS
	PER_ONOFF.pin				= PER_ONOFF_P;
	PER_ONOFF.estado			= LOW;
	pinMode(PER_ONOFF_P, OUTPUT);

	PER_UPDOWN.pin				= PER_UPDOWN_P;
	PER_UPDOWN.estado			= LOW;
	pinMode(PER_UPDOWN_P, OUTPUT);

	// DIMMERS
	DIMMER1ACT.estado 	= LOW;
	DIMMER1ACT.pin		= DIMMER1_P;
	DIMMER1.enabled		= true;
	DIMMER1.actuator	= DIMMER1ACT;
	DIMMER1.activationVal = 0;
	DIMMER1.delay		= 0;
	DIMMER1.pulso		= false;
	pinMode(DIMMER1_P, OUTPUT);

	DIMMER2ACT.estado 	= LOW;
	DIMMER2ACT.pin		= DIMMER2_P;
	DIMMER2.enabled		= true;
	DIMMER2.actuator	= DIMMER2ACT;
	DIMMER2.activationVal = 0;
	DIMMER2.delay		= 0;
	DIMMER2.pulso		= false;
	pinMode(DIMMER2_P, OUTPUT);

}

void DomoBoard::enableSensor(TSensorPtr Sensor, bool enabled){
	Sensor->Activo = enabled;

	if(!enabled)
		Sensor->valor = Sensor->valor_Df;
}

void DomoBoard::leerAllSensor(void){
	leerSensor(&BOTON1);
	leerSensor(&BOTON2);
	leerSensor(&BTN_OPT);
	leerSensor(&PIR_MOV);
}


void  DomoBoard::leerSensor(PTSensorPtr Sensor){
	int valor = 0;

	if(Sensor->Sensor.Activo){
		switch (Sensor->Sensor.eSensor)
		{
		case S_DIGITAL:
			valor = digitalRead(Sensor->Sensor.pin);
			break;

		case S_ANALOGICO:
			valor = analogRead(Sensor->Sensor.pin);
			break;
		}

		if(Sensor->Sensor.valor != valor)
		{
			Sensor->Sensor.valor = valor;

			//Si hay un cambio en el estado del sensor llamamos a la aplicación asociada
			if(Sensor->SensorEvent != NULL)
				Sensor->SensorEvent(&(Sensor->Sensor));
		}
	}
}

void  DomoBoard::setActuator(TActuatorPtr Actuator, bool val){
	digitalWrite(Actuator->pin, val);
	Actuator->estado = val;
}

void DomoBoard::managerActuators(TActuatorLst *ActuatorsLst, bool val){
	for(int n = 0; n < ActuatorsLst->Actuators.count(); n++){
		setActuator(ActuatorsLst->Actuators.peek(n), val);
	}
}

void 	DomoBoard::SetPersiana(tsStaPer staPer)
{
	switch(staPer){
	case PER_STOP:
	case PER_STOP2:
		setActuator(&PER_ONOFF, false);
		setActuator(&PER_UPDOWN, false);
		break;

	case PER_DOWN:
		setActuator(&PER_UPDOWN, false);
		setActuator(&PER_ONOFF, true);
		break;

	case PER_UP:
		setActuator(&PER_UPDOWN, true);
		setActuator(&PER_ONOFF, true);
		break;
	}
}


DomoBoard domoboard;

