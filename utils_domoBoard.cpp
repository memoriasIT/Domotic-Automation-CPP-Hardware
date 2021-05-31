/*
 * utils_domoBoard.cpp
 *
 *
 *      Author: jctejero
 *
 *      Modified: 28/04/2021
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include 	"utils_domoBoard.h"
#include	"Gest_Modbus.h"
#include 	"Gest_Modbus.h"
#include	"DomoBoard/ModbusDomoboard.h"

#define	STEP_CTRL_BLIND		5
#define PERCENTAGE(x)		((x % STEP_CTRL_BLIND) > 2) ? x + (STEP_CTRL_BLIND-(x % STEP_CTRL_BLIND)) : x - (x % STEP_CTRL_BLIND);
#define TIEMPOFASE			10
#define ANTEFASE			100


const uint16_t	persianaTiempoParada 	= 1000;
const uint16_t	persianaTiempoAbierta 	= 5000;


/******************************************************************/
/***                      Variables Locales                     ***/
/******************************************************************/
//Lista de acciones temporizadas
QueueList<PTmbSensorPtr>		AccionesTemporizadas;
QueueList<TTimerEvent>			funcDimmers;
QueueList<TmbDimmerPtr>			listaDimmers;

/*============================================*/
/*		 		    PULSADOR                  */
/*============================================*/
void Pulsado_Soltado(TSensorPtr Sensor){
	bool valor = OFF;

	DEBUG(Sensor->name);
	if(Sensor->valor_Df == Sensor->valor){
		DEBUGLNF(" --> Soltado");
		valor = OFF;
	}else{
		DEBUGLNF(" --> Pulsado");
		valor = ON;
	}

	DomoBoard::managerActuators(&Sensor->ActuatorsLst, valor);
}

/*============================================*/
/*		 		  INTERRUPTOR                 */
/*============================================*/
void Interruptor(TSensorPtr Sensor)
{
	if(Sensor->valor_Df == Sensor->valor){
		DEBUG(Sensor->name);
		DEBUGF(" : Interruptor --> ");
		if(Sensor->Aux == OFF){
			Sensor->Aux = ON;
			DEBUGLNF("ON");
		}else{
			Sensor->Aux = OFF;
			DEBUGLNF("OFF");
		}
	}

	DomoBoard::managerActuators(&Sensor->ActuatorsLst, Sensor->Aux);
}

/*============================================*/
/*		     	   CONMUTADOR                 */
/*============================================*/
void conmutador(TSensorPtr Sensor)
{
	static int valor = OFF;

	if(Sensor->valor_Df == Sensor->valor){
		DEBUGF("Conmutador --> ");
		if(valor == OFF){
			valor = ON;
			DEBUGLNF("ON");
		}else{
			valor = OFF;
			DEBUGLNF("OFF");
		}
	}

	DomoBoard::managerActuators(&Sensor->ActuatorsLst, valor);
}

/*========================================================*/
/*		 		  INTERRUPTOR TEMPORIZADO                 */
/*========================================================*/

/*
 * Este sensor será activado mediante Sensor (Variable de control) y aunque se vuelva inactivo,
 * el interruptor, permanecerá activo durante el tiempo que indique el temporizador.
 * Si durante el tiempo que el interruptor está activo, la variable de control permanece inactiva,
 * o se activa de nuevo, el tiempo que el interruptor permanece activo se
 * irá actualizando, de tal forma que el tiempo que el interruptor permanece ativo,
 * siempre se cuenta desde la última vez que el sensor se activo.
 *
 */

void	Interruptor_Temporizado(TmbSensorPtr mbSensorPtr){

	if(mbSensorPtr->Sensor->Sensor.valor_Df != mbSensorPtr->Sensor->Sensor.valor){

		mbSensorPtr->ctrlTempPtr->time_lastAct = millis();

		if(!mbSensorPtr->ctrlTempPtr->estado){
			//ON Actuators
			mbSensorPtr->ctrlTempPtr->estado = true;
			mbDomoboard.manager_mbActuators(&(mbSensorPtr->ActuatorsLst), (TStateDigitalDev)ON);
		}

	}else{

		uint32_t tempo = (millis() - mbSensorPtr->ctrlTempPtr->time_lastAct);
		uint16_t maxtempo = *(mbSensorPtr->ctrlTempPtr->time_activo);
		uint16_t Cte_Norma = mbSensorPtr->ctrlTempPtr->Cte_Norma;

		if(tempo >= (maxtempo)*(Cte_Norma)){
			if(mbSensorPtr->ctrlTempPtr->estado){
				//OFF Actuators
				mbSensorPtr->ctrlTempPtr->estado = false;
				mbDomoboard.manager_mbActuators(&(mbSensorPtr->ActuatorsLst), (TStateDigitalDev)OFF);
			}
		}
	}
}


/*============================================*/
/*		      INTERRUPTOR MODBUS              */
/*============================================*/

void mbInterruptor(TmbSensorPtr Sensor)
{
	if(hwpSensor(Sensor).valor_Df == hwpSensor(Sensor).valor){
		//Estado del sensor ha cambiado
		mbDomoboard.manager_mbActuators(&Sensor->ActuatorsLst, TOGGLE);
	}
}

/*============================================*/
/*		 	   CONMUTADOR MODBUS              */
/*============================================*/
void mbConmutador(TmbSensorPtr Sensor)
{
	if(hwpSensor(Sensor).valor_Df == hwpSensor(Sensor).valor){

		TStateDigitalDev nSta = TStateDigitalDev(! mbActuator(Sensor, 0)->bkMBRegs[mbActuator(Sensor, 0)->nMBReg]);

		//Estado del sensor ha cambiado
		mbDomoboard.manager_mbActuators(&Sensor->ActuatorsLst, nSta);
	}
}

/*****************************************************************************************/
/***********************************  Leer Temperatura ***********************************/
/*****************************************************************************************/
//mas/menos 1 - 2ºC de exactitud para el TMP36, por lo que escribimos valores decimales
void Calc_Temperatura(TmbSensorPtr mbSensor){
	float valTMP;
	int   temp;

	valTMP = mbSensor->Sensor->Sensor.valor*.004882812;	//Conviere resultado convertidor a voltios

	valTMP = (valTMP - .5)*100;          //Convierte Temperatura de 10 mV por grado con 500 mV de Offset


	temp = (valTMP - (int)valTMP)*100;

    if(temp < 50) temp = 0;
    //else if ((25 < temp)&&(temp < 75)) temp = 5;
    //else if ((75 < temp)&&(temp <= 99)){
    else{
    	temp = 0;
    	valTMP = (int)valTMP +1;
    }

    mbSensor->bkMBRegs[mbSensor->nMBReg] = (((int)valTMP & 0xff) << 8) | (temp & 0xff);

#ifdef DEBUG_TEMP
    static float vT = 0;

    if(vT != valTMP){
    	vT = valTMP;

    	DEBUFLOG("Temperatura = %d.%d\n", (int)valTMP, temp);
    }
#endif
}

/*============================================*/
/*			  TRIGGER WITH LUMINOSIDAD        */
/*============================================*/
/*
 * tRIGGER por nivel de luminosidad. Dispara unevento cuando el nivel del sensor
 * se encuentra por debajo de un determinado nivel
 */
void	trigger_Level(TmbSensorPtr mbSensor){
	uint16_t limInfe = *(mbSensor->ctrlLevelPtr->lInf); //

	if((uint16_t) mbSensor->Sensor->Sensor.valor <= limInfe){
		mbDomoboard.manager_mbActuators(&(mbSensor->ActuatorsLst), (TStateDigitalDev)ON);
	}
}

/*============================================*/
/*			  INTERRUPTOR LUMINOSIDAD         */
/*============================================*/
/*
 * Interruptor por nivel de luminosidad. funcionará con una histeresis, es decir,
 * Si el interruptor está desactivado, se activará cuando alcance el "highlevel".
 *
 * Si el interruptor está ativado, se desactivará cuando alcanze el "lowlevel".
 */
void	interruptor_SRC(TmbSensorPtr mbSensor){

	int state = mbSensor->Sensor->Sensor.Aux;

	TmbActuatorPtr actSen = NULL;

	//Detectamos situación inicial
	if(state <= 0){
		//Seleccionamos el actuador inicial

		if(mbSensor->ActuatorsLst.mbActuators.count() > 0){
			actSen = mbSensor->ActuatorsLst.mbActuators.peek(0);
		} else return;

		//Consideramos que el estado del actuador tiene relación directa con
		//el estado del interruptor

		if(actSen == NULL)
			return;

		if((bool)actSen->bkMBRegs[actSen->nMBReg])
			state = 1;		//Actuator ON
		else state = 2;		//Actuator OFF
	}

	switch(state){
	case 1:
		if(mbSensor->Sensor->Sensor.valor <= (int)*(mbSensor->ctrlLevelPtr->lInf)){
			state = 2;
			//Estado del sensor ha cambiado
			mbDomoboard.manager_mbActuators(&(mbSensor->ActuatorsLst), (TStateDigitalDev)ON);
		}

		break;

	case 2:
		if(mbSensor->Sensor->Sensor.valor >= (int)*(mbSensor->ctrlLevelPtr->lSup)){
			state = 1;
			mbDomoboard.manager_mbActuators(&(mbSensor->ActuatorsLst), (TStateDigitalDev)OFF);
		}

		break;
	}

	mbSensor->Sensor->Sensor.Aux = state;
}

/*============================================*/
/*			     Control Persiana             */
/*============================================*/
void UpDown_Persiana(){
	bool UpP,DownP;
	tsStaPer state = (tsStaPer)(Aregs[MB_STAPER]&0xFF);

	UpP = (bool)mbDomoboard.PERUP.bkMBRegs[mbDomoboard.PERUP.nMBReg];
	DownP = (bool)mbDomoboard.PERDOWN.bkMBRegs[mbDomoboard.PERDOWN.nMBReg];

    switch(state){
    	case PER_STOP: //Parada
    		if (UpP == ON) {
    			state = PER_UP; //Subiendo
    		}

    		if (DownP == ON) {
    			state = PER_DOWN; //Bajando
    		}

    		break;

    	case PER_UP: //Subiendo
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}

    		if(!UpP && DownP){
    			state = PER_STOP2;
    		}
    		break;

    	case PER_DOWN: //Bajando
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}

    		if(UpP && !DownP){
    			state = PER_STOP2;
    		}
    		break;

    	case PER_STOP2:
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}
    		break;

    }

    if(Aregs[MB_STAPER] != state){
    	Aregs[MB_STAPER] = state;
    	writeholdingregister(MB_STAPER);
    }
}

void Persiana(TmbSensorPtr mbSensor){

	//Leemos estado actual de los actuadores
	for(int n = 0; n < mbSensor->ActuatorsLst.mbActuators.count(); n++){
 		mbActuator(mbSensor, n)->bkMBRegs[mbActuator(mbSensor, n)->nMBReg] = (hwpSensor(mbSensor).valor != hwpSensor(mbSensor).valor_Df);
	}

	UpDown_Persiana();
}

void Ctrl_PosicionPersiana(TctrlTimePtr ctrlPosPer, tsStaPer staPer){
	static tsStaPer lastStePer = PER_STOP;

	// lambda function
	auto actualizeTime = [&](int8_t sign){

		*ctrlPosPer->actTime = *ctrlPosPer->actTime + sign*(millis() - ctrlPosPer->time_lastAct);

		if(*ctrlPosPer->actTime < 0){
			*ctrlPosPer->actTime = 0;
		}else if(uint16_t(*ctrlPosPer->actTime) > *ctrlPosPer->maxTime){
			*ctrlPosPer->actTime = *ctrlPosPer->maxTime;
		}

		uint8_t per = PERCENTAGE((*ctrlPosPer->actTime)*100/(*ctrlPosPer->maxTime));

		if(per != Iregs[MB_POSPER]){
			Iregs[MB_POSPER] = per;
		}
	};

	switch(staPer){
	case PER_STOP:
	case PER_STOP2:
		ctrlPosPer->activa = false;
		switch(lastStePer){
		case PER_DOWN:
			actualizeTime(-1);
			break;
		case PER_UP:
			actualizeTime(1);
			break;
		default:
			break;
		}
		break;

	case PER_DOWN:
		if(ctrlPosPer->activa){
			//Continúa bajado
			actualizeTime(-1);
		}else{
			//Comienza a bajada
			ctrlPosPer->activa = true;
		}
		ctrlPosPer->time_lastAct = millis();
		break;

	case PER_UP:
		if(ctrlPosPer->activa){
			//Continúa subiendo
			actualizeTime(1);
		}else{
			//Comienza a subir
			ctrlPosPer->activa = true;
		}
		ctrlPosPer->time_lastAct = millis();
		break;
	}

	lastStePer = staPer;
}

/*============================================*/
/*			  Control Puerta Garage           */
/*============================================*/
void PuertaGarage(TmbSensorPtr mbSensor){
	static uint8_t state = 'A';
	tsStaPer statePer = (tsStaPer)(Aregs[MB_STAPER]&0xFF);

	TmbRelatedSensors *RelatedSensors = (TmbRelatedSensors *)mbSensor->RelatedSensors;

	// lambda function
	auto checkSensors = [&](){

		if(Cregs[MB_KEYGAR]){
			return true;
		}else if(mbSensor->ctrlTempPtr != NULL){
			//BTN1 Sensor
			if(mbSensor->Sensor->Sensor.valor != mbSensor->Sensor->Sensor.valor_Df){
				//Llave/Btn1 Accionado
				Cregs[MB_KEYGAR] = true;
				return true;
			}
		}else{
			//FhotoTTor Sensor

			uint16_t limInfe = *(mbSensor->ctrlLevelPtr->lInf);

			if(((uint16_t) mbSensor->Sensor->Sensor.valor) <= limInfe){
				//Barrera de luz interrumpida
				return true;
			}
		}

		return false;
	};

	// lambda function
	auto Init_timelastAct = [&](uint16_t *tmpAct = NULL){
		if(mbSensor->ctrlTempPtr != NULL){
			mbSensor->ctrlTempPtr->time_lastAct = millis();
			if(tmpAct != NULL)
				mbSensor->ctrlTempPtr->time_activo = tmpAct;

		}else if(RelatedSensors->mbRelatedSensors.count() > 0){
			RelatedSensors->mbRelatedSensors.peek(0)->ctrlTempPtr->time_lastAct = millis();
			if(tmpAct != NULL)
				RelatedSensors->mbRelatedSensors.peek(0)->ctrlTempPtr->time_activo = tmpAct;

		}
	};


	switch(state){
	case 'A':
		//Sólo se debe abrir con llave/Btn1
		if(mbSensor->ctrlTempPtr != NULL){
			if(mbSensor->bkMBRegs[mbSensor->nMBReg] || Cregs[MB_KEYGAR]){
				Cregs[MB_KEYGAR] = false;
				state = 'B';		//llave accionada, abrimos puerta
				statePer = PER_UP;
			}
		}
		break;

	case 'B':
		if(Iregs[MB_POSPER] >= 100){
			//Puerta abierta
			state = 'C';
			statePer = PER_STOP;

			Init_timelastAct((uint16_t *)&persianaTiempoAbierta);
		}
		break;

	case 'C':
		//Comprobar estado BTN1 y FotoTTOR
		if(checkSensors()){
			Cregs[MB_KEYGAR] = false;
			Init_timelastAct();
		}else{
			if(mbSensor->ctrlTempPtr != NULL){
				if((millis() - mbSensor->ctrlTempPtr->time_lastAct) > *mbSensor->ctrlTempPtr->time_activo){
					state = 'D';
					statePer = PER_DOWN;
				}
			}
		}
		break;

	case 'D':
		//Comprobar estado BTN1 y FotoTTOR
		if(checkSensors()){
			//Llave/Btn1 Accionada, o Barrera de luz (fotottor) interrumpida
			Cregs[MB_KEYGAR] = false;
			state = 'E';
			statePer = PER_STOP;

			//Inicia temporizador para esperar 1 segundo
			Init_timelastAct((uint16_t *)&persianaTiempoParada);

		}else{
			if(Iregs[MB_POSPER] == 0){
				state = 'A';
				statePer = PER_STOP;
			}
		}
		break;

	case 'E':
		if(mbSensor->ctrlTempPtr != NULL){
			if((millis() - mbSensor->ctrlTempPtr->time_lastAct) > *mbSensor->ctrlTempPtr->time_activo){
				state = 'B';
				statePer = PER_UP;
			}
		}
		break;

	default:
		break;
	}

	if(Aregs[MB_STAPER] != statePer){
		Aregs[MB_STAPER] = statePer;
		writeholdingregister(MB_STAPER);
	}
}


/*============================================*/
/*				     DIMMERS		          */
/*============================================*/
void ColaDimmers(void){
	for (int i = 0; i < funcDimmers.count(); i++){
		// Llama a eventos de la cola
		(funcDimmers.peek(i))();
	}
}



void gestorDimmer(void){

	if (listaDimmers.count() > 0){
		TmbDimmerPtr mbDimmer;
		for (int i = 0; i < listaDimmers.count(); i++){
			mbDimmer = listaDimmers.peek(i);

			// Si tiene actuador relacionado y está activado gestionar el dimmer
			if(&(mbDimmer->dimmer->actuator) != NULL && mbDimmer->dimmer->enabled){
				TDimmerPtr dimmer = mbDimmer->dimmer;

//				Serial.println(Iregs[mbDimmer->mbActuator.nMBReg]);
//				Serial.println(mbDomoboard.MBDIMMER1.mbActuator.bkMBRegs[mbDimmer->mbActuator.nMBReg]);
//				Serial.println(mbDomoboard.leerRegistroHolding(mbDimmer->mbActuator.nMBReg));
//				mbDomoboard.leerRegistroHolding(7);

				if ((dimmer->delay > ANTEFASE)){ // Apagar!
					digitalWrite(dimmer->actuator.pin, LOW);
			//		Serial.println("LOW");
				} else if(dimmer->delay > mbDimmer->mbActuator.bkMBRegs[mbDimmer->mbActuator.nMBReg] &&
						(dimmer->delay > TIEMPOFASE)
						){ // Encender!
					digitalWrite(dimmer->actuator.pin, HIGH);
					dimmer->activationVal = dimmer->delay;
					dimmer->pulso = true;
			//		Serial.println("HIGH");
				}

			//	// TODO Desactivar después de un cierto tiempo pequeño
//				if (dimmer->delay >= (dimmer->activationVal+4)){
//					digitalWrite(dimmer->actuator.pin, LOW);
////					Serial.println("LOW");
//				}
				dimmer->delay++;

			}
		}
	}
}


void ActivarDimmers(){
	//Activate timers
	attachInterrupt(0, pasoPorCero, RISING);
	funcDimmers.push(gestorDimmer);
	listaDimmers.push(&(mbDomoboard.MBDIMMER1));
	// todo arreglar los dos se puedan poner en la cola
//	listaDimmers.push(&(mbDomoboard.MBDIMMER2));
}

// Se activa con una interrupción cuando pasa por 0
void pasoPorCero(void){
	if (listaDimmers.count() > 0){
		TmbDimmerPtr mbDimmer;

		for (int i = 0; i < listaDimmers.count(); i++){
			mbDimmer = listaDimmers.peek(i);

			// Si tiene actuador relacionado y está activado inicializar el dimmer
			if (&(mbDimmer->dimmer->actuator) != NULL && mbDimmer->dimmer->enabled){
				ModbusDomoboard::InicializarTimerDimmer(mbDimmer->dimmer);
			}
		}

		gestorDimmer();
	}
}

void DimmerAnalogMap(TmbSensorPtr mbSensor){
	int valorSensor = 0;
	TmbActuatorPtr mbActuator;
	for(int i = 0; i<mbSensor->ActuatorsLst.mbActuators.count(); i++){
		mbActuator = mbSensor->ActuatorsLst.mbActuators.peek(i);

		// Si el valor está fuera de los valores permitidos se ha producido un error
		if(mbSensor->Sensor->Sensor.valor < 0 || mbSensor->Sensor->Sensor.valor > 1023)
			return;

		// Calcular valor 0-100
		valorSensor = map(mbSensor->Sensor->Sensor.valor, 0, 1023, 0, 100);


		// Guardar el valor en el banco de registros si ha habido suficiente movimiento
		if(abs((int)(mbActuator->bkMBRegs[mbActuator->nMBReg] - valorSensor)) >= 10){
			mbActuator->bkMBRegs[mbActuator->nMBReg] = valorSensor;

		}
	}


}



void DimmerAnalogMapFotores(TmbSensorPtr mbSensor){
	int valorSensor = 0;
	TmbActuatorPtr mbActuator;
	for(int i = 0; i<mbSensor->ActuatorsLst.mbActuators.count(); i++){
		mbActuator = mbSensor->ActuatorsLst.mbActuators.peek(i);


		// TODO hay errores con esto, si no funciona usar la funcion original
		// El valor puede ser menor que lInf o lSup pero tomaremos valores máximos o mínimos
		// Valor inferior -> 0
		if(mbSensor->Sensor->Sensor.valor < *mbSensor->ctrlLevelPtr->lInf)
			mbSensor->Sensor->Sensor.valor = *mbSensor->ctrlLevelPtr->lInf;

		// Valor superior -> 100
		if(mbSensor->Sensor->Sensor.valor > *mbSensor->ctrlLevelPtr->lSup)
					mbSensor->Sensor->Sensor.valor = *mbSensor->ctrlLevelPtr->lSup;


		// Calcular valor 0-100
		valorSensor = map(mbSensor->Sensor->Sensor.valor, *mbSensor->ctrlLevelPtr->lInf, *mbSensor->ctrlLevelPtr->lSup, 0, 100);


		// Guardar el valor en el banco de registros si ha habido suficiente movimiento
		if(abs((int)(mbActuator->bkMBRegs[mbActuator->nMBReg] - valorSensor)) >= 10){
			mbActuator->bkMBRegs[mbActuator->nMBReg] = valorSensor;

		}
	}


}

/*====================================================================================
 * Llamada regular usada para gestionar los temporizadores usados por la aplicación. =
 * es llamada cada LOOP_TIME (10 ms)
 *====================================================================================*/
void AccionesTemporizadas_Timer(void){
	for(int n=0; n<AccionesTemporizadas.count(); n++){
		AccionesTemporizadas.peek(n)->mbSensorEvent(&(AccionesTemporizadas.peek(n)->mbSensor));
	}
}


