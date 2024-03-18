#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"

// Todo: Add more process
// 104 from task0 (PC 0x82158 SP 0x3FFFC0) to task1 (PC 0x101B30 SP 0x401000)
// 205 from task1 (PC 0x82158 SP 0x400FB0) to task2 (PC 0x101B60 SP 0x402000)void process(char *array)
void process(char *array)
{	
	// while (1) {
		for (int i = 0; i < 5; i++){
			uart_send(array[i]);
			delay(5000000);
		}
	// }
	delay(1);
	return;
}

void process2(char *array)
{
	while (1) {
		for (int i = 0; i < 5; i++){
			uart_send(array[i]);
			delay(5000000);
		}
		printf("\nBack to process2\r\n");
	}
}

// void process3(char *array)
// {
// 	while (1) {
// 		for (int i = 0; i < 5; i++){
// 			uart_send(array[i]);
// 			delay(5000000);
// 		}
// 		printf("\nBack to process\r\n");
// 	}
// }

// void process4(char *array)
// {
// 	while (1) {
// 		for (int i = 0; i < 5; i++){
// 			uart_send(array[i]);
// 			delay(5000000);
// 		}
// 		printf("\nBack to process\r\n");
// 	}
// }

void kernel_main(void)
{
	uart_init();
	init_printf(0, putc);

	printf("kernel boots\n");
	// Init the records array;
	init_records();
	printf("Records array init");
	irq_vector_init();
	generic_timer_init();
	enable_interrupt_controller();
	enable_irq();

	int res = copy_process((unsigned long)&process, (unsigned long)"12345");
	if (res != 0) {
		printf("error while starting process 1");
		return;
	}
	res = copy_process((unsigned long)&process2, (unsigned long)"abcde");
	if (res != 0) {
		printf("error while starting process 2");
		return;
	}

	// res = copy_process((unsigned long)&process3, (unsigned long)"98765");
	// if (res != 0) {
	// 	printf("error while starting process 3");
	// 	return;
	// }

	// res = copy_process((unsigned long)&process4, (unsigned long)"qwert");
	// if (res != 0) {
	// 	printf("error while starting process 4");
	// 	return;
	// }
	// Tode: change the while loop into IDLE
	while (1){
		// IDLE, wait for the first irq
		if (record_num > 50) {
			printf("The kernel task is going to idle\n\r");
			disable_irq();
			asm volatile("wfi"); // Be idle
		}
		delay(50000);
	}	
}
