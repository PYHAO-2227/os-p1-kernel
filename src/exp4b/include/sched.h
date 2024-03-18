#ifndef _SCHED_H
#define _SCHED_H

#define THREAD_CPU_CONTEXT			0 		// offset of cpu_context in task_struct 

#ifndef __ASSEMBLER__

#define THREAD_SIZE				4096

#define NR_TASKS				64 

#define FIRST_TASK task[0]
#define LAST_TASK task[NR_TASKS-1]
// Todo define record array
#define TASK_RUNNING				0

// Define the size of the record array
#define MAX_CONTEXT_SWITCHES 52
extern struct switch_record *cur_record;
extern struct switch_record * records[MAX_CONTEXT_SWITCHES];
extern int record_num;

extern struct task_struct *current;
extern struct task_struct * task[NR_TASKS];
extern int nr_tasks;

struct cpu_context {
	unsigned long x19;
	unsigned long x20;
	unsigned long x21;
	unsigned long x22;
	unsigned long x23;
	unsigned long x24;
	unsigned long x25;
	unsigned long x26;
	unsigned long x27;
	unsigned long x28;
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
};

// Todo: add Waiting state
struct task_struct {
	struct cpu_context cpu_context;
	long state;	
	long counter; /* countdown for scheduling. higher value means having run for less. recharged in schedule(). decremented in timer_tick(). always non negative */
	long priority;
	long preempt_count;
	int pid;
};

// Todo record_struct
struct switch_record {
	unsigned long timestamp; 
    int task_in_id;
    int task_out_id;
    unsigned long task_in_pc;
    unsigned long task_out_pc;
    unsigned long task_in_sp;
    unsigned long task_out_sp;
};

extern void sched_init(void);
extern void schedule(void);
extern void timer_tick(void);
extern void preempt_disable(void);
extern void preempt_enable(void);
extern void switch_to(struct task_struct* next);
extern void cpu_switch_to(struct task_struct* prev, struct task_struct* next);
extern int get_pid(void);
extern unsigned long get_time_ms(void);
extern void init_records(void);
unsigned long get_cntpct(void);

#define INIT_TASK \
/*cpu_context*/	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state etc */	0,0,1,0,0 \
}

#define INIT_RECORD { \
    .timestamp = 0, \
    .task_in_id = 0, \
    .task_out_id = 0, \
    .task_in_pc = 0, \
    .task_out_pc = 0, \
    .task_in_sp = 0, \
    .task_out_sp = 0 \
}

#endif
#endif
