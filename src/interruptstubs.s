.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15handleInterruptEhj

.global _ZN16InterruptManager15ignoreInterruptEv

.macro handleException number
.global _ZN16InterruptManager19handleException\number\()Ev
_ZN16InterruptManager19handleException\number\()Ev:
    movb $\number, (interruptnumber)
    jmp int_bottom
.endm

.macro handleInterruptRequest number
.global _ZN16InterruptManager26handleInterruptRequest\number\()Ev
_ZN16InterruptManager26handleInterruptRequest\number\()Ev:
    movb $\number + IRQ_BASE, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm

handleException 0x00
handleException 0x01
handleException 0x02
handleException 0x03
handleException 0x04
handleException 0x05
handleException 0x06
handleException 0x07
handleException 0x08
handleException 0x09
handleException 0x0A
handleException 0x0B
handleException 0x0C
handleException 0x0D
handleException 0x0E
handleException 0x0F
handleException 0x10
handleException 0x11
handleException 0x12
handleException 0x13

handleInterruptRequest 0x00
handleInterruptRequest 0x01
handleInterruptRequest 0x02
handleInterruptRequest 0x03
handleInterruptRequest 0x04
handleInterruptRequest 0x05
handleInterruptRequest 0x06
handleInterruptRequest 0x07
handleInterruptRequest 0x08
handleInterruptRequest 0x09
handleInterruptRequest 0x0A
handleInterruptRequest 0x0B
handleInterruptRequest 0x0C
handleInterruptRequest 0x0D
handleInterruptRequest 0x0E
handleInterruptRequest 0x0F
handleInterruptRequest 0x31

int_bottom:
    #pusha
    #pushl %ds
    #pushl %es
    #pushl %fs
    #pushl %gs

    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEhj
    movl %eax, %esp

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp

    #popl %gs
    #popl %fs
    #popl %es
    #popl %ds
    #popa

    add $4, %esp

_ZN16InterruptManager15ignoreInterruptEv:

    iret


.data
    interruptnumber: .byte 0
