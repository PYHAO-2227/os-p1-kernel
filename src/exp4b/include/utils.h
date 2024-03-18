#ifndef	_UTILS_H
#define	_UTILS_H

extern void delay ( unsigned long);
extern void put32 ( unsigned long, unsigned int );
extern unsigned int get32 ( unsigned long );
extern int get_el ( void );
extern unsigned long get_sp(void);
extern unsigned long get_pc(void);
extern unsigned long get_pc_out(void);
extern unsigned long get_pc_first(void);
extern unsigned long get_sp_first(void);

#endif  /*_UTILS_H */
