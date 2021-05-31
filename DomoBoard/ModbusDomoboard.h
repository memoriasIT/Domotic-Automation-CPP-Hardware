/*
 * ModbusDomoboard.h
 *
 *  Created on: 11 mar. 2020
 *      Author: jctej
 *
 *  modified on: 28/04/2021
 */

#ifndef DOMOBOARD_MODBUSDOMOBOARD_H_
#define DOMOBOARD_MODBUSDOMOBOARD_H_

/**********************************************************************/
/***                         Include Files                          ***/
/**********************************************************************/
#include "domoBoard.h"
#include "../ModBusSlave/ModBusSlave.h"

/**********************************************************************/
/***                         enums Definitions                      ***/
/**********************************************************************/
/* slave registers */

//Discrete Output Coils
enum {
	MB_RELE,
	MB_TRIAC,
	MB_ACTPIR,					//Salida virtual para Activar/Desactivar PIR
	MB_ACTSRC,					//Salida virtual para Activar/Desactivar SRC --> PhotoResistor
	MB_ACTTOR,					//Salida virtual para Activar/Desactivar Trigger --> PhotoTransistor
	MB_PERUP,					//Salida virtual para el control de la persiana UP
	MB_PERDOWN,					//Salida Virtual para el control de la persiana DOWN
	MB_KEYGAR,					//Salida Virtual para el accionado de la llave del garage
	MB_DIM1POT,
	MB_DIM2POT,
	MB_ACTDIM1,
	MB_ACTDIM2,
	MB_O_COILS
};

//Discrete Input Contacts
enum {
	MB_BOTON1,
	MB_BOTON2,
	MB_BTNOPT,
	MB_PIRMOV,
	MB_I_CONTATCS
};

//Registros ModBus para variables analógicas "Analog Output Holding Register"
enum{
	MB_SELPRACT,		// Registro ModBus Para seleccionar la configuración del sistema
	MB_TMP_PIR,			// Registro para controlar el tiempo activo del sensor PIR (Segundos)
	MB_SRC_HL,			// Registro para controlar el nivel superior de activación SRC
	MB_SRC_LL,			// Registro para controlar el nivel Inferior de activación SRC
	MB_TTOR_LL,			// Registro para controlar el nivel Inferior de activación del PHOTOTTOR
	MB_STAPER,		    // Estado Persiana
	MB_A_REGS			// Total de registros Analógicos
};

//Registros Analógicos de Entrada
enum{
	MB_POT1,			// POTENCIÓMETRO 1
	MB_POT2,			// POTENCIÓMETRO 2
	MB_PHOTORES,		// PhotoResistor
	MB_TEMPSEN,			// Sensor de Temperatura
	MB_PHOTOTTOR,		// Photo Transistor
	MB_GASMQ2,			// Detector fugas de gas
	MB_POSPER,			// Porcentage de apertura de la Persiana
	MB_DIM1,
	MB_DIM2,
	MB_I_REGS			// Total de registros de Entrada
};

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
//State Actuator
typedef enum{
	OFF,
	ON,
	TOGGLE
}TStateDigitalDev;

typedef struct
{
	TActuatorPtr	actuator;
	uint8_t			nMBReg;			//Posición registro para su uso en comunicaciones: Ej. ModBus
	uint16_t 		*bkMBRegs;		//Banco de registros asociado, para su uso en comunicaciones: Ej. ModBus
}TmbActuator, *TmbActuatorPtr;

typedef struct
{
	QueueList<TmbActuatorPtr>	mbActuators;
}TmbActuatorsLst;


// Dimmer
typedef struct
{
	TDimmerPtr		dimmer;			// Dimmer relacionado
	TmbActuator		mbActuator;
}TmbDimmer, *TmbDimmerPtr;


//Estructura para controlar los elementos/acciones Temporizadas.
typedef struct{
	uint32_t 	time_lastAct;		//instante de la última activación de la temporización
	uint16_t	Cte_Norma;			//Cte. usada normalizar el tiempo activo
	uint16_t	*time_activo;		//puntero al tiempo que dura la temporización
	bool		estado;				//Temporizador activo
}TctrlTemp, *TctrlTempPtr;

//Estructura para controlar los tiempos/porcentajes de activación (Persiana)
typedef struct{
	bool			activa;
	uint32_t 		time_lastAct;		//última actualización del tiempo
	uint16_t		*maxTime;			//puntero al tiempo máximo de activación
	int32_t			*actTime;			//puntero al tiempo actual de activación
}TctrlTime, *TctrlTimePtr;

//Estructura para usar niveles.
typedef struct{
	uint16_t	*lSup;					//puntero al valor del nivel superior
	uint16_t	*lInf;					//puntero al valor del nivel inferior
}TctrlLevel, *TctrlLevelPtr;

typedef struct{
	PTSensorPtr			Sensor;			//Sensor domoboard asociado
	uint8_t				nMBReg;			//Posición registro para su uso en comunicaciones: Ej. ModBus
	uint16_t 			*bkMBRegs;		//Banco de registros asociado, para su uso en comunicaciones: Ej. ModBus
	TmbActuatorsLst		ActuatorsLst;	//Listado de actuadores accionados por el sensor
	TctrlTempPtr		ctrlTempPtr;	//Para controlar si el evento asociado al sensor tiene un componente temporizado
	TctrlLevelPtr		ctrlLevelPtr;   //Estructura para controlar utilidades que requieran de niveles
	void				*RelatedSensors; //Lista de sensores relacionados para procesar el evento de sensor
}TmbSensor, *TmbSensorPtr;

typedef struct
{
	QueueList<TmbSensorPtr>	mbRelatedSensors;
}TmbRelatedSensors;


typedef void (*TmbSensorEvent)(TmbSensorPtr);

typedef struct
{
	TmbSensor			mbSensor;
	TmbSensorEvent		mbSensorEvent;	//Evento para aplicación asociada
}PTmbSensor, *PTmbSensorPtr;

/*************************************************************/
/***                   MACROS Definitions                  ***/
/*************************************************************/
#define hwpSensor(sn)			sn->Sensor->Sensor
#define hwSensor(sn)			sn->mbSensor.Sensor->Sensor
#define mbActuator(sn, pos)		TmbActuatorPtr(sn->ActuatorsLst.mbActuators.peek(pos))

/*************************************************************/
/***                         CLASS                         ***/
/*************************************************************/
class ModbusDomoboard: public DomoBoard {
private:

	ModbusSlave	*mbs = NULL;

public:
	//Sensores Sensores Digitales
	PTmbSensor	BOTON1;
	PTmbSensor	BOTON2;
	PTmbSensor	BTN_OPT;
	PTmbSensor	PIR_MOV;

	//Sensores Analógicos
	PTmbSensor	POT1;				// Potenciómetro 1
	PTmbSensor	POT2;				// Potenciómetro 2
	PTmbSensor	PHOTORES;			// Photoresistor
	PTmbSensor	TEMPSEN;			// Sensor Temperatura
	PTmbSensor	PHOTOTTOR;			// Photo Transistor
	PTmbSensor	GASMQ2;				// Sensor de gas

	//Actuadores
	TmbActuator RELE;
	TmbActuator TRIAC;

	//Virtual Actuators
	TmbActuator 	PERUP;				// Actuador virtual para el control de la persiana - Subir
	TmbActuator 	PERDOWN;			// Actuador virtual para el control de la persiana - Bajar

	TmbDimmer		MBDIMMER1;
	TmbActuator		MBDIMMERACT1;
	TmbDimmer		MBDIMMER2;
	TmbActuator		MBDIMMERACT2;

	ModbusDomoboard();
	virtual ~ModbusDomoboard(){};

	void 	leerAllSensor(void);
	void 	leerSensor(PTmbSensor *Sensor);

	void setmbActuator(TmbActuator *Actuator, TStateDigitalDev val);
	void manager_mbActuators(TmbActuatorsLst *ActuatorsLst, TStateDigitalDev val);
	void setModBusSlave(ModbusSlave *mbSlave);
	void set_holdingRegister(uint16_t addReg, uint16_t value);
	void set_coilRegister(uint16_t addReg, uint16_t value);
	static void enableSensor(TmbSensorPtr Sensor, bool enabled);
	static void InicializarTimerDimmer(TDimmerPtr dimmerPtr);


	int leerRegistroHolding(uint8_t index);
};

extern ModbusDomoboard mbDomoboard;

#endif /* DOMOBOARD_MODBUSDOMOBOARD_H_ */
