#include <asf.h>
#include "conf_board.h"

#include "drivers/PWMCommon.h"
#include "drivers/MotorCommand.h"
#include "drivers/QuadEncoder.h"
#include "drivers/ServoCommand.h"
#include "drivers/IRMeasure.h"
#include "drivers/Serial.h"

#define TASK_A_STACK_SIZE            (2048/sizeof(portSTACK_TYPE))
#define TASK_A_STACK_PRIORITY        tskIDLE_PRIORITY+1
#define TASK_B_STACK_SIZE            (1024/sizeof(portSTACK_TYPE))
#define TASK_B_STACK_PRIORITY        tskIDLE_PRIORITY
#define TASK_C_STACK_SIZE            (2048/sizeof(portSTACK_TYPE))
#define TASK_C_STACK_PRIORITY        (tskIDLE_PRIORITY+1)
#define TASK_D_STACK_SIZE            (1024/sizeof(portSTACK_TYPE))
#define TASK_D_STACK_PRIORITY        (tskIDLE_PRIORITY)
#define TASK_E_STACK_SIZE            (2048/sizeof(portSTACK_TYPE))
#define TASK_E_STACK_PRIORITY        (tskIDLE_PRIORITY+1)
#define TASK_F_STACK_SIZE            (1024/sizeof(portSTACK_TYPE))
#define TASK_F_STACK_PRIORITY        (tskIDLE_PRIORITY)

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void xPortSysTickHandler(void);

xSemaphoreHandle xSemaphore = NULL;
xQueueHandle qh = NULL;
char* p_buf;

struct AMessage
{
	char command;
	int data;
};

/**
 * \brief Handler for Sytem Tick interrupt.
 */
void SysTick_Handler(void)
{
	xPortSysTickHandler();
}

/**
 * \brief Called if stack overflow during execution
 */
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed char *pcTaskName)
{
	printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;) {
	}
}

/**
 * \brief This function is called by FreeRTOS idle task
 */
extern void vApplicationIdleHook(void)
{
}

/**
 * \brief This function is called by FreeRTOS each tick
 */
extern void vApplicationTickHook(void)
{
}


static void task_A(void *pvParameters)
{
	int i=0;
	UNUSED(pvParameters);
	for (;;) 
	{		
		puts("TASK A \n\r");		
		vTaskDelay(1000);  // 1000ms
	}
}

static void task_B(void *pvParameters)
{
	UNUSED(pvParameters);
	for (;;) {
		puts("TASK B \n\r");

	}
}

static void task_C(void *pvParameters)
{
	UNUSED(pvParameters);
	for (;;)
	{
		puts("TASK C \n\r");		
		xSemaphoreTake( xSemaphore, portMAX_DELAY );
	}
}

static void task_D(void *pvParameters)
{
	UNUSED(pvParameters);
	for (;;) 
	{	
		puts("TASK D \n\r");
		xSemaphoreGive( xSemaphore );
	}
}



static void task_E(void *pvParameters)
{
	struct AMessage pxMessage;
	pxMessage.command='a';
	pxMessage.data=10;
	vTaskDelay(1000);
	UNUSED(pvParameters);
	for (;;)
	{
		puts("TASK E \n\r");
		xQueueSend( qh, ( void * ) &pxMessage,  portMAX_DELAY );			
		xSemaphoreTake( xSemaphore, portMAX_DELAY );	
		vTaskDelay(1000);
	}
}

char* msg_rx;

static void task_F(void *pvParameters)
{
	struct AMessage pxRxedMessage;
	UNUSED(pvParameters);
	for (;;) 
	{	
		xQueueReceive( qh,  &( pxRxedMessage ) , portMAX_DELAY );
		puts("TASK F \n\r");
		xSemaphoreGive( xSemaphore );
	}
}


int main(void)
{	
	/* Initilize the SAM system */
	PWMClockInit();
	MotorCommandInit();
	QuadEncoderInit();
	ServoCommandInit();
	UartInit();
	IRMeasureInitPoll();
	
	/* Output example information */
	puts(STRING_HEADER);

	if (xTaskCreate(task_A, "task_A", TASK_A_STACK_SIZE, NULL, TASK_A_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create task A\r\n"); }

	if (xTaskCreate(task_B, "task_B", TASK_B_STACK_SIZE, NULL, TASK_B_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create task B\r\n"); }
		
/*	if (xTaskCreate(task_C, "task_C", TASK_C_STACK_SIZE, NULL, TASK_C_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create task C\r\n"); }

	if (xTaskCreate(task_D, "task_D", TASK_D_STACK_SIZE, NULL, TASK_D_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create task D\r\n"); }	
*/
/*	if (xTaskCreate(task_E, "task_E", TASK_E_STACK_SIZE, NULL, TASK_E_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create task C\r\n"); }

	if (xTaskCreate(task_F, "task_F", TASK_F_STACK_SIZE, NULL, TASK_F_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create task D\r\n"); }
*/				
	vSemaphoreCreateBinary(xSemaphore);	
	xSemaphoreTake( xSemaphore, portMAX_DELAY );
	
	// Create a queue capable of containing 10 unsigned long values.
    qh = xQueueCreate( 1, sizeof(struct AMessage ) );

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle task. */
	return 0;
}
