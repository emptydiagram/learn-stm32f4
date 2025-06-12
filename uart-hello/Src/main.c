#include <stdio.h>
#include "uart.h"

int main(void)
{

	usart2_tx_init();

	char* strs[] = {"HELLO ", "W", "O", "R", "L", "D", ".", ".", "."};
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
