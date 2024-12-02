#include "cria_func.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// GABRIEL PATRICIO DE OLIVEIRA 2310806
// JOAO VITOR MALLET MALHEIROS 2310604

/*
Para compilar:
gcc -Wall -Wa,--execstack -o main cria_func.c main.c

Para gerar código de máquina
gcc -c -o foo.o foo.s
objdump -d foo.o

*/

void finalizacao(unsigned char *c, int *i);
void prologo(unsigned char *c, int *i);
void movrdirsi(unsigned char *c, int *i);
void movrdirdx(unsigned char *c, int *i);
void movrsirdx(unsigned char *c, int *i);

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{
    int indice = 0;

    prologo(codigo, &indice);

    // tem que ver se está tudo correto, pois quem chama pode não passar todos os parâmetros, o que desalinha
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
            if (contaPARAM == 1)
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
                indice += sizeof(void *);
                break;

            case IND:
                void *endereco = params[i].valor.v_ptr;
                if (i == 0)
                {
                    // move para r9 e desreferencia em rdi
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xb9;
                    memcpy(&codigo[indice], &endereco, sizeof(void *));
                    indice += sizeof(void *);
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0x8b;
                    codigo[indice++] = 0x39;
                }
                else if (i == 1)
                {
                    // move para r10 e desreferencia em rsi
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xba;
                    memcpy(&codigo[indice], &endereco, sizeof(void *));
                    indice += sizeof(void *);
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0x8b;
                    codigo[indice++] = 0x32;
                }
                else if (i == 2)
                {
                    // move para r11 e desreferencia em rdx
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0xbb;
                    memcpy(&codigo[indice], &endereco, sizeof(void *));
                    indice += sizeof(void *);
                    codigo[indice++] = 0x49;
                    codigo[indice++] = 0x8b;
                    codigo[indice++] = 0x13;
                }
                break;
            }
            break;
        }
    }

    // copio o endereco da funcao para o lugar certo
    codigo[indice++] = 0x48;
    codigo[indice++] = 0xb8;                     // movq %rax, <endereço da função>, o compilar coloca como movabs...
    memcpy(&codigo[indice], &f, sizeof(void *)); // Copia o endereço da função para o código
    indice += sizeof(void *);

    // call *%rax (endereco da funcao chamada) para fazer as operacoes que devem ser feitas, por exemplo, como seriam feitas na mult ou na memcmp...
    codigo[indice++] = 0xff;
    codigo[indice++] = 0xd0;
    // end call

    // finalização
    finalizacao(codigo, &indice);
    // end finalização
}

void finalizacao(unsigned char *codigo, int *indice)
{
    codigo[(*indice)++] = 0xc9; // leave
    codigo[(*indice)++] = 0xc3; // ret
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

void movrdirsi(unsigned char *codigo, int *indice)
{
    codigo[(*indice)++] = 0x48;
    codigo[(*indice)++] = 0x89;
    codigo[(*indice)++] = 0xfe; // mov %rdi, %rsi
}

void movrsirdx(unsigned char *codigo, int *indice)
{
    codigo[(*indice)++] = 0x48;
    codigo[(*indice)++] = 0x89;
    codigo[(*indice)++] = 0xf2; // mov %rsi, %rdx
}

void movrdirdx(unsigned char *codigo, int *indice)
{
    codigo[(*indice)++] = 0x48;
    codigo[(*indice)++] = 0x89;
    codigo[(*indice)++] = 0xfa; // mov %rdi, %rdx
}
