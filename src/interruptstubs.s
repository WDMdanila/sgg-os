.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15handleInterruptEhj

.global _ZN16InterruptManager15ignoreInterruptEv

.macro HandleException number
.global _ZN16InterruptManager16handleException\number\()Ev
_ZN16InterruptManager19handleException\number\()Ev:
    movb $\number, (interruptnumber)
    jpm int_bottom
.endm

.macro handleInterruptRequest number
.global _ZN16InterruptManager26handleInterruptRequest\number\()Ev
_ZN16InterruptManager26handleInterruptRequest\number\()Ev:
    movb $\number + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

handleInterruptRequest 0x00
handleInterruptRequest 0x01

int_bottom:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEhj
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

_ZN16InterruptManager15ignoreInterruptEv:

    iret


.data
    interruptnumber: .byte 0
