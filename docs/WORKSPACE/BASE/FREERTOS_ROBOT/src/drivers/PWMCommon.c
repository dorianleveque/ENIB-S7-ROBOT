/*
 * PWMCommon.c
 *
 * Created: 20/06/2013 16:28:39
 *  Author: kerhoas
 */ 

#include "PWMCommon.h"

#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"

int PWMClockInit() {
	/* Enable PWM peripheral clock */
 	pmc_enable_periph_clk(ID_PWM);
	/* Set PWM clock as PWM_FREQUENCY * PERIOD_VALUE */
 	pwm_clock_t clock_setting = {
 		.ul_clka = PWM_FREQUENCY_MOTOR * PWM_PERIOD_VALUE,
 		.ul_clkb = PWM_FREQUENCY_SERVO * PWM_PERIOD_VALUE ,
 		.ul_mck = sysclk_get_cpu_hz()
 	};
 	pwm_init(PWM, &clock_setting);
	
	return 1;
}
