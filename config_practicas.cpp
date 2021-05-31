/*
 * config_practicas.cpp
 *
 *  Created on: 10/03/2015
 *      Author: jctejero
 *
 *      Modified: 07/05/2021
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"config_practicas.h"
#include	"Gest_Modbus.h"
#include	"debuglog.h"
#include	"utils_domoBoard.h"
#include 	<TimerOne.h>

const uint16_t	persianaTiempoSubida 	= 7200;

#define PERIODO_DIMMER 100 		// En microsegundos

TctrlTime 	ctrlPorPer;			//Control del tanto porciento de apertura de la parsiana

/****************************************************************************/
/***                 Functions                                            ***/
/****************************************************************************/

void config_practica1_apt_1(){
#ifndef SOLO_MODBUS
	domoboard.BOTON1.SensorEvent = Pulsado_Soltado;
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.clear();

	domoboard.BOTON2.SensorEvent = Pulsado_Soltado;
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.clear();

	domoboard.BTN_OPT.SensorEvent = Pulsado_Soltado;
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.clear();
#endif //SOLO_MODBUS
}

void config_practica1_apt_2(void){
#ifndef SOLO_MODBUS
	domoboard.BOTON1.SensorEvent = Interruptor;
	domoboard.BOTON1.Sensor.Aux = OFF;
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.clear();

	domoboard.BOTON2.SensorEvent = Interruptor;
	domoboard.BOTON2.Sensor.Aux = OFF;
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.clear();

	domoboard.BTN_OPT.SensorEvent = Interruptor;
	domoboard.BTN_OPT.Sensor.Aux = OFF;
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.clear();
#endif //SOLO_MODBUS
}

void config_practica1_apt_3(void){
#ifndef SOLO_MODBUS
	domoboard.BOTON1.SensorEvent = conmutador;
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.clear();

	domoboard.BOTON2.SensorEvent = conmutador;
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.clear();

	domoboard.BTN_OPT.SensorEvent = conmutador;
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.clear();
#endif //SOLO_MODBUS
}

void config_practica3_apt_2(void){
#ifndef SOLO_MODBUS
	domoboard.BOTON1.SensorEvent = conmutador;
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.push(&domoboard.RELE);

	domoboard.BOTON2.SensorEvent = conmutador;
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.push(&domoboard.RELE);

	domoboard.BTN_OPT.SensorEvent = conmutador;
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.push(&domoboard.RELE);
#endif //SOLO_MODBUS
}

void config_practica4_apt_1(void){
#ifndef SOLO_MODBUS
	domoboard.BOTON1.SensorEvent = Pulsado_Soltado;
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.push(&domoboard.RELE);

	domoboard.BOTON2.SensorEvent = Pulsado_Soltado;
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.push(&domoboard.TRIAC);

	domoboard.BTN_OPT.SensorEvent = Pulsado_Soltado;
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.clear();
#endif //SOLO_MODBUS
}

void config_practica4_apt_2(void){
#ifndef SOLO_MODBUS
	domoboard.BOTON1.SensorEvent = Interruptor;
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.push(&domoboard.RELE);

	domoboard.BOTON2.SensorEvent = Interruptor;
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.push(&domoboard.TRIAC);

	domoboard.BTN_OPT.SensorEvent = Interruptor;
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.clear();
#endif //SOLO_MODBUS
}

void config_practica4_apt_3(void){
#ifndef SOLO_MODBUS
	domoboard.BOTON1.SensorEvent = conmutador;
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.push(&domoboard.RELE);
	domoboard.BOTON1.Sensor.ActuatorsLst.Actuators.push(&domoboard.TRIAC);

	domoboard.BOTON2.SensorEvent = conmutador;
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.push(&domoboard.RELE);
	domoboard.BOTON2.Sensor.ActuatorsLst.Actuators.push(&domoboard.TRIAC);

	domoboard.BTN_OPT.SensorEvent = conmutador;
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.clear();
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.push(&domoboard.RELE);
	domoboard.BTN_OPT.Sensor.ActuatorsLst.Actuators.push(&domoboard.TRIAC);
#endif //SOLO_MODBUS
}

void config_practica5_apt_4(){

	mbDomoboard.BOTON1.mbSensorEvent = mbInterruptor;
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.push(&mbDomoboard.RELE);

	mbDomoboard.BOTON2.mbSensorEvent = mbInterruptor;
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.push(&mbDomoboard.TRIAC);

	mbDomoboard.BTN_OPT.mbSensorEvent = mbInterruptor;
	mbDomoboard.BTN_OPT.mbSensor.ActuatorsLst.mbActuators.clear();
}

void Config_P7_SensorMovimiento(){
	static TctrlTemp	ctrlTempPIR;  //La declaramos static para que al volver de la función,

	//Inicialmente, habilitamos el sensor PIR
	DomoBoard::enableSensor(&(mbDomoboard.PIR_MOV.mbSensor.Sensor->Sensor), true);
	Cregs[MB_ACTPIR] = 0x01;  //Actualiza registro que monitoriza la habilitación del sensor

	//Borramos los actuadores previos asignado a cada sensor de entrada
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BTN_OPT.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.PIR_MOV.mbSensor.ActuatorsLst.mbActuators.clear();

	//Inicializamos control de temporización
	ctrlTempPIR.estado = false;
	ctrlTempPIR.Cte_Norma = 1000;
	ctrlTempPIR.time_lastAct = millis();
	ctrlTempPIR.time_activo = &(Aregs[MB_TMP_PIR]);

	mbDomoboard.PIR_MOV.mbSensorEvent = Interruptor_Temporizado;
	mbDomoboard.PIR_MOV.mbSensor.ctrlTempPtr = &ctrlTempPIR;
	mbDomoboard.PIR_MOV.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BOTON1.mbSensorEvent = mbConmutador;
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.TRIAC));

	mbDomoboard.BOTON2.mbSensorEvent = mbConmutador;
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.TRIAC));

	mbDomoboard.BTN_OPT.mbSensorEvent = mbConmutador;
	mbDomoboard.BTN_OPT.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.TRIAC));

	AccionesTemporizadas.clear();
	AccionesTemporizadas.push(&(mbDomoboard.PIR_MOV));
}


void Config_P9_SRC_Interruptor(void){
	static TctrlLevel	ctrlLevelSRC;  //La declaramos static para que al volver de la función,
	static TctrlLevel	ctrlLevelTTOR;  //La declaramos static para que al volver de la función,

	AccionesTemporizadas.clear();
	mbDomoboard.enableSensor(&(mbDomoboard.PIR_MOV.mbSensor), false);

	//Borramos los actuadores previos asignado a cada sensor de entrada
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BTN_OPT.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.PIR_MOV.mbSensor.ActuatorsLst.mbActuators.clear();

	ctrlLevelSRC.lSup = &(Aregs[MB_SRC_HL]);
	ctrlLevelSRC.lInf = &(Aregs[MB_SRC_LL]);
	mbDomoboard.PHOTORES.mbSensor.ctrlLevelPtr = &ctrlLevelSRC;
	mbDomoboard.PHOTORES.mbSensorEvent = interruptor_SRC;
	mbDomoboard.PHOTORES.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.TRIAC));


	ctrlLevelTTOR.lInf = &(Aregs[MB_TTOR_LL]);
	mbDomoboard.PHOTOTTOR.mbSensor.ctrlLevelPtr = &ctrlLevelTTOR;
	mbDomoboard.PHOTOTTOR.mbSensorEvent = trigger_Level;
	mbDomoboard.PHOTOTTOR.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BOTON1.mbSensorEvent = mbConmutador;
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BOTON2.mbSensorEvent = mbConmutador;
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.RELE));

	mbDomoboard.BTN_OPT.mbSensorEvent = mbConmutador;
	mbDomoboard.BTN_OPT.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.RELE));
}

void Config_P10_CtrlPersianas(){
	static int32_t		TiempoSubidaActual = 0;

	ctrlPorPer.activa		= false;
	ctrlPorPer.maxTime		= (uint16_t *)&persianaTiempoSubida;
	ctrlPorPer.actTime		= &TiempoSubidaActual;

	AccionesTemporizadas.clear();

	//Borramos los actuadores previos asignado a cada sensor de entrada
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BTN_OPT.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.PIR_MOV.mbSensor.ActuatorsLst.mbActuators.clear();

	mbDomoboard.BOTON1.mbSensorEvent = Persiana;
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.PERUP));

	mbDomoboard.BOTON2.mbSensorEvent = Persiana;
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.PERDOWN));
}

void Config_P10_CtrlGarage(){
	static int32_t				TiempoSubidaActual = 0;
	static TmbRelatedSensors 	RelatedSensors;			//Lista de sensores relacionados/implicados en la utilidad
	static TctrlTemp			ctrlTempGar;  			//La declaramos static para que al volver de la función,

	//Inicializamos control de temporización
	ctrlTempGar.estado = false;
	ctrlTempGar.Cte_Norma = 1000;
	ctrlTempGar.time_lastAct = millis();

	ctrlPorPer.activa		= false;
	ctrlPorPer.maxTime		= (uint16_t *)&persianaTiempoSubida;
	ctrlPorPer.actTime		= &TiempoSubidaActual;

	AccionesTemporizadas.clear();

	//Borramos los actuadores previos asignado a cada sensor de entrada
	mbDomoboard.BOTON1.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BOTON2.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BTN_OPT.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.PIR_MOV.mbSensor.ActuatorsLst.mbActuators.clear();
	mbDomoboard.BOTON2.mbSensorEvent = NULL;

	RelatedSensors.mbRelatedSensors.push(&(mbDomoboard.BOTON1.mbSensor));

	mbDomoboard.BOTON1.mbSensorEvent = PuertaGarage;
	mbDomoboard.BOTON1.mbSensor.ctrlTempPtr = &ctrlTempGar;

	mbDomoboard.PHOTOTTOR.mbSensor.ctrlLevelPtr->lInf = &(Aregs[MB_TTOR_LL]);
	mbDomoboard.PHOTOTTOR.mbSensor.ctrlTempPtr = NULL;
	mbDomoboard.PHOTOTTOR.mbSensorEvent = PuertaGarage;
	mbDomoboard.PHOTOTTOR.mbSensor.RelatedSensors = &RelatedSensors;

	//Para comprobar regularmente el estado de la puerta del garage
	AccionesTemporizadas.clear();
	AccionesTemporizadas.push(&(mbDomoboard.BOTON1));
}


void Config_P11_Potenciometros(){
	// DOMOBOARD
	mbDomoboard.POT1.mbSensorEvent = DimmerAnalogMap;
	mbDomoboard.POT1.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.MBDIMMER1.mbActuator));

	mbDomoboard.POT2.mbSensorEvent = DimmerAnalogMap;
	mbDomoboard.POT2.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.MBDIMMER2.mbActuator));

	// TIMERS
	// Initialization
	Timer1.initialize(PERIODO_DIMMER);
	Timer1.attachInterrupt(ColaDimmers);
	ActivarDimmers();
}

void Config_P11_2_Fotorresistor(){
	// DOMOBOARD
	mbDomoboard.PHOTORES.mbSensorEvent = DimmerAnalogMapFotores;
	mbDomoboard.PHOTORES.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.MBDIMMER1.mbActuator));
//	mbDomoboard.PHOTORES.mbSensor.ActuatorsLst.mbActuators.push(&(mbDomoboard.MBDIMMER2.mbActuator));

	// TIMERS
	// Initialization
	Timer1.initialize(PERIODO_DIMMER);
	Timer1.attachInterrupt(ColaDimmers);
	ActivarDimmers();
}




void SelectionConfiguration(uint8_t selConf){
	switch(selConf){
		case P1_PULSADORES:
			DEBUGLNF("P1 PULSADORES Seleccionado (:");
			config_practica1_apt_1();
			break;

		case P1_INTERRUPTOR:
			DEBUGLNF("P1 INTERRUPTOR Seleccionado");
			config_practica1_apt_2();
			break;

		case P1_CONMUTADOR:
			DEBUGLNF("P1 CONMUTADOR Seleccionado");
			config_practica1_apt_3();
			break;

		case P3_CONMUTADOR:
			DEBUGLNF("P3 CONMUTADOR Seleccionado");
			config_practica3_apt_2();
			break;

		case P4_PULSADORES:
			DEBUGLNF("P4 PULSADORES Seleccionado ");
			config_practica4_apt_1();
			break;

		case P4_INTERRUPTOR:
			DEBUGLNF("P4 INTERRUPTOR Seleccionado");
			config_practica4_apt_2();
			break;

		case P4_CONMUTADOR:
			DEBUGLNF("P4 CONMUTADOR Seleccionado");
			config_practica4_apt_3();
			break;

		case P5_INTERRUPTOR:
			DEBUGLNF("P5 INTERRUPTOR Seleccionado");
			config_practica5_apt_4();
			break;

		case P6_INTERRUPTOR:
			DEBUGLNF("P6 INTERRUPTOR Seleccionado");
			config_practica5_apt_4();
			break;

		case P7_PIR:
			DEBUGLNF("P07 Seleccionada --> Sensor Movimiento (PIR)");
			Config_P7_SensorMovimiento();
			break;

		case P9_All:
			DEBUGLNF("P09 Seleccionada --> Sensores Analógicos (All)");
			Config_P9_SRC_Interruptor();
			break;

		case P10_PER:
			DEBUGLNF("P10-1 Control de persianas");
			Config_P10_CtrlPersianas();
			break;

		case P10_GAR:
			DEBUGLNF("P10-2 Control de Puerta Garage");
			Config_P10_CtrlGarage();
			break;

		case P11_POT:
			DEBUGLNF("P11-1 Control dimmers por potenciometros");
			Config_P11_Potenciometros();
			break;
		case P11_FOTORES:
			DEBUGLNF("P11-1 Control dimmers por fotorresistor");
			Config_P11_2_Fotorresistor();
			break;
	}
}
