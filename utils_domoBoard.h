/*
 * utils_domoBoard.h
 *
 *  Created on: 10/03/2015
 *      Author: jctejero
 *
 *      Modified: 28/04/2021
 */

#ifndef UTILS_DOMOBOARD_H_
#define UTILS_DOMOBOARD_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"Arduino.h"
#include	"DomoBoard/ModbusDomoBoard.h"
#include	<EEPROM.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define 	ON   				HIGH
#define     OFF					LOW

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
void Pulsado_Soltado(TSensorPtr Sensor);
void Interruptor(TSensorPtr Sensor);
void conmutador(TSensorPtr Sensor);
void mbInterruptor(TmbSensorPtr Sensor);
void mbConmutador(TmbSensorPtr Sensor);
void Interruptor_Temporizado(TmbSensorPtr mbSensor);
void AccionesTemporizadas_Timer(void);
void Calc_Temperatura(TmbSensorPtr mbSensor);
void interruptor_SRC(TmbSensorPtr mbSensor);
void trigger_Level(TmbSensorPtr mbSensor);
void Persiana(TmbSensorPtr mbSensor);
void UpDown_Persiana();
void Ctrl_PosicionPersiana(TctrlTimePtr ctrlPosPer, tsStaPer staPer);
void PuertaGarage(TmbSensorPtr mbSensor);

void DimmerAnalogMap(TmbSensorPtr mbSensor);
void pasoPorCero(void);
void ColaDimmers(void);
void ActivarDimmers();
void DimmerAnalogMapFotores(TmbSensorPtr mbSensor);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern QueueList<PTmbSensorPtr>		AccionesTemporizadas;

#endif /* UTILS_DOMOBOARD_H_ */
