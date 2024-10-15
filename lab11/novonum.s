.data
Sf: .string "numero: "
percent_d: .string "%d"


.globl novonum

.text
novonum:

pushq %rbp # RA
movq %rsp, %rbp # RA
subq $16, %rsp # abrindo espaco na pilha


movq $Sf, %rdi
movl $0, %eax
call printf # nao tem segundo parametro


movq $percent_d, %rdi
# leaq -4(%rbp), %rsi # load effective address de -4 rbp

# podemos fazer o leaq assim:

movq %rbp, %rsi
subq $4, %rsi

call scanf
movl -4(%rbp), %eax

leave
ret
