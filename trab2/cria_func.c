#include <stdio.h>

// pushq %rbp = 0x55
// movq %rsp, %rbp = 0x48, 0x89, 0xe5
// mov const reg = (codigo reg), (valorHex)  --> por exemplo movl $5, %esi = 0xbf 0x04 0x00 0x00 0x00 e se fosse movl $5, %esi 0xbe 0x05 0x00 0x00 0x00, perceba que sao 4 bytes o inteiro!
// Esse eu não sei direito: a instrucao mov, as vezes tem 3 componentes as vezes tem apenas duas, mas sempre tem 0x89.
// leave = 0xc9
// ret = 0xc3

int foo(int a);

int main(void)
{
    printf("%d\n", foo(1));
    return 0;
}
