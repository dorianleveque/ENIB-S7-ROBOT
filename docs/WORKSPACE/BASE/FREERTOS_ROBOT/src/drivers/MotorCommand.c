/*
 * MotorCommand.c
 *
 * Created: 20/06/2013 10:47:27
 */ 

#include "MotorCommand.h"

#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"

static struct {
	pwm_channel_t channel;
	int duty;
} _motorStatus[2];

int MotorCommandInit() {
	_motorStatus[LEFT].duty = 500;
	_motorStatus[RIGHT].duty = 500;
	
	gpio_configure_pin(PIN_PWM_MOTORL_GPIO, PIN_PWM_MOTORL_FLAGS);
	gpio_configure_pin(PIN_PWM_MOTORR_GPIO, PIN_PWM_MOTORR_FLAGS);
	
	/*--------------------------------------*/
	/*Initialize PWM channel for the pin18*/
	/* Period is left-aligned */
	_motorStatus[LEFT].channel.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	_motorStatus[LEFT].channel.polarity = PWM_HIGH;
	/* Use PWM clock A as source clock */
	_motorStatus[LEFT].channel.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	_motorStatus[LEFT].channel.ul_period = PWM_PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	_motorStatus[LEFT].channel.ul_duty = _motorStatus[0].duty;
	_motorStatus[LEFT].channel.channel = PIN_PWM_MOTORL_CHANNEL;
	pwm_channel_init(PWM, &_motorStatus[0].channel);
	/* Enable channel counter event interrupt */
	//pwm_channel_enable_interrupt(PWM, PIN_PWM_MOTORL_CHANNEL, 0);
	/*--------------------------------------*/
	
	/*--------------------------------------*/
	/*Initialize PWM channel for the pin19*/
	/* Period is left-aligned */
	_motorStatus[RIGHT].channel.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	_motorStatus[RIGHT].channel.polarity = PWM_HIGH;
	/* Use PWM clock A as source clock */
	_motorStatus[RIGHT].channel.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	_motorStatus[RIGHT].channel.ul_period = PWM_PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	_motorStatus[RIGHT].channel.ul_duty = _motorStatus[1].duty;
	_motorStatus[RIGHT].channel.channel = PIN_PWM_MOTORR_CHANNEL;
	pwm_channel_init(PWM, &_motorStatus[RIGHT].channel);
	/* Enable channel counter event interrupt */
	//pwm_channel_enable_interrupt(PWM, PIN_PWM_MOTORR_CHANNEL, 0);
	/*--------------------------------------*/
	
	pwm_channel_enable(PWM, PIN_PWM_MOTORL_CHANNEL);
	pwm_channel_enable(PWM, PIN_PWM_MOTORR_CHANNEL);
	
	// EN_MOT = 0
	ioport_set_pin_level(PIO_PA26_IDX, false);

	return 1;	
}

bool SetDutyCycleLeft(int duty) {
	uint32_t rtv = pwm_channel_update_duty(PWM, &_motorStatus[LEFT].channel, duty);
	
	if (rtv == 0) {
		_motorStatus[LEFT].duty = duty;
		return true;
	}
	
	return false;
}

bool SetDutyCycleRight(int duty) {
	uint32_t rtv = pwm_channel_update_duty(PWM, &_motorStatus[RIGHT].channel, duty);
	
	if (rtv == 0) {
		_motorStatus[RIGHT].duty = duty;
		return true;
	}
	
	return false;
}

int GetDutyCycleLeft() {
	return _motorStatus[LEFT].duty;
}

int GetDutyCycleRight() {
	return _motorStatus[RIGHT].duty;
}