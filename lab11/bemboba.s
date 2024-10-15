.globl bemboba

bemboba:

pushq %rbp
movq %rsp, %rbp

# abrindo espaco na pilha
subq $32, %rsp # ja abri logo 32 pois seriam 16 do vetor e mais alguma outra coisa qualquer
movq %rbx, -8(%rbp) # salvando o rbx

movl $0, %ecx

# leaq -32(%rbp), %rbx # depois tem que lembrar de salvar esse registrador
movq %rbp, %rbx
subq $32, %rbx # -32 rbp eh p endereco de rbx


# a = local
movq %rbx, %r9
loop:

cmpl $4, %ecx
jge fim_for

movl %edi, (%r9) # *a = num
addq $4, %r9 # a++
addl $1, %ecx

jmp loop

fim_for:

movq %rbx, %rdi # passando o endereco
movl $4, %esi

call addl # ja devolve de em %eax
movq -8(%rbp), %rbx # recupera o valor de rbx
leave
ret
