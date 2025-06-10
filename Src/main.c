#include <stdint.h>

// User LED: PA5, (port, pin) = (A, 5)

#define PERIPH_BASE 	(0x40000000UL)

// AHB1 > GPIO A: 0x4002 0000 - 0x4002 03FF. width is 0x400 = 2^10
#define AHB1_OFF 		(0x20000UL)
#define AHB1_BASE 		(PERIPH_BASE + AHB1_OFF)

// GPIOA = AHB1 start
#define GPIO_A_AHB1_OFF	 (0x0UL)
#define GPIOA_BASE		 (AHB1_BASE + GPIO_A_AHB1_OFF)

// RCC: 0x4002 3800 - 0x4002 3BFF
// 0x3C00 - 0x3800 = 0x400
#define RCC_AHB1_OFF 	(0x3800UL)
#define RCC_BASE 		(AHB1_BASE + RCC_AHB1_OFF)

#define GPIOA_MODER_OFF (0x0UL)
#define REG_GPIOA_MODER	(* (volatile unsigned int *) (GPIOA_BASE + GPIOA_MODER_OFF))

#define GPIOA_ODR_OFF 	(0x14UL)
#define REG_GPIOA_ODR	(* (volatile unsigned int *) (GPIOA_BASE + GPIOA_ODR_OFF))

#define RCC_AHB1ENR_OFF (0x30UL)
#define REG_RCC_AHB1ENR (* (volatile unsigned int *) (RCC_BASE + RCC_AHB1ENR_OFF))

//// Masks
// mask for bit 0 of RCC_AHB1ENR register
#define GPIOAEN 		(1UL << 0)

// the User LED I/O pin is GPIO PA5
#define PIN5 			(1UL << 5)
#define USER_LED_PIN	PIN5

#define COUNT_SIZE		(100000)

#define __IO	volatile

typedef struct {
	__IO uint32_t MODER;
	__IO uint32_t OTYPER;
	__IO uint32_t OSPEEDR;
	__IO uint32_t PUPDR;
	__IO uint32_t IDR;
	__IO uint32_t ODR;
	__IO uint32_t BSRR;
	__IO uint32_t LCKR;
	__IO uint32_t AFR[2];
} GPIO_TypeDef;

typedef struct {
	uint32_t DUMMY[12];
	__IO uint32_t AHB1ENR;
} RCC_TypeDef;

#define RCC ((RCC_TypeDef*) RCC_BASE)
#define GPIOA ((GPIO_TypeDef*) GPIOA_BASE)

void blink_odr(int count_size_1, int count_size_2) {
	REG_GPIOA_ODR ^= USER_LED_PIN;
	for (int i = 0; i < count_size_1; i++);
	REG_GPIOA_ODR ^= USER_LED_PIN;
	for (int i = 0; i < count_size_2; i++);
}

int main(void) {
    // 1. enable clock access to GPIO A
	//REG_RCC_AHB1ENR |= GPIOAEN;
	RCC->AHB1ENR |= GPIOAEN;

	// 2. set PA5 as output pin
	//REG_GPIOA_MODER |= (1UL<<10);
	//REG_GPIOA_MODER &= ~(1UL<<11);
	GPIOA->MODER |= (1UL<<10);
	GPIOA->MODER &= ~(1UL<<11);

	// 3. inside loop, toggle LED pin

	while (1) {
		for (int i = 0; i < 2; i++)
			blink_odr(9*COUNT_SIZE, 9*COUNT_SIZE);

		for (int i = 0; i < 6; i++)
			blink_odr(3*COUNT_SIZE, 3*COUNT_SIZE);

		for (int i = 0; i < 18; i++)
			blink_odr(1*COUNT_SIZE, 1*COUNT_SIZE);

		for (int i = 0; i < 6; i++)
			blink_odr(3*COUNT_SIZE, 3*COUNT_SIZE);

		for (int i = 0; i < 2; i++)
			blink_odr(9*COUNT_SIZE, 9*COUNT_SIZE);
	}
}
