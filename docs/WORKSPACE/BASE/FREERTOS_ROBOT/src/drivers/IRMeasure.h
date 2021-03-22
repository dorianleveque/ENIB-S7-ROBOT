/*
 * IRMeasure.h
 *
 * Created: 20/06/2013 10:51:22
 *  Author: ziad
 */ 


#ifndef IRMEASURE_H_
#define IRMEASURE_H_
#include "asf.h"

int IRMeasureInit(void);
uint16_t IRMeasureInitPoll(void);
int GetDistances(void);

uint16_t GetDistanceL(void);
uint16_t GetDistanceR(void);
int GetDistancePoll(uint16_t*);

#endif /* IRMEASURE_H_ */