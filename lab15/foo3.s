.text

.globl foo3

foo3:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    
    movq $0, -8(%rbp)
    movsd -8(%rbp), %xmm1
    movl $0, %ecx
    
    loop:
        cmpl %esi, %ecx
        jge fim
        
        movsd (%rdi), %xmm0
        
        movq %rdi, -8(%rbp)
        movsd %xmm1, -16(%rbp)
        movl %ecx, -20(%rbp)
        movl %esi, -24(%rbp)
        
        call sin 
        
        movq -8(%rbp), %rdi # recupera o ponteiro
        movsd -16(%rbp), %xmm1 # recupera r
        movl -20(%rbp), %ecx # recupera i
        movl -24(%rbp), %esi # recupera n
        
        
        addsd %xmm0, %xmm1
        
        addq $8, %rdi
        addl $1, %ecx
        jmp loop
        
    fim:
        movsd %xmm1, %xmm0
        leave
        ret
