.text

.globl add

add:
    pushq %rbp
    movq %rsp, %rbp
    
    movl $0, %ecx
    
    loop:
    cmpq $0, %rdi
    je fim
    
    addl (%rdi), %ecx
    
    movq 8(%rdi), %rdi 
    jmp loop
    
    fim:
    movl %ecx, %eax
    leave
    ret
    
