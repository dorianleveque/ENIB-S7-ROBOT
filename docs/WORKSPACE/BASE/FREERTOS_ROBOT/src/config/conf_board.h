#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

/** PWM frequency in Hz */
#define PWM_FREQUENCY_MOTOR		48000
#define PWM_FREQUENCY_SERVO		50
/** Period value of PWM output waveform */
#define PWM_PERIOD_VALUE        1000

#define LEFT	0
#define RIGHT	1

#define SPEED_HARDWARE_MEASURE 0

/** Enable Com Port. */
#define CONF_BOARD_UART_CONSOLE
/** Usart Hw ID used by the console (UART0). */
#define CONSOLE_UART_ID          ID_UART0
/** UART Interface */
#define CONF_UART            CONSOLE_UART
/** Baudrate setting */
#define CONF_UART_BAUDRATE   9600
/** Parity setting */
#define CONF_UART_PARITY     UART_MR_PAR_NO
/** FIFO Size */
#define BUFFER_SIZE  2

#endif /* CONF_BOARD_H_INCLUDED */