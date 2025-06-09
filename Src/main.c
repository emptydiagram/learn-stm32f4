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
#define AHB1_START_OFF 	(0x20000UL)
#define AHB1_BASE 		(PERIPH_BASE + AHB1_START_OFF)

// GPIOA = AHB1 start
#define GPIO_A_AHB1_OFF	 (0x0UL)
#define GPIO_A_BASE		 (AHB1_BASE + GPIO_A_AHB1_OFF)

// RCC: 0x4002 3800 - 0x4002 3BFF
// 0x3C00 - 0x3800 = 0x400
#define RCC_AHB1_OFF 	(0x3800UL)
#define RCC_AHB1_BASE 	(AHB1_BASE + RCC_AHB1_OFF)

#define GPIO_A_WIDTH	(0x400UL)
#define RCC_WIDTH 		(0x400UL)
