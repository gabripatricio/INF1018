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
void epilogo(unsigned char* c, int* i);
void prologo(unsigned char* c, int* i);
void movrdirsi(unsigned char* c, int* i);
void movrdirdx(unsigned char* codigo, int* indice);
void movrsirdx(unsigned char* c, int* i);


void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{
    printf("Endereço recebido: %p\n", f);
    // TA COM BASTANTE COMENTARIO PARA A GENTE NÃO SE PERDER... (DEPOIS TIRAMOS ALGUNS
    int indice = 0;

    prologo(codigo, &indice);

    // movq %rdi, -8(%rbp), PROVAVELMENTE NEM PRECISA, JÁ QUE ESTAMOS COPIANDO O VALOR NA MÃO GRANDE, DEPOIS REVER ESSE PARTE... (A NECESSIDADE)
    codigo[indice++] = 0x48;
    codigo[indice++] = 0x89;
    codigo[indice++] = 0x7d;
    codigo[indice++] = 0xf8;
    // end movq


    // tem que ver se está tudo corretamente, pois que chama pode não passar todos os parâmetros
    int contaPARAM = 0;
    for (int i = 0; i < n; i++)
    {
        if (params[i].orig_val == PARAM)
            contaPARAM++;
    }

    if (contaPARAM != n)
    {
        if (n == 2)
        {
            if (params[0].orig_val != PARAM)
            {
                // tenho que mover de di para si
                movrdirsi(codigo, &indice);
            }
        }

        if (n == 3)
        {
            if(contaPARAM == 1)
            {
                if (params[0].orig_val != PARAM)
                {
                    if (params[1].orig_val == PARAM)
                    {
                        // tenho que mover de di para si
                        movrdirsi(codigo, &indice);
                    }
                    else if (params[2].orig_val == PARAM)
                    {
                        // Movo de rdi para rdx
                        movrdirdx(codigo, &indice);
                    }
                }
                // nao tem else, pois está no lugar certo.
            }
            else // 2 parametros, 3 não pode pq não é != n
            {
                if (params[0].orig_val != PARAM)
                {
                    // tenho que shiftar todos para a direita
                    // fazendo si --> dx primeiro
                    movrsirdx(codigo, &indice);

                    // Movo de rdi para rsi
                    movrdirsi(codigo, &indice);
                }
                if (params[1].orig_val != PARAM)
                {
                    // so tenho que mover um, pois significa que o outro ta no lugar certo (di)
                    movrsirdx(codigo, &indice);
                }
            }
        }
    }

    /*
    n == 2:

    Caso (1º FIX, 2º PARAM): Move rdi → rsi. Resolvido.
    Caso (1º PARAM, 2º FIX): Nenhuma ação necessária. Resolvido.

    n == 3:

    1º FIX, 2º PARAM, 3º FIX: Move rdi → rsi. Resolvido.
    1º FIX, 2º FIX, 3º PARAM: Move rdi → rdx. Resolvido.
    1º PARAM, 2º FIX, 3º FIX: Nenhuma ação necessária. Resolvido.
    1º FIX, 2º FIX, 3º FIX (não há PARAM): Nenhuma ação necessária. Resolvido.
    1º FIX, 2º PARAM, 3º PARAM:
        Move rsi → rdx.
        Move rdi → rsi. Resolvido.
    1º PARAM, 2º FIX, 3º PARAM:
        Move rsi → rdx. Resolvido.
    
    */

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
                int *endereco = (int *)params[i].valor.v_ptr;
                if (i == 0)
                {
                    // vou mover esse endereço para r9 se for o primeiro param
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xb9;
                    memcpy(&codigo[indice], &endereco, sizeof(int *));
                    indice += sizeof(int *);
                    codigo[indice++] = 0x41;
                    codigo[indice++] = 0x8b;
                    codigo[indice++] = 0x39;
                }
                else if (i == 1)
                {
                    // vou mover esse endereço para r10 se for o segundo param
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xba;
                    memcpy(&codigo[indice], &endereco, sizeof(int *));
                    indice += sizeof(int *);
                    codigo[indice++] = 0x41;
                    codigo[indice++] = 0x8b;
                    codigo[indice++] = 0x32;
                }
                else if (i == 2)
                {
                    // vou mover esse endereco para r11 se for o terceiro param
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xbb;
                    memcpy(&codigo[indice], &endereco, sizeof(int *));
                    indice += sizeof(int *);
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
                if (i == 0)
                {
                    codigo[indice++] = 0x48;
                    codigo[indice++] = 0xbf; // rdi
                }
                else if (i == 1)
                {
                    codigo[indice++] = 0x48;
                    codigo[indice++] = 0xbe; // rsi
                }
                else if (i == 2)
                {
                    codigo[indice++] = 0x48;
                    codigo[indice++] = 0xba; // rdx
                }
                memcpy(&codigo[indice], &params[i].valor.v_ptr, sizeof(void *));
                printf("Endereço recebido em cria_func: %p\n", params[i].valor.v_ptr);
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
    epilogo(codigo, &indice);
    // end finalização

    printf("Código gerado:\n");
    for (int j = 0; j < indice; j++)
    {
        printf("%02x ", codigo[j]);
    }
    printf("\n");

    FILE *fout = fopen("codigo.bin", "wb");
    if (fout == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }
    fwrite(codigo, sizeof(unsigned char), indice, fout);
    fclose(fout);

    printf("Código gerado gravado em 'codigo.bin'.\n");
}

void epilogo(unsigned char *codigo, int *indice) {
    // leave
    codigo[(*indice)++] = 0xc9;
    // ret
    codigo[(*indice)++] = 0xc3;
}

void prologo(unsigned char *codigo, int *indice)
{
    // prologo da funcao, i.e., pushq rbp, movq rsp rbp, subq 32 (pode ser necessario aumentar)
    codigo[(*indice)++] = 0x55;
    codigo[(*indice)++] = 0x48;
    codigo[(*indice)++] = 0x89;
    codigo[(*indice)++] = 0xe5;
    codigo[(*indice)++] = 0x48; // subq 32 rsp
    codigo[(*indice)++] = 0x83;
    codigo[(*indice)++] = 0xec;
    codigo[(*indice)++] = 0x20;
    // end prologo
}

void movrdirsi(unsigned char* codigo, int* indice)
{
    codigo[(*indice)++] = 0x48;
    codigo[(*indice)++] = 0x89;
    codigo[(*indice)++] = 0xfe; // mov %rdi, %rsi
}

void movrsirdx(unsigned char* codigo, int* indice)
{
    codigo[(*indice)++] = 0x48;
    codigo[(*indice)++] = 0x89;
    codigo[(*indice)++] = 0xf2; // mov %rsi, %rdx
}

void movrdirdx(unsigned char* codigo, int* indice)
{
    codigo[(*indice)++] = 0x48;
    codigo[(*indice)++] = 0x89;
    codigo[(*indice)++] = 0xfa; // mov %rdi, %rdx
}
