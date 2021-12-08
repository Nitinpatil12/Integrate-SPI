#define F_CPU  (4000000UL)
#include <avr/io.h>
#include "UART_1_AVR128DA64.h"
#include <util/delay.h>
#include <string.h>
static void SPI0_init(void);

void clientSelect(void);
void clientDeselect(void);

static void SPI0_init(void)
{
	PORTA.DIR |= PIN4_bm; /* Set MOSI pin direction to output */
	PORTA.DIR &= ~PIN5_bm; /* Set MISO pin direction to input */
	PORTA.DIR |= PIN6_bm; /* Set SCK pin direction to output */
	PORTA.DIR |= PIN7_bm; /* Set SS pin direction to output */
	
	SPI0.CTRLA = SPI_CLK2X_bm           /* Enable double-speed */
	| SPI_ENABLE_bm          /* Enable module */
	| SPI_MASTER_bm          /* SPI module in Master mode */
	| SPI_PRESC_DIV16_gc;    /* System Clock divided by 16 */
}

void SPI_0_send_char(char data)
{
	clientSelect();
	SPI0.DATA= data;

	while (!(SPI0.INTFLAGS & (1 << 7)))           // flag is set when a serial transfer is complete.
	{
		;
	}
	clientDeselect();
}

void SPI_0_send_string(char *str)
{
	while(*str != '\0')
	{
		SPI_0_send_char(*str);
		str++;
	}
}
void clientSelect(void)
{
	PORTA.OUT &= ~PIN7_bm;    // Set SS pin value to LOW
}
void clientDeselect(void)
{
	PORTA.OUT |= PIN7_bm;    // Set SS pin value to HIgh
}

int main(void)
{
	USART1_init(9600);
	char data[10] ={"agva\n"};
	//char data[10] = "Hello";
	SPI0_init();
	while (1)
	{
		_delay_ms(10);
		SPI_0_send_string(data);
	}
}