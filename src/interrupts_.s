.set IRQ_BASE, 0x20
.section .text
.extern _ZN17Interrupt_Manager16handle_interruptEhj
.global _ZN17Interrupt_Manager16ignore_interruptEv

.macro handle_exception number
	.global _ZN17Interrupt_Manager21handle_exception_\number\()Ev
		_ZN17Interrupt_Manager21handle_exception_\number\()Ev:
		movb $\number, (interrupt_number)
		jmp interrupt_handler_call
.endm

.macro handle_interrupt number
	.global _ZN17Interrupt_Manager21handle_interrupt_\number\()Ev
		_ZN17Interrupt_Manager21handle_interrupt_\number\()Ev:
		movb $\number + IRQ_BASE, (interrupt_number)
		jmp interrupt_handler_call
.endm

handle_interrupt 0x00
handle_interrupt 0x01

interrupt_handler_call:
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	pushl %esp
	push (interrupt_number)
	call _ZN17Interrupt_Manager16handle_interruptEhj
	movl %eax, %esp

	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa

_ZN17Interrupt_Manager16ignore_interruptEv:
	iret

.data
	interrupt_number: .byte 0
