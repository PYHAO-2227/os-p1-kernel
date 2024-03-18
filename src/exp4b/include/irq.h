#ifndef	_IRQ_H
#define	_IRQ_H

void enable_interrupt_controller( void );

void irq_vector_init( void );
void enable_irq( void );
void disable_irq( void );
extern void save_record_in(void);
extern void save_record_out(void);
extern void save_record_out_first(void);

#endif  /*_IRQ_H */
