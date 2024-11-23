.text

.globl add2

add2:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    
    cmpq $0, %rdi
    je fim
    
    movl (%rdi), %ecx
    movq 8(%rdi), %rdi
    
    movl %ecx, -4(%rbp)
    
    call add2
    
    movl -4(%rbp), %ecx
    
    addl %ecx, %eax
    
    leave
    ret
    
    fim:
    movl $0, %eax
    leave
    ret
