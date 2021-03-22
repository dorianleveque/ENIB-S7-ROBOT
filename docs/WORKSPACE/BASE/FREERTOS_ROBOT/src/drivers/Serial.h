/*
 * Serial.h
 *
 * Created: 08/01/2014 11:52:39
 *  Author: Kerhoas
 *	kerhoas@enib.fr
 */ 

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"


#define STRING_EOL    "\r"
#define STRING_HEADER	"-- ROBOT ENIB --\r\n" \
						"-- "BOARD_NAME" --\r\n" \
						"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/* Pdc transfer buffer */
uint8_t g_uc_pdc_buffer[BUFFER_SIZE];
/* PDC data packet for transfer */
pdc_packet_t g_pdc_uart_packet;
/* Pointer to UART PDC register base */
Pdc *g_p_uart_pdc;


void UartInit(void);