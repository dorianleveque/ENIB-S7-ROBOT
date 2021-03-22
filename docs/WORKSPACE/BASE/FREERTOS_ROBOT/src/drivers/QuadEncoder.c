/*
 * QuadEncoder.c
 *
 * Created: 20/06/2013 10:46:34
 * Authors: kerhoas, ziad
 */ 

#include "QuadEncoder.h"

#define COUNT_PER_ROUND_PHA		1250*2
#define COUNT_PER_ROUND			COUNT_PER_ROUND_PHA

uint16_t SpeedHardL[500];
uint16_t SpeedHardR[500];

void TC5_Handler( void )  // Capture Speed Hard Right
{ static int i=0;
	
	if(i < 500)
	{
		SpeedHardR[i]=(uint16_t)(tc_read_ra(TC1, 0) & 0xFFFF);
		
		i++;
	}
	
	tc_get_status(TC1,2);
	tc_start(TC1, 2); 
	
}

void TC2_Handler( void )  // Capture Speed Hard Left
{ static int i=0;
	
	if(i < 500)
	{
		SpeedHardL[i]=(uint16_t)(tc_read_ra(TC0, 0) & 0xFFFF);
		
		i++;
	}
	
	tc_get_status(TC0,2);
	tc_start(TC0, 2); 
	
}

int QuadEncoderInit() 
{
	int mck=0;
	
	gpio_configure_pin(PIN_TC0_TIOA0, PIN_TC0_TIOA0_FLAGS);
	gpio_configure_pin(PIN_TC0_TIOB0, PIN_TC0_TIOB0_FLAGS);
	//gpio_configure_pin(PIN_TC0_TIOA1, PIN_TC0_TIOA1_FLAGS);
	gpio_configure_pin(PIN_TC0_TIOB1, PIN_TC0_TIOB1_FLAGS);
	
	sysclk_enable_peripheral_clock(ID_TC0);
	sysclk_enable_peripheral_clock(ID_TC1);
	sysclk_enable_peripheral_clock(ID_TC2);
	//-----------------------------------------------------------------------------------------
	#if SPEED_HARDWARE_MEASURE
	tc_set_block_mode(TC0, 
			TC_BMR_QDEN  | TC_BMR_SPEEDEN | TC_BMR_FILTER | TC_BMR_MAXFILT(10)); 
	#else	
	tc_set_block_mode(TC0, 
			TC_BMR_QDEN  | TC_BMR_POSEN	|TC_BMR_FILTER | TC_BMR_MAXFILT(10)); 
	#endif
	//-----------------------------------------------------------------------------------------
	
	//-----------------------------------------------------------------------------------------		 
	#if SPEED_HARDWARE_MEASURE		 
 	tc_init(TC0, 2,			// TC0 channel 2
 			TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPC_TOGGLE | TC_CMR_TCCLKS_TIMER_CLOCK3 ); // Timer Clock3 : MCK/32
	tc_init(TC0, 0,			// TC0 channel 0
			TC_CMR_TCCLKS_XC0| TC_CMR_ABETRG | TC_CMR_ETRGEDG_RISING| TC_CMR_LDRA_RISING | TC_CMR_LDRB_RISING);		
	tc_enable_interrupt(TC0,2,1<<4);

	/* Enable UART interrupt */
	NVIC_EnableIRQ(TC2_IRQn);		 	 
			 
	mck=sysclk_get_cpu_hz();
 	tc_write_rc(TC0, 2, mck/(32*1000*2));  // 1ms
	tc_start(TC0, 2); 
	 
	#else
	tc_init(TC0, 0,			// TC0 channel 0
			TC_CMR_TCCLKS_XC0| TC_CMR_ABETRG | TC_CMR_ETRGEDG_RISING);

	#endif
	
		tc_init(TC0, 1,			// TC0 channel 1
			 TC_CMR_TCCLKS_XC0); 
	//----------------------------------------------------------------------------------
	tc_start(TC0, 0);
	tc_start(TC0, 1);

	//==================================================================================
	
	gpio_configure_pin(PIN_TC1_TIOA3, PIN_TC1_TIOA3_FLAGS);
	gpio_configure_pin(PIN_TC1_TIOB3, PIN_TC1_TIOB3_FLAGS);
	gpio_configure_pin(PIN_TC1_TIOB4, PIN_TC1_TIOB4_FLAGS);
	
	sysclk_enable_peripheral_clock(ID_TC3);
	sysclk_enable_peripheral_clock(ID_TC4);
	sysclk_enable_peripheral_clock(ID_TC5);
	
		
	#if SPEED_HARDWARE_MEASURE
	tc_set_block_mode(TC1,
	TC_BMR_QDEN  | TC_BMR_SPEEDEN | TC_BMR_FILTER | TC_BMR_MAXFILT(10));
	#else
	tc_set_block_mode(TC1,
	TC_BMR_QDEN  | TC_BMR_POSEN	|TC_BMR_FILTER | TC_BMR_MAXFILT(10)); 
	#endif
	

	#if SPEED_HARDWARE_MEASURE
	tc_init(TC1, 2,			// TC0 channel 2
	TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPC_TOGGLE | TC_CMR_TCCLKS_TIMER_CLOCK3 ); // Timer Clock3 : MCK/32
	tc_init(TC1, 0,			// TC0 channel 0
	TC_CMR_TCCLKS_XC0| TC_CMR_ABETRG | TC_CMR_ETRGEDG_RISING| TC_CMR_LDRA_RISING | TC_CMR_LDRB_RISING);
	tc_enable_interrupt(TC1,2,1<<4);

	/* Enable UART interrupt */
	NVIC_EnableIRQ(TC5_IRQn);
	
	mck=sysclk_get_cpu_hz();
	tc_write_rc(TC1, 2, mck/(32*1000*2));  // 1ms
	tc_start(TC1, 2);
	
	#else
	tc_init(TC1, 0,			// TC0 channel 0
	TC_CMR_TCCLKS_XC0| TC_CMR_ABETRG | TC_CMR_ETRGEDG_RISING);

	#endif

	tc_init(TC1, 1,		// TC1 channel 1
			TC_CMR_TCCLKS_XC0);
	
	tc_start(TC1, 0);
	tc_start(TC1, 1);
	tc_start(TC1, 2);
	
	return 1;
}


uint16_t CapturePosition16L(void) 
{	
	uint16_t PositionL = 0;
 	PositionL = (uint16_t)(tc_read_cv(TC0, 0) & 0xFFFF);
	return PositionL;
}

uint16_t CapturePosition16R(void) 
{
	uint16_t PositionR = 0;
 	PositionR = (uint16_t)(tc_read_cv(TC1, 0) & 0xFFFF);
	return PositionR;
}

uint32_t CapturePosition32L(void) 
{
	uint32_t PositionL = 0;
	PositionL = (uint32_t)((uint32_t)(tc_read_cv(TC0, 1) * COUNT_PER_ROUND) + (uint32_t)(tc_read_cv(TC0, 0) & 0xFFFF));
	//PositionL = (uint32_t)(tc_read_cv(TC0, 1) );	
	return PositionL;
}

uint32_t CapturePosition32R(void) 
{
	uint32_t PositionR = 0;	
	PositionR = (uint32_t)((uint32_t)(tc_read_cv(TC1, 1) * COUNT_PER_ROUND) | (uint32_t)(tc_read_cv(TC1, 0) & 0xFFFF));	
	return PositionR;
}

int16_t CaptureSpeedSoftL(void)
{
	static int16_t SpeedSoftL=0;
	SpeedSoftL=tc_read_cv(TC0, 0) & 0xFFFF;
	tc_start(TC0, 0);	
	return SpeedSoftL;
}
	
int16_t CaptureSpeedSoftR(void)
{
	static int16_t SpeedSoftR=0;
	SpeedSoftR=tc_read_cv(TC1, 0) & 0xFFFF;
	tc_start(TC1, 0);	
	return SpeedSoftR;
}


