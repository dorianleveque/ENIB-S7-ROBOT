/*
 * ServoCommand.c
 * Created: 20/06/2013 10:50:07
 * Authors: kerhoas, ziad
 * kerhoas@enib.fr
 */ 
#include "ServoCommand.h"
#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"

#define SERVO_HIGH	0
#define SERVO_LOW	1
#define NUM_SERVOS	2

#define SERVO_H_CHANNEL		2
#define SERVO_L_CHANNEL		3

static struct {
	pwm_channel_t channel;
	int duty;
} _servoStatus[NUM_SERVOS];

int ServoCommandInit() 
{
	gpio_configure_pin(PIN_PWM_SERVOH_GPIO, PIN_PWM_SERVOH_FLAGS);
	gpio_configure_pin(PIN_PWM_SERVOL_GPIO, PIN_PWM_SERVOL_FLAGS);
	
	_servoStatus[SERVO_HIGH].duty = SERVO_H_DUTY_INIT;
	_servoStatus[SERVO_LOW ].duty = SERVO_L_DUTY_INIT;
	/*--------------------------------------*/
	/*Initialize PWM channel for the pin20*/
	/* Period is left-aligned */
	_servoStatus[SERVO_HIGH].channel.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	_servoStatus[SERVO_HIGH].channel.polarity = PWM_LOW;
	/* Use PWM clock A as source clock */
	_servoStatus[SERVO_HIGH].channel.ul_prescaler = PWM_CMR_CPRE_CLKB;
	/* Period value of output waveform */
	_servoStatus[SERVO_HIGH].channel.ul_period = PWM_PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	_servoStatus[SERVO_HIGH].channel.ul_duty = SERVO_H_DUTY_INIT;
	_servoStatus[SERVO_HIGH].channel.channel = PIN_PWM_SERVOH_CHANNEL;
	pwm_channel_init(PWM, &_servoStatus[SERVO_HIGH].channel);
	/* Enable channel counter event interrupt */
	//pwm_channel_enable_interrupt(PWM, PIN_PWM_SERVOH_CHANNEL, 0);
	/*--------------------------------------*/
	
	/*--------------------------------------*/
	/*Initialize PWM channel for the pin21*/
	/* Period is left-aligned */
	_servoStatus[SERVO_LOW].channel.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	_servoStatus[SERVO_LOW].channel.polarity = PWM_LOW;
	/* Use PWM clock A as source clock */
	_servoStatus[SERVO_LOW].channel.ul_prescaler = PWM_CMR_CPRE_CLKB;
	/* Period value of output waveform */
	_servoStatus[SERVO_LOW].channel.ul_period = PWM_PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	_servoStatus[SERVO_LOW].channel.ul_duty =  SERVO_L_DUTY_INIT;
	_servoStatus[SERVO_LOW].channel.channel = PIN_PWM_SERVOL_CHANNEL;
	pwm_channel_init(PWM, &_servoStatus[SERVO_LOW].channel);

	/*--------------------------------------*/
	pwm_channel_enable(PWM, PIN_PWM_SERVOH_CHANNEL);
	pwm_channel_enable(PWM, PIN_PWM_SERVOL_CHANNEL);
	
	return 1;
}


int SetPosH(int duty) 
{
    if (duty<H_MIN) {duty = H_MIN;}
	else if (duty>H_MAX) {duty = H_MAX;}	
	pwm_channel_update_duty(PWM, &_servoStatus[SERVO_HIGH].channel, duty);
	_servoStatus[SERVO_HIGH].duty = duty;
	return duty;
}

int SetPosL(int duty) 
{
	if (duty<L_MIN) {duty = L_MIN;}
	else if (duty>L_MAX) {duty = L_MAX;}
	pwm_channel_update_duty(PWM, &_servoStatus[SERVO_LOW].channel, duty);
	_servoStatus[SERVO_LOW].duty = duty;
	return duty;
}


int GetPosH() 
{
	return _servoStatus[SERVO_HIGH].duty;
}

int GetPosL() 
{
	return _servoStatus[SERVO_LOW].duty;
}