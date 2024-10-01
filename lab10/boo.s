# 1 %rdi %edi %di %dil
# 2 %rsi %esi %si %sil
# 3 %rdx %edx %dx %dl

.text 

.globl boo

boo:
pushq %rbp
movq %rsp, %rbp


loop:

cmpl $0, %esi

je end


movl 4(%rdi), %ebx # ebx += px->val2

pushq %rdi # salva rdi antes de passar na chamada de funcao (ponteiro)
pushq %rdx # idem (val)
pushq %rsi # salva n


# ajustando os parametros

movl (%rdi), %edi # px->val1 para o primeiro parametro
# como eu nao sei o comportamento de f eu vou assumir que eu corretamente salvei o valor de edx... mas posso simplesmente passar edx (segundo parametro)

call f

popq %rsi
popq %rdx
popq %rdi

addq $8, %rdi

subl $1, %esi # n --


jmp loop


end:
leave 
ret
