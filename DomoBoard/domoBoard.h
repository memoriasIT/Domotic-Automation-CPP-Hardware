/*
 * domoBoard.h
 *
 *  Created on: 09/03/2015
 *      Author: jctejero
 *
 *      Modified: 12/04/2021
 */

#ifndef DOMOBOARD_H_
#define DOMOBOARD_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"Arduino.h"
#include	"../QueueList.h"
#include	"../debuglog.h"

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
	S_DIGITAL,
	S_ANALOGICO
}teSensor;

typedef struct
{
	byte	pin;
	byte	estado;
}TActuator, *TActuatorPtr;

typedef struct
{
	QueueList<TActuatorPtr>	Actuators;
}TActuatorLst;

typedef struct
{
	byte				pin;			//Pin asignado al sensor
	int	    			valor;			//Valor leido
	int					valor_Df;		//Valor Sensor por defecto
	bool				Activo;			//Activar/Desactivas sensor
	teSensor			eSensor;		//Tipos de sensor ANALÓGICO/DIGITAL
	int					Aux;			//Variables para ser usadas en la gestión del sensor
	char				*name;
	TActuatorLst		ActuatorsLst;
}TSensor, *TSensorPtr;

typedef void (*TSensorEvent)(TSensor *);

// DIMMER
typedef void (*TTimerEvent)(void);

typedef struct {
	TActuator			actuator;		// Actuador relacionado
	bool				enabled;		// Activado o no
	bool				pulso;			// Pulso activado
	uint32_t			delay;
	uint32_t			activationVal;
}TDimmer, *TDimmerPtr;


typedef struct
{
	TSensor				Sensor;
	TSensorEvent		SensorEvent;	//Evento para aplicación asociada
}PTSensor, *PTSensorPtr;

typedef enum {
	PER_STOP,
	PER_DOWN,
	PER_UP,
	PER_STOP2
}tsStaPer; //Control persianas

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define		FASE_P				2			// Detector de fase
#define 	BUTTON1_P   		3   	   	// Pulsador 1
#define 	RELE_P				4			// RELE 1
#define		PER_ONOFF_P			5			// ONOFF Persianas
#define		PER_UPDOWN_P		6			// UPDOWN Persianas
#define		TRIAC_P				7			// ON/OFF Solid State Triac
#define		DIMMER1_P			8			// Dimmer 1
#define		DIMMER2_P			9			// Dimmer 2
#define 	BUTTON2_P   		10        	// Pulsador 2
#define 	BTN_OPT_P    		11 	    	// Entrada Optocoplada
#define		EN_485				12			// Send/Recv a través del puerto RS485
#define     PIR_P				13			// Sensor de Movimiento

#define		PHOTORES_P			A0			// Photo resistor SRC
#define     GASMQ2_P			A1			// Sensor de gas MQ-2
#define		TEMPSEN_P			A2			// Sensor de temperatura
#define		POT1_P				A3			// Potenciómetro 1
#define		POT2_P				A4			// Potenciómetro 2
#define		PHOTOTTOR_P			A5			// Sensor de luminosidad (PhotoTransistor)


/****************************************************************************/
/***        Exported Class                                                ***/
/****************************************************************************/
class DomoBoard
{
private:


public:

	DomoBoard(); 						//Constructor

	PTSensor		BOTON1;
	PTSensor		BOTON2;
	PTSensor 		BTN_OPT;
	PTSensor		PIR_MOV;
	PTSensor		FASE;

	//Sensores Analógicos
	PTSensor		PHOTORES;			// Photoresistor SRC
	PTSensor		TEMPSEN;			//Sensor de temperatura
	PTSensor		PHOTOTTOR;			//Foto transistor
	PTSensor		POT1;				// Potenciómetro 1
	PTSensor		POT2;				// Potenciómetro 2
	PTSensor        GASMQ2;				// Sensor de Gas

	//Salidas
	TActuator		RELE;
	TActuator		TRIAC;

	TActuator		PER_ONOFF;
	TActuator       PER_UPDOWN;

	TDimmer			DIMMER1;
	TActuator		DIMMER1ACT;
	TDimmer			DIMMER2;
	TActuator		DIMMER2ACT;

	void 			leerAllSensor(void);
	void 			SetPersiana(tsStaPer staPer);

	static void 	leerSensor(PTSensorPtr Sensor);
	static void		setActuator(TActuator *Actuator, bool val);
	static void 	managerActuators(TActuatorLst *ActuatorsLst, bool val);
	static void 	enableSensor(TSensorPtr Sensor, bool enabled);
};

#ifndef SOLO_MODBUS
extern DomoBoard domoboard;
#endif


#endif /* DOMOBOARD_H_ */
