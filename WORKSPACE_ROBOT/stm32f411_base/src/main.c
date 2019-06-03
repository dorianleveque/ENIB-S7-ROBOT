#include "main.h"


//#######################################################
#define EX1 1
#define EX2 2
#define EX3 3

#define SYNCHRO_EX EX1

//#######################################################

// Déclaration des objets synchronisants !! Ne pas oublier de les créer
xSemaphoreHandle xSemaphore = NULL;
xQueueHandle qh = NULL;


extern uint8_t rec_buf2[NB_CAR_TO_RECEIVE+1];	 // defined in drv_uart.c
extern uint8_t rec_buf6[NB_CAR_TO_RECEIVE+1];

struct AMessage
{
	char command;
	int data;
};

//========================================================
#if SYNCHRO_EX == EX1

int dist_IR[2];
int consigne;
int cam_status = 1; // 0: ok, 1: research, 2: error
int cam_cmd_x = 0;
int cam_cmd_y = 0;

int led = 0;

static void task_A(void *pvParameters)
{


	for (;;)
	{
		//term_printf("TASK A \n\r");

		// DISTANCE

		captDistIR_Get(dist_IR);

		//term_printf("dist_G: %d \t|\t dist_D: %d \n\r", dist_IR[0], dist_IR[1]);
		/*
		consigne = (int)(dist_IR[1]*200/3200);
		if (consigne>200) {
			consigne = 200;
		}
		motorRight_SetDuty(consigne);

		int commandeServo;
		commandeServo= dist_IR[1] *120/3200;

		if(commandeServo > 120)
		{
			commandeServo = 120;
		}

		if(commandeServo < 20)
		{
			commandeServo = 20;
		}

		servoHigh_Set(commandeServo);
		servoLow_Set(commandeServo);

		*/

		//pixyCam_SetLED(0,255,0);

		uint16_t blockPosition[2], blockSize[2];
		pixyCam_Get(blockPosition, blockSize);

		//term_printf("x: %d, y: %d, w: %d, h: %d\n", blockPosition[0], blockPosition[1], blockSize[0], blockSize[1]);

		//servoLow_Set(120);
		//tracking();
		//pixyCam_Test();
		vTaskDelay(50); // 1000 ms

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
		xQueueSend( qh, ( void * ) &pxMessage,  portMAX_DELAY );
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
#endif

	vSemaphoreCreateBinary(xSemaphore);
	xSemaphoreTake( xSemaphore, portMAX_DELAY );

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

		 HAL_UART_Receive_IT(&Uart2Handle, (uint8_t *)rec_buf2, NB_CAR_TO_RECEIVE);
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

void tracking()
{
	uint16_t XCamWidth, YCamHeight, XCamCenter, YCamCenter;
	int XDiff, YDiff;
	XCamWidth = 260;
	YCamHeight = 280;

	uint16_t blockPosition[2], blockSize[2];
	pixyCam_Get(blockPosition, blockSize);
	uint16_t xObject, yObject, wObject, hObject, xObjectCenter, yObjectCenter;
	xObject = blockPosition[0];
	yObject = blockPosition[1];
	wObject = blockSize[0];
	hObject = blockSize[1];

	// calcul center of object
	xObjectCenter = (uint16_t) (xObject + wObject)/2;
	yObjectCenter = (uint16_t) (yObject + hObject)/2;

	// calcul center of cam window
	XCamCenter = (uint16_t) (XCamWidth)/2;
	YCamCenter = (uint16_t) (YCamHeight)/2;

	// calcul diff between object and cam center
	XDiff = xObjectCenter - XCamCenter;
	YDiff = yObject - YCamCenter;

	term_printf("XDiff: %d, YDiff: %d\n", XDiff, YDiff);


	if (XDiff>0) {
		cam_cmd_x --;
	}
	if (XDiff<0) {
		cam_cmd_x ++;
	}

	if (YDiff>0) {
		cam_cmd_y --;
	}
	if (YDiff<0) {
		cam_cmd_y ++;
	}

	/*if (XDiff>0 && YDiff>0) {
		term_printf("BAS DROITE\n");
	}
	if (XDiff<0 && YDiff>0) {
		term_printf("BAS GAUCHE\n");
	}
	if (XDiff<0 && YDiff<0) {
		term_printf("HAUT GAUCHE\n");
	}
	if (XDiff>0 && YDiff<0) {
		term_printf("HAUT DROITE\n");
	}*/

	servoLow_Set(cam_cmd_x+70);
	servoHigh_Set(cam_cmd_y+70);
	// Si bloc pas detecte ou trop petit
		// Chercher
	// Si bloc suffisament grand, orienter les servos

}
