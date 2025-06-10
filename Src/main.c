#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx.h"

// User LED: PA5, (port, pin) = (A, 5)

//// Masks
// mask for bit 0 of RCC_AHB1ENR register
#define GPIOAEN            (1UL << 0)

// the User LED I/O pin is GPIO PA5
#define PIN5               (1UL << 5)
#define USER_LED_PIN       PIN5

#define COUNT_SIZE         (80000)
#define FIB_BLINK_COUNT    (230000)
#define FIB_PAUSE_COUNT    (5*FIB_BLINK_COUNT)

#define    __IO            volatile


void toggle_odr_spin(int spin_count) {
    GPIOA->ODR ^= USER_LED_PIN;
    for (int i = 0; i < spin_count; i++);
}

void blink_odr_spin(int spin_count_1, int spin_count_2) {
	toggle_odr_spin(spin_count_1);
	toggle_odr_spin(spin_count_2);
}

void blink_bsrr_spin(int spin_count_1, int spin_count_2) {
	GPIOA->BSRR = USER_LED_PIN;
	for (int i = 0; i < spin_count_1; i++);
	GPIOA->BSRR = (1UL << (16 + 5));
	for (int i = 0; i < spin_count_2; i++);
}

int main(void) {
    // 1. enable clock access to GPIO A
    RCC->AHB1ENR |= GPIOAEN;

    // 2. set PA5 as output pin
    GPIOA->MODER |= (1UL<<10);
    GPIOA->MODER &= ~(1UL<<11);

    // 3. inside loop, toggle LED pin

    GPIOA->ODR &= ~(USER_LED_PIN);
    while (1) {
        // for (int i = 0; i < 4; i++)
        //     toggle_odr_spin(9*COUNT_SIZE);
        // for (int i = 0; i < 12; i++)
        //     toggle_odr_spin(3*COUNT_SIZE);
        // for (int i = 0; i < 36; i++)
        //     toggle_odr_spin(1*COUNT_SIZE);
        // for (int i = 0; i < 12; i++)
        //     toggle_odr_spin(3*COUNT_SIZE);
        // for (int i = 0; i < 4; i++)
        //     toggle_odr_spin(9*COUNT_SIZE);

        int fib_seq[] = {0, 1, 1, 2, 3, 5, 8, 13};
        size_t len = sizeof(fib_seq) / sizeof(fib_seq[0]);

        for (size_t i = 0; i < len; i++) {
            for (int j = 0; j < FIB_PAUSE_COUNT; j++);
            // for (int j = 0; j < 2 * fib_seq[i]; j++)
            //     toggle_odr_spin(FIB_BLINK_COUNT);
            for (int j = 0; j < fib_seq[i]; j++)
                blink_bsrr_spin(FIB_BLINK_COUNT, FIB_BLINK_COUNT);
        }
    }
}
