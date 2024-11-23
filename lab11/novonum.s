.data
Sf: .string "numero: "
percent_d: .string "%d"

.text

.globl novonum

novonum:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    
    movq $Sf, %rdi
    call printf
    
    movq $percent_d, %rdi
    
    # leaq -4(%rbp), %rsi # load effective address de -4 rbp
    
    movq %rbp, %rsi # iguala o endereco a uma regiao da pilha (leaq)
    subq $4, %rsi
    call scanf
    
    movl -4(%rbp), %eax
    
    leave
    ret
