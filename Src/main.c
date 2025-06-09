//#include <stdint.h>
//
//#if !defined(__SOFT_FP__) && defined(__ARM_FP)
//  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
//#endif
//
//int main(void)
//{
//    /* Loop forever */
//	for(;;);
//}


// where is LED connected?
// TODO: look at nucleo board user guide


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
#define REG_GPIOA_MODER	(* (volatile unsigned int *) (GPIO_BASE + GPIOA_MODER_OFF))

#define GPIOA_ODR_OFF 	(0x14UL)
#define REG_GPIOA_ODR	(* (volatile unsigned int *) (GPIO_BASE + GPIOA_ODR_OFF))

#define RCC_AHB1ENR_OFF (0x30UL)
#define REG_RCC_AHB1ENR (* (volatile unsigned int *) (RCC_BASE + RCC_AHB1ENR_OFF))


//// Masks
// mask for bit 0 of RCC_AHB1ENR register
#define GPIOAEN 		(1UL << 0)

// the User LED I/O pin is GPIO PA5
#define PIN5 			(1UL << 5)
#define USER_LED_PIN	PIN5

int main(void) {
    return 0;
}
