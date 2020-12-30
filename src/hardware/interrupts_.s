.set IRQ_BASE, 0x20
.section .text
.extern _ZN17Interrupt_manager16handle_interruptEhj
.global _ZN17Interrupt_manager16ignore_interruptEv

.macro handle_exception number
	.global _ZN17Interrupt_manager21handle_exception_\number\()Ev
		_ZN17Interrupt_manager21handle_exception_\number\()Ev:
		movb $\number, (interrupt_number)
		// error state is set by the CPU itself
		jmp interrupt_handler_call
.endm

.macro handle_interrupt number
	.global _ZN17Interrupt_manager21handle_interrupt_\number\()Ev
		_ZN17Interrupt_manager21handle_interrupt_\number\()Ev:
		movb $\number + IRQ_BASE, (interrupt_number)
		// error state is set to 0
		pushl $0
		jmp interrupt_handler_call
.endm

handle_interrupt 0x00
handle_interrupt 0x01
handle_interrupt 0x0C

interrupt_handler_call:
	/*
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
	*/
	pushl %ebp
	pushl %edi
	pushl %esi

	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

	pushl %esp
	push (interrupt_number)
	call _ZN17Interrupt_manager16handle_interruptEhj
	movl %eax, %esp

	popl %eax
	popl %ebx
	popl %ecx
	popl %edx

	popl %esi
	popl %edi
	popl %ebp
	/*
	popl %gs
	popl %fs
	popl %es
	popl %ds
	*/

	add $4, %esp

_ZN17Interrupt_manager16ignore_interruptEv:
	iret

.data
	interrupt_number: .byte 0
