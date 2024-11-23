.text

.globl map2

map2:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movl $0, %ecx
    
    loop:
        cmpl %edx, %ecx # i < n ????
        jge fim

        movl %ecx, %r10d
        imull $4, %r10d
        
        movq %rdi, -8(%rbp)
        movq %rsi, -16(%rbp)
        movl %ecx, -20(%rbp)
        movl %r10d, -24(%rbp) 

        addq %r10, %rdi
        addq %r10, %rsi


        movl (%rsi), %edi
        call f

        movq -8(%rbp), %rdi
        movq -16(%rbp), %rsi
        movl -20(%rbp), %eax
        movl -24(%rbp), %r10d

        movq %rsi, %r9
        addq %r10, %r9
        movl %eax, (%r9)        

        addl $1, %ecx # i++
        jmp loop

    fim:
        leave
        ret
