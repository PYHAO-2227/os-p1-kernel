.globl get_el
get_el:
	mrs x0, CurrentEL
	lsr x0, x0, #2
	ret

.globl put32
put32:
	str w1,[x0]
	ret

.globl get32
get32:
	ldr w0,[x0]
	ret

.globl delay
delay:
	subs x0, x0, #1
	bne delay
	ret

.globl get_sp
get_sp:
	mov x0, sp
	ret

.globl get_pc
get_pc:
	mrs x0, elr_el1
	ret

.globl get_pc_out
get_pc_out:
	mrs x0, elr_el1
	ret

.globl get_pc_first
get_pc_first:
	// ldr x0, [x8]
	mov x0, x19
	ret

.globl get_sp_first
get_sp_first:
	mov x0, x9
	ret
