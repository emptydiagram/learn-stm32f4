#include <stdint.h>

// User LED: PA5, (port, pin) = (A, 5)

#define PERIPH_BASE        (0x40000000UL)

// AHB1 → GPIO A: 0x40020000 - 0x400203FF. width is 0x400 = 2^10
#define AHB1_OFF           (0x20000UL)
#define AHB1_BASE          (PERIPH_BASE + AHB1_OFF)

// GPIOA = AHB1 start
#define GPIO_A_AHB1_OFF    (0x0UL)
#define GPIOA_BASE         (AHB1_BASE + GPIO_A_AHB1_OFF)

// RCC: 0x40023800 - 0x40023BFF
// 0x3C00 - 0x3800 = 0x400
#define RCC_AHB1_OFF       (0x3800UL)
#define RCC_BASE           (AHB1_BASE + RCC_AHB1_OFF)

//// Masks
// mask for bit 0 of RCC_AHB1ENR register
#define GPIOAEN            (1UL << 0)

// the User LED I/O pin is GPIO PA5
#define PIN5               (1UL << 5)
#define USER_LED_PIN       PIN5

#define COUNT_SIZE         (80000)

#define	__IO	           volatile

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

void toggle_odr_spin(int spin_count) {
	GPIOA->ODR ^= USER_LED_PIN;
	for (int i = 0; i < spin_count; i++);
}

int main(void) {
    // 1. enable clock access to GPIO A
	RCC->AHB1ENR |= GPIOAEN;

	// 2. set PA5 as output pin
	GPIOA->MODER |= (1UL<<10);
	GPIOA->MODER &= ~(1UL<<11);

	// 3. inside loop, toggle LED pin

	while (1) {
		for (int i = 0; i < 4; i++)
			toggle_odr_spin(9*COUNT_SIZE);

		for (int i = 0; i < 12; i++)
			toggle_odr_spin(3*COUNT_SIZE);

		for (int i = 0; i < 36; i++)
			toggle_odr_spin(1*COUNT_SIZE);

		for (int i = 0; i < 12; i++)
			toggle_odr_spin(3*COUNT_SIZE);

		for (int i = 0; i < 4; i++)
			toggle_odr_spin(9*COUNT_SIZE);
	}
}
