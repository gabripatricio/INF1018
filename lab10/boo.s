.text

.globl boo


boo:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    loop:

    cmpl $0, %esi
    je fim

    # movendo rdi para a pilha
    movq %rdi, -12(%rbp)
    # salvando n na pilha
    movl %esi, -4(%rbp)
    # salvando val na pilha
    movl %edx, -16(%rbp)

    movl %edx, %esi
    call f
    # Pegando o valor de volta
    movq -12(%rbp), %rdi
    movl %eax, 4(%rdi)
    movl -4(%rbp), %esi
    movl -16(%rbp), %edx

    addq $8, %rdi
    decl %esi

    jmp loop

    fim:
    leave
    ret
