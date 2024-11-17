.text

.globl foo

foo:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movl $1, %edi
    movl $1, %esi
    movl $1, %edx

    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq %rdx, -24(%rbp)

    movq $0, %rdi
    movq $0, %rsi
    movq $0, %rdx
    
    movq %rdi, %rax
    call *%rax

    
    leave
    ret
