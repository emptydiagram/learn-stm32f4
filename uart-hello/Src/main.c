#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"

#define GPIOAEN            (1U << 0)
#define USART2EN           (1U << 17)
//#define USART_CR1_TE       (1U << 3)
//#define USART_CR1_UE       (1U << 13)
//#define USART_SR_TXE       (1U << 7)

// 16 MHz internal RC oscillator ??
#define DEFAULT_SYS_FREQ   (16000000)
#define APB1_CLK           DEFAULT_SYS_FREQ

#define USART_BAUDRATE     115200

void usart2_tx_init();
static void usart_set_baudrate(USART_TypeDef *usartx, uint32_t periph_clk, uint32_t baud_rate);
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baud_rate);
void usart2_write(int ch);

// somehow this retargets printf
int __io_putchar(int ch) {
	usart2_write(ch);
	return ch;
}

int main(void)
{

	usart2_tx_init();

	char* strs[] = {"HELLO ", "W", "O", "R", "L", "D", " "};
	int strs_len = sizeof(strs) / sizeof(strs[0]);
	int str_idx = 0;
	while(1) {
		printf(strs[str_idx]);
		fflush(stdout);
		int count_multiple = ( str_idx == 0) ? 2 : 5;
		for (int i = 0; i < (count_multiple * 100000); i++);
		str_idx = (str_idx + 1) % strs_len;
	}
}

void usart2_tx_init() {
	/*
	 * # Config UART GPIO pin
	 *  - enable clock access to GPIO A
	 *  - set PA2 mode to alternate function mode
	 *  - set PA2 alt. fn. type to UART Tx, AF7
	 */
	RCC->AHB1ENR |= GPIOAEN;

	// set MODER2 on GPIOA (PA2) to (b4 = 0, b5 = 1)
	GPIOA->MODER &= ~(1U << 4);
	GPIOA->MODER |= (1U << 5);

	// set bits 8-11 to 0b0111
	GPIOA->AFR[0] |= (7U << 8);
	GPIOA->AFR[0] &= ~(1U << 11);


	/*
	 * # Configure UART Module
	 *  - enable USART2 on APB1
	 *  - config baud rate
	 *  - config transfer direction
	 *  - enable UART module
	 */

	RCC->APB1ENR |= USART2EN;

	usart_set_baudrate(USART2, APB1_CLK, USART_BAUDRATE);

	// not logical OR, "want to clean" the CR1
	USART2->CR1 = USART_CR1_TE;

	USART2->CR1 |= USART_CR1_UE;
}

static void usart_set_baudrate(USART_TypeDef *usartx, uint32_t periph_clk, uint32_t baud_rate) {
	usartx->BRR = compute_uart_bd(periph_clk, baud_rate);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baud_rate) {
	// "empirically derived" formula from Video 15 ~16:40, of "Embedded Systems Bare-Metal Programming Ground Up™ (STM32)"
	return (periph_clk + (baud_rate / 2U)) / baud_rate;
}

void usart2_write(int ch) {
    // ensure transmit data register is empty
	// spins until TXE bit is high.
	// "This bit is set by hardware when the content of the TDR register has been transferred into the shift register."
	while (!(USART2->SR & USART_SR_TXE));

	// write to Transmit DR
	USART2->DR = ch & 0xFF;
}
