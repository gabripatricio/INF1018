.text

.globl bemboba

bemboba:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp # 16 do vetor, mais alguma tralha 
    
    movl $0, %ecx
    movq %rbp, %rdx
    subq $16, %rdx # rdx = -16(%rbp)
    
    movq %rdx, -24(%rbp) # apenas salvando o endereco para usar depois
    
    loop:
    cmpl $4, %ecx
    jge fim
    
    movl %edi, (%rdx)
    
    addq $4, %rdx
    addl $1, %ecx
    jmp loop
    
    
    fim:
    movq -24(%rbp), %rdi # pegando o endereco de volta para passar corretamente
    movl $4, %esi 
    call addl
    
    leave
    ret
    
