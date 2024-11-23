.text

.globl foo2

foo2:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    
    // vou converter todos para double (mais facil)
    cvtss2sd %xmm0, %xmm0
    cvtss2sd %xmm1, %xmm1
    
    // salva xmm1
    movsd %xmm1, -16(%rbp)
    
    call sin 
    movsd %xmm0, -8(%rbp)
    
    movsd -16(%rbp), %xmm0
    call cos
    
    addsd -8(%rbp), %xmm0 # -8(%rbp) eh o retorno do sin
    
    leave
    ret
    
