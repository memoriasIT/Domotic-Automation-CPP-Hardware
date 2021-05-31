/*
 * ModbusDomoboard.cpp
 *
 *  Created on: 11 mar. 2020
 *      Author: jctejero
 *
 *  modified on: 28/04/2021
 */

#include "ModbusDomoboard.h"
#include "../utils_domoBoard.h"

/***************************************************
 * Definición Bancos de registros usados en ModBus *
 ***************************************************/
uint16_t	Cregs[MB_O_COILS];		//Registros para "Dicrete Output Coils"
uint16_t	Dregs[MB_I_CONTATCS];	//Registros para "Dicrete Input Contacts"
uint16_t	Aregs[MB_A_REGS];		//Registros para "Analog Output Holding Registers"
uint16_t	Iregs[MB_I_REGS];		//Registros para "Analog Input Registers"

ModbusDomoboard mbDomoboard;

ModbusDomoboard::ModbusDomoboard():DomoBoard() {
	//*****  Initialize ModBus Sensors  ****

	//Initialize ModBus Register for BOTON1

	BOTON1.mbSensor.bkMBRegs 		= 	Dregs;
	BOTON1.mbSensor.nMBReg 			= 	MB_BOTON1;
	BOTON1.mbSensor.Sensor 			= 	&(this->DomoBoard::BOTON1);
	BOTON1.mbSensorEvent			= 	NULL;
	Dregs[MB_BOTON1] 				= 	this->DomoBoard::BOTON1.Sensor.valor;

	BOTON2.mbSensor.bkMBRegs 		= 	Dregs;
	BOTON2.mbSensor.nMBReg 			= 	MB_BOTON2;
	BOTON2.mbSensor.Sensor 			= 	&(this->DomoBoard::BOTON2);
	BOTON2.mbSensorEvent			= 	NULL;
	Dregs[MB_BOTON2] 				= 	this->DomoBoard::BOTON2.Sensor.valor;

	BTN_OPT.mbSensor.bkMBRegs 		= 	Dregs;
	BTN_OPT.mbSensor.nMBReg 		= 	MB_BTNOPT;
	BTN_OPT.mbSensor.Sensor 		= 	&(this->DomoBoard::BTN_OPT);
	BTN_OPT.mbSensorEvent			= 	NULL;
	Dregs[MB_BTNOPT] 				= 	this->DomoBoard::BTN_OPT.Sensor.valor;

	//Inicializamos el registros ModBus Sensor PIR
	PIR_MOV.mbSensor.bkMBRegs		= 	Dregs;
	PIR_MOV.mbSensor.nMBReg			=	MB_PIRMOV;
	PIR_MOV.mbSensor.Sensor			= 	&(this->DomoBoard::PIR_MOV);
	PIR_MOV.mbSensorEvent			= 	NULL;

	//Initialize ModBus Analog sensors
	POT1.mbSensor.Sensor			=   &(this->DomoBoard::POT1);
	POT1.mbSensor.bkMBRegs			=	Iregs;
	POT1.mbSensor.nMBReg			=	MB_POT1;
	POT1.mbSensorEvent				= 	NULL;

	POT2.mbSensor.Sensor			=   &(this->DomoBoard::POT2);
	POT2.mbSensor.bkMBRegs			=	Iregs;
	POT2.mbSensor.nMBReg			=	MB_POT2;
	POT2.mbSensorEvent				= 	NULL;

	PHOTORES.mbSensor.Sensor		=   &(this->DomoBoard::PHOTORES);
	PHOTORES.mbSensor.bkMBRegs		=	Iregs;
	PHOTORES.mbSensor.nMBReg		=	MB_PHOTORES;
	POT2.mbSensorEvent				= 	NULL;

	TEMPSEN.mbSensor.Sensor			=   &(this->DomoBoard::TEMPSEN);
	TEMPSEN.mbSensor.bkMBRegs		=	Iregs;
	TEMPSEN.mbSensor.nMBReg			=	MB_TEMPSEN;
	TEMPSEN.mbSensorEvent			=   Calc_Temperatura;

	PHOTOTTOR.mbSensor.Sensor		=   &(this->DomoBoard::PHOTOTTOR);
	PHOTOTTOR.mbSensor.bkMBRegs		=	Iregs;
	PHOTOTTOR.mbSensor.nMBReg		=	MB_PHOTOTTOR;
	PHOTOTTOR.mbSensorEvent			= 	NULL;

	GASMQ2.mbSensor.Sensor			=   &(this->DomoBoard::GASMQ2);
	GASMQ2.mbSensor.bkMBRegs		=	Iregs;
	GASMQ2.mbSensor.nMBReg			=	MB_GASMQ2;
	GASMQ2.mbSensorEvent			= 	NULL;

	//Initialize ModBus Actuators
	RELE.actuator 					= 	&(this->DomoBoard::RELE);
	RELE.bkMBRegs 					= 	Cregs;
	RELE.nMBReg 					= 	MB_RELE;
	Cregs[MB_RELE] 					= 	this->DomoBoard::RELE.estado;

	TRIAC.actuator 					= 	&(this->DomoBoard::TRIAC);
	TRIAC.bkMBRegs 					= 	Cregs;
	TRIAC.nMBReg 					= 	MB_TRIAC;
	Cregs[MB_TRIAC] 				= 	this->DomoBoard::TRIAC.estado;

	PERUP.bkMBRegs					=	Cregs;
	PERUP.nMBReg					=	MB_PERUP;
	Cregs[MB_PERUP] 				=	OFF;

	PERDOWN.bkMBRegs				=	Cregs;
	PERDOWN.nMBReg					=	MB_PERDOWN;
	Cregs[MB_PERDOWN] 				=	OFF;

	// TODO
	MBDIMMER1.dimmer			= &(this->DomoBoard::DIMMER1);
	MBDIMMERACT1.actuator		= &(MBDIMMER1.dimmer->actuator);
	MBDIMMERACT1.bkMBRegs		= Iregs;
	MBDIMMERACT1.nMBReg			= MB_DIM1;
	MBDIMMER1.mbActuator		= MBDIMMERACT1;

	// TODO
	MBDIMMER2.dimmer			= &(this->DomoBoard::DIMMER2);
	MBDIMMERACT2.actuator		= &(MBDIMMER2.dimmer->actuator);
	MBDIMMERACT2.bkMBRegs		= Iregs;
	MBDIMMERACT2.nMBReg			= MB_DIM2;
	MBDIMMER2.mbActuator		= MBDIMMERACT2;
}

void ModbusDomoboard::enableSensor(TmbSensorPtr Sensor, bool enabled){

	DomoBoard::enableSensor(&(Sensor->Sensor->Sensor), enabled);

	if(!enabled){
		Sensor->bkMBRegs[Sensor->nMBReg] = Sensor->Sensor->Sensor.valor;
	}
}

void ModbusDomoboard::set_holdingRegister(uint16_t addReg, uint16_t value){
	Aregs[addReg] = value;

	if(mbs->writeholdingregister != NULL)
		mbs->writeholdingregister(addReg);
}

void ModbusDomoboard::set_coilRegister(uint16_t addReg, uint16_t value){
	Cregs[addReg] = value;
	if(mbs->writecoil != NULL)
		mbs->writecoil(addReg);
}

void ModbusDomoboard::leerAllSensor(void){
	leerSensor(&BOTON1);
	leerSensor(&BOTON2);
	leerSensor(&BTN_OPT);
	leerSensor(&PIR_MOV);
	leerSensor(&POT1);
	leerSensor(&POT2);
	leerSensor(&PHOTORES);
	leerSensor(&TEMPSEN);
	leerSensor(&PHOTOTTOR);
	leerSensor(&GASMQ2);
}

void ModbusDomoboard::leerSensor(PTmbSensorPtr Sensor){

	DomoBoard::leerSensor(Sensor->mbSensor.Sensor);

	//compueba si el valor leído por el sensor difiere del valor almacenado en el registro correspondiente
	//del banco de registros
	if((int16_t)Sensor->mbSensor.bkMBRegs[Sensor->mbSensor.nMBReg] != Sensor->mbSensor.Sensor->Sensor.valor){
		//Estado Sensor ha cambiado
		//Se actualiza el registro correspondiente con el nuevo valor leído en el sensor.
		Sensor->mbSensor.bkMBRegs[Sensor->mbSensor.nMBReg] = Sensor->mbSensor.Sensor->Sensor.valor;

		//Se inícia el evento asociado a la actualización del banco de registro correpondiente
		if((Sensor->mbSensorEvent != NULL) && Sensor->mbSensor.Sensor->Sensor.Activo){
			Sensor->mbSensorEvent(&(Sensor->mbSensor));
		}
	}
}

void  ModbusDomoboard::setmbActuator(TmbActuator *Actuator, TStateDigitalDev val){
	bool newVal = (bool)val;

	if(val == TOGGLE){
		newVal = !Actuator->bkMBRegs[Actuator->nMBReg];
	}

	if(Actuator->bkMBRegs[Actuator->nMBReg] != newVal){
		Actuator->bkMBRegs[Actuator->nMBReg] = newVal;

		if(mbs != NULL){
			if(mbs->writecoil != NULL){
				mbs->writecoil(Actuator->nMBReg);
			}
		}
	}
}

void ModbusDomoboard::InicializarTimerDimmer(TDimmerPtr dimmerPtr){
	dimmerPtr->pulso = false;
	dimmerPtr->activationVal = 0;
	dimmerPtr->delay = 0;

}

void ModbusDomoboard::setModBusSlave(ModbusSlave *mbSlave){
	mbs = mbSlave;
}

void ModbusDomoboard::manager_mbActuators(TmbActuatorsLst *ActuatorsLst, TStateDigitalDev val){
	for(int n = 0; n < ActuatorsLst->mbActuators.count(); n++){
		setmbActuator(ActuatorsLst->mbActuators.peek(n), val);
	}
}


int ModbusDomoboard::leerRegistroHolding(uint8_t index){
	Serial.println(Iregs[index]);
	return Iregs[index];
}
