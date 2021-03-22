/*
 * MotorCommand.h
 *
 * Created: 20/06/2013 10:47:42
 *  Author: kerhoas
 */ 


#ifndef MOTORCOMMAND_H_
#define MOTORCOMMAND_H_

#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"


int MotorCommandInit(void);
int GetDutyCycleLeft(void); 


int GetDutyCycleRight(void);
bool SetDutyCycleLeft(int);
bool SetDutyCycleRight(int);

#endif /* MOTORCOMMAND_H_ */