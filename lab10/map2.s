.text
.globl map2

map2:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    
    movl $0, %ecx
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    
    loop:
    cmpl %edx, %ecx
    jge fim
    
    movl %ecx, -20(%rbp)
    
    imull $4, %ecx
    addq %rcx, %rdi
    movl (%rdi), %edi
    
    movl %ecx, -24(%rbp)
    call f 
    
    movq -8(%rbp), %rdi
    movq -16(%rbp), %rsi
    movl -20(%rbp), %ecx
    movl -24(%rbp), %r9d
    
    movq %rsi, %r10
    addq %r9, %r10
    
    movl %eax, (%r10)
    
    incl %ecx
    jmp loop
    
    fim:
    leave
    ret
    
