#ifndef QUADENCODER_H_
#define QUADENCODER_H_

#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"

#define QENC_SPEED_MIN (-420)
#define QENC_SPEED_MAX (+440)

int QuadEncoderInit(void);

uint16_t CapturePosition16L(void);
uint16_t CapturePosition16R(void);

uint32_t CapturePosition32L(void);
uint32_t CapturePosition32R(void);

int16_t CaptureSpeedSoftL(void);
int16_t CaptureSpeedSoftR(void);

#endif /* QUADENCODER_H_ */