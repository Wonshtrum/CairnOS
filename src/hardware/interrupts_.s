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

handle_exception 0x00
handle_exception 0x01
handle_exception 0x02
handle_exception 0x03
handle_exception 0x04
handle_exception 0x05
handle_exception 0x06
handle_exception 0x07
handle_exception 0x08
handle_exception 0x09
handle_exception 0x0A
handle_exception 0x0B
handle_exception 0x0C
handle_exception 0x0D
handle_exception 0x0E
handle_exception 0x0F
handle_exception 0x10
handle_exception 0x11
handle_exception 0x12
handle_exception 0x13

handle_interrupt 0x00
handle_interrupt 0x01
handle_interrupt 0x02
handle_interrupt 0x03
handle_interrupt 0x04
handle_interrupt 0x05
handle_interrupt 0x06
handle_interrupt 0x07
handle_interrupt 0x08
handle_interrupt 0x09
handle_interrupt 0x0A
handle_interrupt 0x0B
handle_interrupt 0x0C
handle_interrupt 0x0D
handle_interrupt 0x0E
handle_interrupt 0x0F

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
