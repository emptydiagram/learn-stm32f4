#include "stm32f4xx.h"

#define CYCLES_PER_MS      16000

#define SYST_CSR_ENABLE    (1U << 0)
#define SYST_CSR_CLKSOURCE (1U << 2)
#define SYST_CSR_COUNTFLAG (1U << 16)


void systick_delay_ms(int delay) {
    // core peripheral like systick info is available Arm Cortex M4 docs

	//
	SysTick->LOAD = CYCLES_PER_MS;

	SysTick->VAL = 0;

	SysTick->CTRL = SYST_CSR_ENABLE | SYST_CSR_CLKSOURCE;

	for (int i = 0; i < delay; i++) {
		while ((SysTick->CTRL & SYST_CSR_COUNTFLAG) == 0);
	}
	SysTick->CTRL = 0;
}
