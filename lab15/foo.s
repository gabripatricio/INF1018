.text

.globl foo

foo:
    pushq %rbp
    movq %rsp, %rbp 
    
    cvtss2sd %xmm1, %xmm1
    
    movsd %xmm0, %xmm2
    addsd %xmm1, %xmm2 # a + b 
    
    movsd %xmm0, %xmm3
    subsd %xmm1, %xmm3
    
    mulsd %xmm2, %xmm3
    movsd %xmm3, %xmm0
    
    cvtsd2ss %xmm0, %xmm0
    leave
    ret

/*
Outra abordagem usando mais a pilha:
.text

.globl foo

foo:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    
    movsd %xmm0, -8(%rbp)
    cvtss2sd %xmm1, %xmm1
    movsd %xmm1, -16(%rbp)
    
    // -8(%rbp) == a
    // -16(%rbp) == b
    // -24(%rbp) == soma
    // -32 (%rbp) == menos
    
    addsd %xmm1, %xmm0
    movsd %xmm0, -24(%rbp)
    
    movsd -8(%rbp), %xmm0
    subsd %xmm1, %xmm0
    movsd %xmm0, -32(%rbp)
    
    movsd -24(%rbp), %xmm0
    mulsd -32(%rbp), %xmm0
    
    cvtsd2ss %xmm0, %xmm0

    leave
    ret

*/
