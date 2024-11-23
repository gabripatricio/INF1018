.text

.globl foo1

foo1:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    
    cvtss2sd %xmm0, %xmm0
    movss %xmm1, -4(%rbp) 
    call sin 
    
    movss -4(%rbp), %xmm1
    cvtss2sd %xmm1, %xmm1
    addsd %xmm1, %xmm0
    
    cvtsd2ss %xmm0, %xmm0
    
    leave
    ret
    
