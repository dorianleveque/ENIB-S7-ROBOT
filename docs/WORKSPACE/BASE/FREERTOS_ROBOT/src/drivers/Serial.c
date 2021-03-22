#include "Serial.h"



extern char* p_buf;

void UART0_Handler(void)
{	int i=0;
	//char* p_buf;
	/* Get UART status and check if PDC receive buffer is full */
	if ((uart_get_status(CONSOLE_UART) & UART_SR_RXBUFF) == UART_SR_RXBUFF) {
		/* Configure PDC for data transfer (RX and TX) */
		pdc_rx_init(g_p_uart_pdc, &g_pdc_uart_packet, NULL);		
		p_buf=g_pdc_uart_packet.ul_addr;
			
		pdc_tx_init(g_p_uart_pdc, &g_pdc_uart_packet, NULL);
	}
}


void UartInit(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	
	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	
	/* Get pointer to UART PDC register base */
	g_p_uart_pdc = uart_get_pdc_base(CONSOLE_UART);

	/* Initialize PDC data packet for transfer */
	g_pdc_uart_packet.ul_addr = (uint32_t) g_uc_pdc_buffer;
	g_pdc_uart_packet.ul_size = BUFFER_SIZE;

	/* Configure PDC for data receive */
	pdc_rx_init(g_p_uart_pdc, &g_pdc_uart_packet, NULL);

	/* Enable PDC transfers */
	pdc_enable_transfer(g_p_uart_pdc, PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN);

	/* Enable UART IRQ */
	uart_enable_interrupt(CONSOLE_UART, UART_IER_RXBUFF);

	/* Enable UART interrupt */
	NVIC_EnableIRQ(UART0_IRQn);	
}


