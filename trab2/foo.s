.text

.globl foo

foo:
    pushq %rbp
    movq %rsp, %rbp
    movl %edi, %eax
    leave
    ret
