.text

.globl add

add:


# inicio (abrindo espaco na pilha)
pushq %rbp
movq %rsp, %rbp

cmpq $0, %rdi # x == NULL?

je end

movl (%rdi), %ebx # x -> val

movq 8(%rdi), %rdi # tem que aumentar pois vamos chamar a funcao com x->next
call add  # que vai me retornar em %eax

addl %ebx, %eax


# return

leave
ret

end:
movl $0, %eax
leave
ret
