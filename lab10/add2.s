
.text
 
.globl add
 
add:

pushq %rbp 
movq %rsp, %rbp

movl $0, %ecx # int a = 0
 
loop:
 
cmpq $0, %rdi # != NULL
 
je end_for 
 

addl (%rdi), %ecx
movq 8(%rdi), %rdi
jmp loop
 
 
end_for:
movl %ecx, %eax
leave
ret
