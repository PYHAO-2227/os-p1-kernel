The files that I modifiied:
entry.s
boot.s
mm.h
entry.h
irq.c
kernel.c

The handler function is defined in the irq.c(mimicing the handler_irq);
The vector table init and SP init is in the "master" part of boot.s;