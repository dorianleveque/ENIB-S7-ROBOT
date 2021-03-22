/*
 * IRMeasure.c
 *
 * Created: 20/06/2013 10:51:00
 *  Author: ziad, kerhoas
 *	kerhoas@enib.fr
 */ 

#include "IRMeasure.h"
#include <string.h>
#include "conf_board.h"


/**
 * Formula: ADCClock = MCK / ( (PRESCAL+1) * 2 )
 * For example, MCK = 64MHZ, PRESCAL = 4, then:
 * ADCClock = 64 / ((4+1) * 2) = 6.4MHz;
 */
/**
 * Formula:
 *     Startup  Time = startup value / ADCClock
 *     Startup time = 64 / 6.4MHz = 10 us
 */
#define MCK					(sysclk_get_cpu_hz())
#define PRESCAL				(4)
#define ADC_CLOCK			(MCK / ((PRESCAL+1) * 2))
#define ADC_STARTUP_TIME	(ADC_MR_STARTUP_SUT64 >> ADC_MR_STARTUP_Pos)

/*
 * We use two ADC channels for this example:
 *    ADC_CHANNEL_0  (IR Sensor Left)
 *    ADC_CHANNEL_1  (IR Sensor Right)
 */

/* Tracking Time*/
#define  TRACKING_TIME         1
/* Transfer Period */
#define  TRANSFER_PERIOD       1

/** Total number of ADC channels in use */
#define NUM_CHANNELS    2
/** ADC convention done mask. */
#define ADC_DONE_MASK   ( (1<<NUM_CHANNELS) - 1 )

/** Size of the receive buffer and transmit buffer. */
#define BUFFER_SIZE     NUM_CHANNELS

/** Reference voltage for ADC, in mv. */
#define VOLT_REF        (3300)
#define MAX_DIGITAL     (1023)

/** ADC channel for IR Sensors */
#define ADC_CHANNEL_IRLEFT   ADC_CHANNEL_0
#define ADC_CHANNEL_IRRIGHT  ADC_CHANNEL_1

#define USE_PDC		0
#define USE_SEQ		0
#define USE_PWR_SAV	0

#define IR_ERROR_VALUE UINT16_MAX

/************************************
*			METHODE : SCRUTATION
*************************************/

uint16_t IRMeasureInitPoll()
{
	/* Enable peripheral clock. */
	uint32_t i;
	pmc_enable_periph_clk(ID_ADC);
	/* Initialize ADC. */
	adc_init(ADC, MCK, ADC_CLOCK, ADC_STARTUP_TIME);
	/* Set ADC timing. */
	adc_configure_timing(ADC, TRACKING_TIME);
	ADC ->ADC_CHER=0x3;
	/* Configure trigger mode and start convention. */
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);	/* Disable hardware trigger. */
	return 0;
}

int GetDistancePoll(uint16_t* tab_dist) 
{	
	adc_start(ADC);
	while((adc_get_status(ADC) & 0x3) != 0x3)
	{}
	tab_dist[0] = ADC->ADC_CDR[0];
	tab_dist[1] = ADC->ADC_CDR[1];
	return 0;	
}