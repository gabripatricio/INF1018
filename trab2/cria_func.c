#include "cria_func.h"
#include <stdlib.h>
#include <stdio.h>

// GABRIEL PATRICIO DE OLIVEIRA 2310806
// JOAO VITOR MALLET MALHEIROS

/*

pushq %rbp = 0x55
movq %rsp, %rbp = 0x48, 0x89, 0xe5
mov const reg = (codigo reg), (valorHex)  --> por exemplo movl $5, %esi = 0xbf 0x04 0x00 0x00 0x00 e se fosse movl $5, %esi 0xbe 0x05 0x00 0x00 0x00, perceba que sao 4 bytes o inteiro! Esse eu não sei direito: a instrucao mov, as vezes tem 3 componentes as vezes tem apenas duas, mas sempre tem 0x89.
leave = 0xc9
ret = 0xc3

movl constante, %esi = be constante
movl constante, %edx = ba constante

gcc -Wall -Wa,--execstack -o main cria_func.c main.c

gcc -c -o foo.o foo.s
objdump -d foo.o
*/

unsigned char *pegaBytes(int numero)
{
    unsigned char *bytes = (unsigned char *)malloc(4 * sizeof(unsigned char));

    bytes[0] = (numero & 0xFF);       // Byte menos significativo
    bytes[1] = (numero >> 8) & 0xFF;  // Próximo byte
    bytes[2] = (numero >> 16) & 0xFF; // Segundo byte mais significativo
    bytes[3] = (numero >> 24) & 0xFF; // Byte mais significativo

    return bytes;
}

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{
    printf("Endereço recebido: %p\n", f);
    // TA COM BASTANTE COMENTARIO PARA A GENTE NÃO SE PERDER... (DEPOIS TIRAMOS ALGUNS
    int indice = 0;

    // epilogo da funcao, i.e., pushq rbp, movq rsp rbp, subq 32 (pode ser necessario aumentar)
    codigo[indice++] = 0x55;
    codigo[indice++] = 0x48;
    codigo[indice++] = 0x89;
    codigo[indice++] = 0xe5;
    codigo[indice++] = 0x48; //subq 32 rsp
    codigo[indice++] = 0x83;
    codigo[indice++] = 0xec;
    codigo[indice++] = 0x20;
    // end epilogo

    // movq %rdi, -8(%rbp) salvando o valor do endereco original da funcao, pode ser sobrescrito durante a chamada de alguma funcao qualquer, rdi não é callee saved...
    codigo[indice++] = 0x48;
    codigo[indice++] = 0x89;
    codigo[indice++] = 0x7d;
    codigo[indice++] = 0xf8;
    // end

    
    // copiar todos os parâmetros para os registradores corretos
    for (int i = 0; i < n; i++)
    {
        switch (params[i].tipo_val)
        {
        case INT_PAR:
            switch (params[i].orig_val)
            {
            case PARAM:
                printf("Não era aqui.");
                break;
            case FIX:
                if (i == 0)
                    codigo[indice++] = 0xbf;
                else if (i == 1)
                    codigo[indice++] = 0xbe;
                else if (i == 2)
                    codigo[indice++] = 0xba;

                unsigned char *bytes = pegaBytes(params[i].valor.v_int);

                for (int k = 0; k < 4; k++)
                {
                    codigo[indice++] = bytes[k];
                }

                free(bytes);

                break;
            case IND:
                printf("Não era aqui.");
                break;
            }
            break;

        case PTR_PAR:
            switch (params[i].orig_val)
            {
            case PARAM:
                printf("Não era aqui.");
                break;

            case FIX:
                printf("Não era aqui.");
                break;
            case IND:
                printf("Não era aqui.");
                break;
            }
            break;
        }
    }

    // movq -8(%rbp), %rdi (recupera o endereco orignal da funcao passada como parametro)
    codigo[indice++] = 0x48;
    codigo[indice++] = 0x8b;
    codigo[indice++] = 0x7d;
    codigo[indice++] = 0xf8;
    // end recuperacao

    // call *%rdi (endereco da funcao chamada) para fazer as operacoes que devem ser feitas, por exemplo, como seriam feitas na mult ou na memcmp...
    codigo[indice++] = 0xff;
    codigo[indice++] = 0xd7;
    // end call

    // finalização
    codigo[indice++] = 0xc9;
    codigo[indice++] = 0xc3;

    printf("Código gerado:\n");
    for (int j = 0; j < indice; j++)
    {
        printf("%02x ", codigo[j]);
    }
    printf("\n");
}
