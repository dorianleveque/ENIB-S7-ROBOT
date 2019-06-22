#include "main.h"


//#######################################################
#define EX1 1
#define EX2 2
#define EX3 3
#define US 4

#define SYNCHRO_EX US

#define ID_ROUES 'R'
#define ID_ROUES_UP 'A'
#define ID_ROUES_DOWN 'R'
#define ID_ROUES_LEFT 'G'
#define ID_ROUES_RIGHT 'D'
#define ID_ROUES_STOP 'S'

#define ID_TOURELLE 'T'
#define ID_TOURELLE_UP 'A'
#define ID_TOURELLE_DOWN 'R'
#define ID_TOURELLE_LEFT 'G'
#define ID_TOURELLE_RIGHT 'D'
#define ID_TOURELLE_STOP 'S'

#define ID_CONSIGNE 'C'

//#######################################################

// Déclaration des objets synchronisants !! Ne pas oublier de les créer
xSemaphoreHandle xSemaphore = NULL;
xSemaphoreHandle semA = NULL;
xSemaphoreHandle mutex = NULL;

xQueueHandle qh = NULL;

extern uint8_t rec_buf2[NB_CAR_TO_RECEIVE+1];	 // defined in drv_uart.c
extern uint8_t rec_buf6[NB_CAR_TO_RECEIVE+1];

struct AMessage
{
	char command;
	int data;
};

struct MotorCmdMsg
{
	int motorCmdLeft;
	int motorCmdRight;
};

enum direction_state
{
	stop = 0,
	avant = 1,
	gauche = 2,
	droite = 3,
	arriere = 4
};
int current_dirrection = stop;

enum mode
{
	mode_auto = 0,
	mode_cmd = 1,
	mode_demo = 2
};
int current_mode = mode_auto;

int dist_IR[2];

int cam_cmd_x = 70;
int cam_cmd_y = 70;

int speed_motor_cmd = 400;

float Kp_L = 0.005;
float Ki_L = 0.3; //0.003/(0.1*0.02) // Te/(

float Kp_R = 0.005;
float Ki_R = 0.3;//0.3 0.75; //0.003/(0.1*0.02) // Te/(

int Te = 3; //3ms

int test_cmd[1000];
int test_index=0;

//========================================================
#if SYNCHRO_EX == EX1

static void task_A(void *pvParameters)
{
	uint8_t buttons=0;

	for (;;)
	{
		//term_printf("TASK A \n\r");




		/*tracking();

		servoLowValue = servoLow_Get();
		if (servoLowValue < 40) // right side
		{
			asservRightMotor(speed_motor_cmd-100);
		}
		else {
			asservRightMotor(speed_motor_cmd);
		}


		if (servoLowValue > 100) // left side
		{
			asservLeftMotor(speed_motor_cmd-100);
		}
		else {
			asservLeftMotor(speed_motor_cmd);
		}
		//term_printf("value: %d\n\r", servoLowValue);*/

		buttons=screenLCD_ReadButtons();
		term_printf("buttons = %d \n\r",buttons);

		vTaskDelay(70); // 1000 ms

	}

}

static void task_B(void *pvParameters)
{
	for (;;)
	{
		//term_printf("TASK B \n\r");
	}
}
//========================================================
#elif SYNCHRO_EX == EX2

static void task_C( void *pvParameters )
{
	for (;;)
	{
		term_printf("TASK C \n\r");
		xSemaphoreTake( xSemaphore, portMAX_DELAY );
	}
}

static void task_D( void *pvParameters )
{
	for (;;)
	{
		term_printf("TASK D \n\r");
		xSemaphoreGive( xSemaphore );
	}
}

//========================================================
#elif SYNCHRO_EX == EX3

static void task_E( void *pvParameters )
{
	struct AMessage pxMessage;
	pxMessage.command='a';
	pxMessage.data=10;
	vTaskDelay(1000);
	for (;;)
	{
	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
		term_printf("TASK E \n\r");
		//xQueueSend( qh, ( void * ) &pxMessage,  portMAX_DELAY );
		xSemaphoreTake( xSemaphore, portMAX_DELAY );
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
		vTaskDelay(SAMPLING_PERIOD_ms);

	}
}

static void task_F(void *pvParameters)
{
	struct AMessage pxRxedMessage;

	for (;;)
	{
		xQueueReceive( qh,  &( pxRxedMessage ) , portMAX_DELAY );
		term_printf("TASK F \n\r");
		xSemaphoreGive( xSemaphore );
	}
}
#elif SYNCHRO_EX == US

	static void task_action( void *pvParameters )
	{
		struct MotorCmdMsg pxRcvMsg;

		int orderMotorLeft=0;
		int speedMotorLeft = 0;
		static float errorLeft = 0;
		static float upLeft = 0;
		static float uiLeft = 0;
		int cmdMotorLeft = 0;

		int orderMotorRight=0;
		int speedMotorRight= 0;
		static float errorRight = 0;
		static float upRight = 0;
		static float uiRight = 0;
		int cmdMotorRight = 0;

		int corr = 0;
		for(;;)
		{
			xQueueReceive( qh,  &( pxRcvMsg ) , portMAX_DELAY );
			orderMotorLeft = pxRcvMsg.motorCmdLeft;
			orderMotorRight= pxRcvMsg.motorCmdRight;

			// move cam to point to the correct color
			tracking();

			// get motors speed
			speedMotorLeft = quadEncoder_GetSpeedL();
			speedMotorRight= quadEncoder_GetSpeedR();

			// motors slip correction
			corr = (speedMotorLeft - speedMotorRight)*18;

			// left motor speed control
			if (current_dirrection == avant)
				errorLeft = orderMotorLeft - corr - speedMotorLeft;
			else
				errorLeft = orderMotorLeft - speedMotorLeft;

			upLeft = Kp_L * errorLeft;
			uiLeft = Ki_L * upLeft + uiLeft;
			cmdMotorLeft = 100 + uiLeft + upLeft;

			if (cmdMotorLeft > 200) { cmdMotorLeft = 200; }
			if (cmdMotorLeft < 0)   { cmdMotorLeft = 0;   }
			motorLeft_SetDuty(cmdMotorLeft);



			// right motor speed control
			if (current_dirrection == avant)
				errorRight = orderMotorRight + corr - speedMotorRight;
			else
				errorRight = orderMotorRight - speedMotorRight;

			upRight = Kp_R * errorRight;
			uiRight = Ki_R * upRight + uiRight;
			cmdMotorRight = 100 + uiRight + upRight;

			if (cmdMotorRight > 200) { cmdMotorRight = 200; }
			if (cmdMotorRight < 0)   { cmdMotorRight = 0;   }
			motorRight_SetDuty(cmdMotorRight);



			term_printf("errL: %d, errR: %d, corr: %d\n\r", errorLeft, errorRight, corr);
			xSemaphoreGive( semA );  // give the hand to decision task
		}
	}

	static void task_decision( void *pvParameters )
	{
		int loop = 0; //70   -> avec vtaskdelay => 24 loop
		struct MotorCmdMsg pxMsg;
		pxMsg.motorCmdLeft=0; //speed
		pxMsg.motorCmdRight=0; //speed

		// sensors variables
		int ahead_dist_left = 0;
		int ahead_dist_right= 0;
		int back_dist_left 	= 0;
		int back_dist_right	= 0;
		int cam_angle = 0;

		// launch sensors measure
		captDistUS_Measure(0xE0);
		captDistUS_Measure(0xE2);
		vTaskDelay(Te);//Te, necessary for motor speed control
		for(;;)
		{
			//term_printf("TASK SENSORS %d, %d\n\r", dist_AR_L, dist_AR_R);

			/************* UPDATE ALL SENSORS DATA **************/

			// update ahead distance
			captDistIR_Get(dist_IR);
			ahead_dist_left = dist_IR[0];
			ahead_dist_right= dist_IR[1];

			// update back distance
			if (loop>Te*8) { //Te*8
				xSemaphoreTake( mutex, portMAX_DELAY );

				// get back distance
				back_dist_right = captDistUS_Get(0xE0);
				back_dist_left 	= captDistUS_Get(0xE2);

				// launch new sensors measure
				captDistUS_Measure(0xE0);
				captDistUS_Measure(0xE2);

				xSemaphoreGive( mutex );
				loop = 0;
			}
			loop++;

			// update cam angle
			cam_angle = servoLow_Get();
			/************* END UPDATE ALL SENSORS DATA **************/

			/************* DECISION **************/

			if (cam_angle < 60) {
				current_dirrection = gauche;
				pxMsg.motorCmdLeft = 200;
				pxMsg.motorCmdRight = 400;
			}
			else if (cam_angle > 80) {
				current_dirrection = droite;
				pxMsg.motorCmdRight = 200;
				pxMsg.motorCmdLeft = 400;
			}
			else {
				current_dirrection = avant;
				pxMsg.motorCmdLeft = speed_motor_cmd;
				pxMsg.motorCmdRight = speed_motor_cmd;
			}

			//term_printf("distR: %d, distL: %d \n\r", dist_AR_R, dist_AR_L);

			xQueueSend( qh, ( void * ) &pxMsg,  portMAX_DELAY ); // give the hand to action task
			xSemaphoreTake( semA, portMAX_DELAY );  // take the hand to action task
			
			vTaskDelay(Te); // Te
		}
	}

	static void task_btn( void *pvParameters )
	{
		for(;;)
		{
			xSemaphoreTake( mutex, portMAX_DELAY );
			//term_printf("TASK BTN \n\r");

			uint8_t buttons=0;
			buttons=screenLCD_ReadButtons();
			//term_printf("buttons = %d \n\r",buttons);

			switch(buttons){
			case 14:
				current_mode = mode_auto;
				break;
			case 13:
				current_mode = mode_cmd;
				break;
			case 11:
				current_mode = mode_demo;
				break;
			/*case 7:
				break;*/
			}

			//vTaskDelay(10); // donne la main à une tache libre (task lcd)
			//vTaskDelay(Te);
			xSemaphoreGive( mutex ); // redonne la main à motor
		}
	}

	static void task_lcd( void *pvParameters )
	{
		char mes1[16];
		char mes2[16];
		for(;;)
		{
			xSemaphoreTake( mutex, portMAX_DELAY );
			//term_printf("TASK LCD \n\r");
			switch(current_dirrection)
			{
			case stop:
				sprintf(mes1,"DIR:STOP   ");
				break;
			case avant:
				sprintf(mes1,"DIR:MARCHE ");
				break;
			case arriere:
				sprintf(mes1,"DIR:ARRIERE");
				break;
			case gauche:
				sprintf(mes1,"DIR:GAUCHE ");
				break;
			case droite:
				sprintf(mes1,"DIR:DROITE ");
				break;
			}

			switch(current_mode)
			{
			case mode_auto:
				sprintf(mes2,"MODE:AUTO");
				break;
			case mode_cmd:
				sprintf(mes2,"MODE:CMD");
				break;
			case mode_demo:
				sprintf(mes2,"MODE:DEMO");
				break;
			}
			screenLCD_SetBacklight(200);
			//screenLCD_Write(mes1, 12,  0, 1);
			//screenLCD_Write(mes2, 12,  0, 0);

			//vTaskDelay(3);
			xSemaphoreGive( mutex );
		}
	}


#endif
//=========================================================
//	>>>>>>>>>>>>	MAIN	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//=========================================================

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	uart2_Init();			// CABLE
	uart6_Init();			// ZIGBEE
	i2c1_Init();
	spi1Init();
	pixyCam_Init();			// Caméra Pixy
	captDistIR_Init();		// Capteurs Infrarouge
	quadEncoder_Init();		// Encodeurs Incrémentaux
	motorCommand_Init();	// Commande des Hacheurs
	servoCommand_Init();	// Commande des Servomoteurs

	HAL_Delay(1000);

	motorLeft_SetDuty(100);
	motorRight_SetDuty(100);

#if SYNCHRO_EX == EX1
	xTaskCreate( task_A, ( const portCHAR * ) "task A", 512 /* stack size */, NULL, tskIDLE_PRIORITY+2, NULL );
	xTaskCreate( task_B, ( const portCHAR * ) "task B", 512 /* stack size */, NULL, tskIDLE_PRIORITY+1, NULL );
#elif SYNCHRO_EX == EX2
	xTaskCreate( task_C, ( signed portCHAR * ) "task C", 512 /* stack size */, NULL, tskIDLE_PRIORITY+2, NULL );
	xTaskCreate( task_D, ( signed portCHAR * ) "task D", 512 /* stack size */, NULL, tskIDLE_PRIORITY+1, NULL );
#elif SYNCHRO_EX == EX3
	xTaskCreate( task_E, ( signed portCHAR * ) "task E", 512 /* stack size */, NULL, tskIDLE_PRIORITY+2, NULL );
	xTaskCreate( task_F, ( signed portCHAR * ) "task F", 512 /* stack size */, NULL, tskIDLE_PRIORITY+1, NULL );
#elif SYNCHRO_EX == US
	xTaskCreate( task_decision, ( signed portCHAR * ) "task Decision", 512, NULL, tskIDLE_PRIORITY+3, NULL );
	xTaskCreate( task_action,	( signed portCHAR * ) "task Action"	, 512, NULL, tskIDLE_PRIORITY+2, NULL );
	xTaskCreate( task_lcd, 		( signed portCHAR * ) "task lcd"	, 512, NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( task_btn, 		( signed portCHAR * ) "task Btn"	, 512, NULL, tskIDLE_PRIORITY+1, NULL );

#endif

	//vSemaphoreCreateBinary(xSemaphore);
	vSemaphoreCreateBinary(semA);
	vSemaphoreCreateBinary(mutex);
	//mutex = xSemaphoreCreateMutex();

	xSemaphoreTake( semA, portMAX_DELAY );

	qh = xQueueCreate( 1, sizeof(struct AMessage ) );

	vTaskStartScheduler();

	return 0;

}


//=================================================================
//	UART RECEIVE CALLBACK5
//=================================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle -> Instance == USART2)
	{

		 HAL_UART_Receive_IT(&Uart2Handle, (uint8_t *)rec_buf2, 3); // 3 est le nombre de caracteres pour déclencher l'interupt
		 if(*rec_buf2 == 0x41) // == A
		 {
			 servoHigh_Set(20);
		 }
		 else if(*rec_buf2 == 0x52) // == R
		 {
			 servoHigh_Set(120);
		 }
		 else if(*rec_buf2 == 0x47) // == G
		 {
			 servoLow_Set(120);
		 }
		 else if(*rec_buf2 == 0x44) // == D
		 {
			 servoLow_Set(20);
		 }

		 else if(*rec_buf2 == 0x53) // == S
		 {
			 servoHigh_Set(70);
			 servoLow_Set(70);
		 }
		 else
		 {
			 // Nothing to do
		 }

	}

	if(UartHandle -> Instance == USART6)
	{

		 HAL_UART_Receive_IT(&Uart6Handle, (uint8_t *)rec_buf6, NB_CAR_TO_RECEIVE);
		 if(*rec_buf2 == 0x5A) // == Z
		 {
			 servoHigh_Set(20);
		 }
		 servoLow_Set(120);
	}

}



//=================================================================
// Called if stack overflow during execution
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed char *pcTaskName)
{
	//term_printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;) {
	}
}
//=================================================================
//This function is called by FreeRTOS idle task
extern void vApplicationIdleHook(void)
{
}
//=================================================================
// brief This function is called by FreeRTOS each tick
extern void vApplicationTickHook(void)
{
//	HAL_IncTick();
}


/*void asservLeftMotor(int consigne)
{
	int speedMotorL = quadEncoder_GetSpeedL();

	static float erreur = 0;
	static float up = 0;
	static float ui = 0;

	//erreur = consigne - sup_glissement() - speed;
	erreur = consigne - speed;
	erreurL=erreur;


	up = Kp_L * erreur;
	ui = Ki_L * up + ui;
	motor_left_cmd = 100 + ui + up;

	if (motor_left_cmd > 200)  { motor_left_cmd = 200; }
	if (motor_left_cmd < 0) 	{ motor_left_cmd = 0;   }

	motorLeft_SetDuty(motor_left_cmd);
}

void asservRightMotor(int consigne)
{
	int speed = quadEncoder_GetSpeedR();

	static float erreur = 0;
	static float up = 0;
	static float ui = 0;
	sup_glissement();
	term_printf("errL: %d, errR: %d, corr: %d\n\r", speed, speed, speed);
	erreur = consigne - speed;
	//erreur = consigne + sup_glissement() - speed;
	erreurR=erreur;


	up = Kp_R * erreur;
	ui = Ki_R * up + ui;
	motor_right_cmd = 100 + ui + up;

	if (motor_right_cmd > 200)  { motor_right_cmd = 200; }
	if (motor_right_cmd < 0) 	{ motor_right_cmd = 0;   }

	if( test_index < 1000)
	{
	test_cmd[test_index] = speed;
	test_index++;
	}

	motorRight_SetDuty(motor_right_cmd);
}

int sup_glissement()
{
	int speedR = quadEncoder_GetSpeedR();
	int speedL = quadEncoder_GetSpeedL();
	int corr = (speedL - speedR);
	//term_printf("errL: %d, errR: %d, corr: %d\n\r", speedL, speedR, corr);
	return corr;// speed corrected
}*/


void tracking()
{
	static int32_t nbBlock = -1;
	nbBlock = pixyCam_GetBlocks(1);

	if (nbBlock) // if an object is detected
	{
		uint16_t XCamWidth, YCamHeight, XCamCenter, YCamCenter;
		int XDiff, YDiff;
		XCamWidth = 260;
		YCamHeight = 280;

		uint16_t blockPosition[2], blockSize[2];  // position x, y and width, height
		pixyCam_Get(blockPosition, blockSize);

		uint16_t xObject, yObject;
		xObject = blockPosition[0];  // center of object
		yObject = blockPosition[1];

		// calcul center of cam window
		XCamCenter = (uint16_t) (XCamWidth)/2;
		YCamCenter = (uint16_t) (YCamHeight)/2;

		// calcul diff between object and cam center
		XDiff = xObject - XCamCenter;
		YDiff = yObject - YCamCenter;

		//term_printf("XDiff: %d, YDiff: %d\n", XDiff, YDiff);


		if (XDiff>20 && cam_cmd_x>20) {
			cam_cmd_x --;
		}
		if (XDiff<-20 && cam_cmd_x<120) {
			cam_cmd_x ++;
		}

		if (YDiff>20 && cam_cmd_y>20) {
			cam_cmd_y ++;
		}
		if (YDiff<-20 && cam_cmd_y<120) {
			cam_cmd_y --;
		}

		servoLow_Set(cam_cmd_x);
		servoHigh_Set(cam_cmd_y);
	}
}
