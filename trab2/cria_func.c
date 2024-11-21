#include "cria_func.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

objdump -D -b binary -m i386:x86-64 codigo.bin
*/

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
    codigo[indice++] = 0x48; // subq 32 rsp
    codigo[indice++] = 0x83;
    codigo[indice++] = 0xec;
    codigo[indice++] = 0x20;
    // end epilogo

    // movq %rdi, -8(%rbp), PROVAVELMENTE NEM PRECISA, JÁ QUE ESTAMOS COPIANDO O VALOR NA MÃO GRANDE, DEPOIS REVER ESSE PARTE... (A NECESSIDADE)
    codigo[indice++] = 0x48;
    codigo[indice++] = 0x89;
    codigo[indice++] = 0x7d;
    codigo[indice++] = 0xf8;
    // end movq

    // copiar todos os parâmetros para os registradores corretos
    for (int i = 0; i < n; i++)
    {
        switch (params[i].tipo_val)
        {
        case INT_PAR:
            switch (params[i].orig_val)
            {
            case PARAM:
                // ja estão no lugar certo :)
                continue;
                break;
            case FIX:
                if (i == 0)
                    codigo[indice++] = 0xbf; // edi
                else if (i == 1)
                    codigo[indice++] = 0xbe; // esi
                else if (i == 2)
                    codigo[indice++] = 0xba; // edx

                memcpy(&codigo[indice], &params[i].valor.v_int, sizeof(int));
                indice += sizeof(int);
                // copia bem sucedida

                break;
            case IND:
            // o que eu movo aqui é o valor do ponteiro para r9 e depois (r9) para o respectivo reg
                int* endereco = (int*) params[i].valor.v_ptr;
                if (i == 0){
                // vou mover esse endereço para r9 se for o primeiro param 
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xb9;
                    memcpy(&codigo[indice], &endereco, sizeof(int *));
                    indice += sizeof(int*);
                    codigo[indice++] = 0x41;
                    codigo[indice++] = 0x8b;
                    codigo[indice++] = 0x39;
                }
                else if (i == 1){
                // vou mover esse endereço para r10 se for o segundo param
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xba;
                    memcpy(&codigo[indice], &endereco,  sizeof(int *));
                    indice += sizeof(int*);
                    codigo[indice++] = 0x41;
                    codigo[indice++] = 0x8b;
                    codigo[indice++] = 0x32;
                }
                else if (i == 2){
                // vou mover esse endereco para r11 se for o terceiro param
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xbb;
                    memcpy(&codigo[indice], &endereco, sizeof(int *));
                    indice += sizeof(int*);
                    codigo[indice++] = 0x41;
                    codigo[indice++] = 0x8b;
                    codigo[indice++] = 0x13;
                }
                break;
            }
            break;

        case PTR_PAR:
            switch (params[i].orig_val)
            {
            case PARAM:
                // TODO
                // já era aqui :)
                continue;
                break;

            case FIX:
                if (i == 0){
                    codigo[indice++] = 0x48;
                    codigo[indice++] = 0xbf; // rdi
                }
                else if (i == 1){
                    codigo[indice++] = 0x48;
                    codigo[indice++] = 0xbe; // rsi
                    }
                else if (i == 2){
                    codigo[indice++] = 0x48;
                    codigo[indice++] = 0xba; // rdx
                }
                memcpy(&codigo[indice], params[i].valor.v_ptr, sizeof(void *));
                printf("Endereço recebido em cria_func: %p\n", params[i].valor.v_ptr)    ;
                indice += sizeof(void *);            
                break;
            case IND:
            printf("Não era aqui\n");
                // TODO
                break;
            }
            break;
        }
    }

    // ESTAVA PERDENDO A REFERENCIA PARA A FUNCAO, ESTÃO COPIEI NA MÃO GRANDE
    codigo[indice++] = 0x48;
    codigo[indice++] = 0xb8;                     // movq %rax, <endereço da função>, o compilar coloca como movabs...
    memcpy(&codigo[indice], &f, sizeof(void *)); // Copia o endereço da função para o código
    indice += sizeof(void *);

    // call *%rax (endereco da funcao chamada) para fazer as operacoes que devem ser feitas, por exemplo, como seriam feitas na mult ou na memcmp...
    codigo[indice++] = 0xff;
    codigo[indice++] = 0xd0;
    // end call

    // finalização
    codigo[indice++] = 0xc9;
    codigo[indice++] = 0xc3;
    // end finalização

    printf("Código gerado:\n");
    for (int j = 0; j < indice; j++)
    {
        printf("%02x ", codigo[j]);
    }
    printf("\n");

    FILE *fout = fopen("codigo.bin", "wb");
    if (fout == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }
    fwrite(codigo, sizeof(unsigned char), indice, fout);
    fclose(fout);

    printf("Código gerado gravado em 'codigo.bin'.\n");
}
