#include "utils.h"
#include "printf.h"
#include "timer.h"
#include "entry.h"
#include "peripherals/irq.h"
#include "sched.h"

void print_record(struct switch_record * record) {
    printf("%d from task%d (PC 0x%X SP 0x%X) to task%d (PC 0x%X SP 0x%X)\r\n",
           record->timestamp,
           record->task_in_id,
           record->task_in_pc,
           record->task_in_sp,
           record->task_out_id,
           record->task_out_pc,
           record->task_out_sp);
}

const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",
    "IRQ_INVALID_EL1t",		
    "FIQ_INVALID_EL1t",		
    "ERROR_INVALID_EL1T",		

    "SYNC_INVALID_EL1h",		
    "IRQ_INVALID_EL1h",		
    "FIQ_INVALID_EL1h",		
    "ERROR_INVALID_EL1h",		

    "SYNC_INVALID_EL0_64",		
    "IRQ_INVALID_EL0_64",		
    "FIQ_INVALID_EL0_64",		
    "ERROR_INVALID_EL0_64",	

    "SYNC_INVALID_EL0_32",		
    "IRQ_INVALID_EL0_32",		
    "FIQ_INVALID_EL0_32",		
    "ERROR_INVALID_EL0_32"	
};

void enable_interrupt_controller()
{
    // Enables Core 0 Timers interrupt control for the generic timer 
    put32(TIMER_INT_CTRL_0, TIMER_INT_CTRL_0_VALUE);
}

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address)
{
    printf("%s, ESR: %x, address: %x\r\n", entry_error_messages[type], esr, address);
}

void handle_irq(void)
{
	// If number of record is lager than 50, make the kernel in idle instead of reset the timer.
	if (record_num > 50) {
        for (int i = 0; i < MAX_CONTEXT_SWITCHES; i++) {
            print_record(records[i]);
        }
	}
    // Each Core has its own pending local intrrupts register
    unsigned int irq = get32(INT_SOURCE_0);
    switch (irq) {
        case (GENERIC_TIMER_INTERRUPT):
            handle_generic_timer_irq();
            break;
        default:
            printf("Unknown pending irq: %x\r\n", irq);
    }
}

// Todo: add save in and out functions
void save_record_in(void) {
    printf("\r\n Irq in \r\n");
    cur_record->timestamp = get_time_ms();
    printf("timestamp = %d\r\n", cur_record->timestamp);
    cur_record->task_in_id = get_pid();
    printf("task_in_pid = %d\r\n", cur_record->task_in_id);
    cur_record->task_in_pc = get_pc();
    printf("task_in_pc = %X\r\n", cur_record->task_in_pc);
    cur_record->task_in_sp = get_sp() - S_FRAME_SIZE;
    printf("task_in_sp = %X\r\n", cur_record->task_in_sp);
}

void save_record_out(void) {
    printf("\r\n Irq out \r\n");
    cur_record->task_out_id = get_pid();
    printf("task_out_pid = %d\r\n", cur_record->task_out_id);
    cur_record->task_out_pc = get_pc_out();
    printf("task_out_pc = %X\r\n", cur_record->task_out_pc);
    cur_record->task_out_sp = get_sp() + S_FRAME_SIZE;
    printf("task_out_sp = %X\r\n", cur_record->task_out_sp);
    record_num++;
    printf("Cur record num = %d\r\n", record_num);
    cur_record = records[record_num];
}

void save_record_out_first(void) {
    if (record_num < 4) {
        // printf("\r\n Irq out \r\n");
        cur_record->task_out_id = get_pid();
        // printf("task_out_pid = %d\r\n", cur_record->task_out_id);
        cur_record->task_out_pc = current->cpu_context.x19;
        // printf("task_out_pc = %X\r\n", cur_record->task_out_pc);
        cur_record->task_out_sp = current->cpu_context.sp;
        // printf("task_out_sp = %X\r\n", cur_record->task_out_sp);
        record_num++;
        // printf("Cur record num = %d\r\n", record_num);
        cur_record = records[record_num];
    }
}