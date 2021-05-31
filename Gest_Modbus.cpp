/*
 * Gest_Modbus.cpp
 *
 *  Created on: 16/05/2014
 *      Author: jctejero
 *
 *  modified on: 28/04/2021
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	<Arduino.h>
#include	"Gest_Modbus.h"
#include 	"utils_domoBoard.h"
#include	"config_practicas.h"

/****************************************************************************/
/***        Variables Locales                                             ***/
/****************************************************************************/
/* First step MBS: create an instance */
ModbusSlave mbs;

/****************************************************************************/
/***                 Definici�n de funciones                              ***/
/****************************************************************************/
void writecoil(unsigned int addrReg);
void writeholdingregister(unsigned int addrReg);

/****************************************************************************/
/***                 Functions                                            ***/
/****************************************************************************/
void Init_RTU_Modbus()
{
	/* configure modbus communication
	 * 19200 bps, 8E1, two-device network */
	/* Second step MBS: configure */
	/* the Modbus slave configuration parameters */
	const unsigned char 	SLAVE 	= ADDR_SLAVE;		//Address SLAVE
	const long 				BAUD 	= SERIAL_BPS;
	const char 				PARITY 	= SERIAL_PARITY;
	const char 				TXENPIN = 0; //EN_485;

	//Inicialmente configuramos 485 para recibir
	if(TXENPIN != 0)
		digitalWrite(EN_485, LOW);
	else digitalWrite(EN_485, HIGH);

	//Para la conexi�n 485/ModBus usamos
	Serial485 = &Serial;

	//We configure the ModBus Register Banks
	mbs.set_BankCoils(Cregs, MB_O_COILS);
	mbs.set_BankDiscreteInputs(Dregs, MB_I_CONTATCS);
	mbs.set_BankOutputHoldingRegister(Aregs, MB_A_REGS);
	mbs.set_BankInputRegister(Iregs, MB_I_REGS);

	mbs.configure(SLAVE,BAUD,PARITY,TXENPIN);

	//Sets up event that occurred when Output Discrete register are updated
	mbs.writecoil = writecoil;
	mbs.writeholdingregister = writeholdingregister;

	//Initialize Gestion ModBus DomoBoard
	mbDomoboard.setModBusSlave(&mbs);
}


/*
 *
 */

void writecoil(unsigned int addrReg){
	switch(addrReg){
	case	MB_RELE:
		DomoBoard::setActuator(&(mbDomoboard.DomoBoard::RELE),Cregs[addrReg]!=0x00);
		break;

	case	MB_TRIAC:
		DomoBoard::setActuator(&(mbDomoboard.DomoBoard::TRIAC),Cregs[addrReg]!=0x00);
		break;

	case   MB_ACTPIR:
		DomoBoard::enableSensor(&(mbDomoboard.PIR_MOV.mbSensor.Sensor->Sensor), Cregs[addrReg]!=0x00);
		EEPROM.update(ADDR_ACTPIR, Cregs[addrReg]!=0x00);
		break;

	case	MB_ACTSRC:
		DomoBoard::enableSensor(&(mbDomoboard.PHOTORES.mbSensor.Sensor->Sensor), Cregs[addrReg]!=0x00);
		EEPROM.update(ADDR_ACTSRC, Cregs[addrReg]!=0x00);
		break;

	case	MB_ACTTOR:
		DomoBoard::enableSensor(&(mbDomoboard.PHOTOTTOR.mbSensor.Sensor->Sensor), Cregs[addrReg]!=0x00);
		EEPROM.update(ADDR_ACTTOR, Cregs[addrReg]!=0x00);
		break;

	case	MB_PERUP:
	case	MB_PERDOWN:
		UpDown_Persiana();
		break;

	case 	MB_KEYGAR:

		break;

	}
}

void writeholdingregister(unsigned int addrReg){
	//char msg[50];

	switch(addrReg){
	case MB_SELPRACT:
		//Save new configuration
		EEPROM.update(ADDR_SELPRACT, Aregs[ADDR_SELPRACT]&0xFF);

		//Go To Selecction Configuration
		SelectionConfiguration(Aregs[MB_SELPRACT]&0xFF);
		break;

	case MB_TMP_PIR:
		//Almacenar Tiempo de activaci�n Sensor PIR (Big Endian)
		EEPROM.update(ADDR_TIEMPO_PIR_1, (Aregs[MB_TMP_PIR]>>8)&0xFF);
		EEPROM.update(ADDR_TIEMPO_PIR_2, (Aregs[MB_TMP_PIR]&0xFF));
		break;

	case MB_SRC_HL:
		//Almacenar nivel superior de activaci�n SRC (Big Endian)
		EEPROM.update(ADDR_SRC_HL_1, (Aregs[MB_SRC_HL]>>8)&0xFF);
		EEPROM.update(ADDR_SRC_HL_2, (Aregs[MB_SRC_HL]&0xFF));
		break;

	case MB_SRC_LL:
		//Almacenar nivel inferior de activaci�n SRC (Big Endian)
		EEPROM.update(ADDR_SRC_LL_1, (Aregs[MB_SRC_LL]>>8)&0xFF);
		EEPROM.update(ADDR_SRC_LL_2, (Aregs[MB_SRC_LL]&0xFF));
		break;

	case MB_TTOR_LL:
		//Almacenar nivel inferior de activaci�n SRC (Big Endian)
		EEPROM.update(ADDR_TTOR_LL_1, (Aregs[MB_TTOR_LL]>>8)&0xFF);
		EEPROM.update(ADDR_TTOR_LL_2, (Aregs[MB_TTOR_LL]&0xFF));
		break;

	case MB_STAPER:
		//Actualiza posici�n Persiana
		Ctrl_PosicionPersiana(&ctrlPorPer, tsStaPer(Aregs[MB_STAPER]&0xFF));

		mbDomoboard.SetPersiana(tsStaPer(Aregs[MB_STAPER]&0xFF));
		break;
	}
}


void RTU_ModBus()
{
	unsigned long wdog = 0;         /* watchdog */

	if(mbs.update()){
		wdog = millis();

		if ((millis() - wdog) > 5000)  {      // no comms in 5 sec
			//regs[MB_CTRL] = 0;	// turn off led
		}
	}
}

/*
 * void load_config().- load initial configuration for arduino aplication
 */
void load_Config(){
	//Leemos configuraci�n Actual "Selecci�n Pr�ctica"
	uint8_t val = EEPROM.read(ADDR_SELPRACT);	//Read EEPROM
	mbDomoboard.set_holdingRegister(MB_SELPRACT, val);

	/*
	 * Config PIR
	 */
	//Leer activaci�n del PIR
	val = EEPROM.read(ADDR_ACTPIR);
	mbDomoboard.set_coilRegister(MB_ACTPIR, val);

	//Leer Tiempo activaci�n PIR
	Aregs[MB_TMP_PIR] = ((EEPROM.read(ADDR_TIEMPO_PIR_1)&0xFF) << 8) + (EEPROM.read(ADDR_TIEMPO_PIR_2)&0xFF) ;

	/*
	 * Configuration SRC
	 */
	val = EEPROM.read(ADDR_ACTSRC);
	Cregs[MB_ACTSRC] = val;
	mbs.writecoil(MB_ACTSRC);

	Aregs[MB_SRC_HL] = (EEPROM.read(ADDR_SRC_HL_1) << 8) + (EEPROM.read(ADDR_SRC_HL_2)&0xFF) ;
	Aregs[MB_SRC_LL] = (EEPROM.read(ADDR_SRC_LL_1) << 8) + (EEPROM.read(ADDR_SRC_LL_2)&0xFF) ;

	/*
	 * Configuration Phototransistor
	 */
	val = EEPROM.read(ADDR_ACTTOR);
	Cregs[MB_ACTTOR] = val;
	mbs.writecoil(MB_ACTTOR);

	Aregs[MB_TTOR_LL] = (EEPROM.read(ADDR_TTOR_LL_1) << 8) + (EEPROM.read(ADDR_TTOR_LL_2)&0xFF) ;
}

