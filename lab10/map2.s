# OBSERVACAO: NAO DEU TEMPO DE TERMINAR... TALVEZ EDITE NO FUTURO.

.text 
.globl map2

map2:

    # inicio ... 
    pushq %rbp
    movq %rsp, %rbp


    movl $0, %ecx # i = 0

loop:

    cmpl %ecx, %edx

    je fim

    movl (%rsi, %rcx, 4), % ebx # *(outro + i)

    pushq %rdi # salva o registrador


    movl (%rdi, %rcx, 4), %edi # chamado com *( um+i)
    call f

    movl %eax, %ebx
    popq %rdi

    addl $1, %ecx  

    jmp loop


fim:
    # retornando
    leave 
    ret
