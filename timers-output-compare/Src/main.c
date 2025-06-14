#include <stdio.h>
#include "tim.h"
#include "uart.h"


int main(void)
{
    // user LED (PA5) setup
    // output compare: toggling a pin (LED) using a timer
    tim2_pa5_output_compare(5);

    while (1) {

    }
}
