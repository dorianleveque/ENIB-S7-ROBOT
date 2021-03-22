/*
 * ServoCommand.h
 *
 * Created: 20/06/2013 10:50:24
 *  Author: ziad
 */

#ifndef SERVOCOMMAND_H_
#define SERVOCOMMAND_H_

#define SERVO_H_DUTY_INIT	100 //75
#define SERVO_L_DUTY_INIT	70
#define L_MIN	25
#define L_MAX	112
#define H_MIN	32    //32
#define H_MAX	100    // 79

int ServoCommandInit(void);
int SetPosH(int);
int SetPosL(int);
int GetPosH(void);
int GetPosL(void);

#endif /* SERVOCOMMAND_H_ */