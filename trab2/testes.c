#include <stdio.h>
#include <string.h>
#include "cria_func.h"

typedef int (*func_ptr)(void* candidata, size_t n);
char fixa[] = "quero saber se a outra string é um prefixo dessa";


int mult(int x, int y);  // Declaração de mult
int somar(int x, int y); // Declaração de somar
int teste3(void);
int teste4(void);
int teste5(void);
int teste6(void);

int main(void) {
    int quantidade = 0;
    int total = 4;  // Agora temos 5 testes.

    int teste3_r = teste3();
    if (teste3_r) {
        printf("T3: OK\n");
        quantidade++;
    } else {
        printf("T3: Fail\n");
    }

    int teste4_r = teste4();
    if (teste4_r) {
        printf("T4: OK\n");
        quantidade++;
    } else {
        printf("T4: Fail\n");
    }

    int teste5_r = teste5();
    if (teste5_r) {
        printf("T5: OK\n");
        quantidade++;
    } else {
        printf("T5: Fail\n");
    }

    int teste6_r = teste6();
    if (teste6_r) {
        printf("T6: OK\n");
        quantidade++;
    } else {
        printf("T6: Fail\n");
    }

    printf("Passou em %d/%d testes.\n", quantidade, total);
  
    return 0;
}

int teste3(void) {
    DescParam params[3];    // Declarando o vetor de parâmetros.
    func_ptr mesmo_prefixo; // Ponteiro para a função gerada dinamicamente.
    char s[] = "quero saber tudo";
    int tam;
    unsigned char codigo[500]; // Código gerado para a função.

    // Definindo o primeiro parâmetro de 'memcmp' (ponteiro para char).
    params[0].tipo_val = PTR_PAR; 
    params[0].orig_val = FIX;    
    params[0].valor.v_ptr = fixa;

    // Definindo o segundo parâmetro de 'memcmp' (ponteiro para char).
    params[1].tipo_val = PTR_PAR;
    params[1].orig_val = PARAM;   
    params[1].valor.v_ptr = NULL; // Este valor será configurado pela função gerada.

    // Definindo o terceiro parâmetro de 'memcmp' (tamanho do prefixo).
    params[2].tipo_val = INT_PAR;
    params[2].orig_val = PARAM;
    params[2].valor.v_int = 0;  // Este valor será configurado pela função gerada.

    // Gerando a função a partir de 'memcmp' com os parâmetros configurados.
    cria_func(memcmp, params, 3, codigo);
    mesmo_prefixo = (func_ptr)codigo;  // Cast do código gerado para um ponteiro para função.

    // Testando a função gerada.
    tam = 12;
    printf("'%s' tem mesmo prefixo-%d de '%s'? %s\n", s, tam, fixa, mesmo_prefixo(s, tam) ? "NAO" : "SIM");

    // Testando com o comprimento completo da string.
    tam = strlen(s);
    printf("'%s' tem mesmo prefixo-%d de '%s'? %s\n", s, tam, fixa, mesmo_prefixo(s, tam) ? "NAO" : "SIM");

    return 1;  // Retorna 1 para indicar sucesso.
}

int teste4(void) {
    // Testando amarração de um valor fixo em um parâmetro inteiro.
    DescParam params[2];
    func_ptr multiplicar_por_10;
    unsigned char codigo[500];
    
    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;
    params[0].valor.v_int = 0;  // Este valor será configurado pela função gerada.
    
    params[1].tipo_val = INT_PAR;
    params[1].orig_val = FIX;
    params[1].valor.v_int = 10;  // O segundo parâmetro será amarrado a 10.
    
    cria_func((void*)mult, params, 2, codigo);  // A função 'mult' deve ser criada dinamicamente.
    multiplicar_por_10 = (func_ptr)codigo;

    printf("Multiplicando 5 por 10: %d\n", multiplicar_por_10((void*)5, 0));  // 5 * 10

    return 1;
}

int teste5(void) {
    // Testando amarração indireta de um valor de variável.
    DescParam params[2];
    func_ptr multiplicar_por_valor;
    unsigned char codigo[500];
    
    int valor = 3;
    
    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;
    params[0].valor.v_int = 0;  // Este valor será configurado pela função gerada.
    
    params[1].tipo_val = INT_PAR;
    params[1].orig_val = IND;
    params[1].valor.v_ptr = &valor;  // O segundo parâmetro será amarrado à variável 'valor'.
    
    cria_func((void*)mult, params, 2, codigo);  // A função 'mult' será usada com o valor da variável.
    multiplicar_por_valor = (func_ptr)codigo;

    printf("Multiplicando 5 por valor %d: %d\n", valor, multiplicar_por_valor((void*)5, 0));  // 5 * 3

    return 1;
}

int teste6(void) {
    // Testando a amarração de parâmetros com uma combinação de FIX e PARAM.
    DescParam params[2];
    func_ptr somar_valor_fixo;
    unsigned char codigo[500];
    
    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;
    params[0].valor.v_int = 0;  // Este valor será configurado pela função gerada.
    
    params[1].tipo_val = INT_PAR;
    params[1].orig_val = FIX;
    params[1].valor.v_int = 7;  // O segundo parâmetro será amarrado a 7.
    
    cria_func((void*)somar, params, 2, codigo);  // A função 'somar' será criada dinamicamente.
    somar_valor_fixo = (func_ptr)codigo;

    printf("Somando 5 e 7: %d\n", somar_valor_fixo((void*)5, 0));  // 5 + 7

    return 1;
}

// Funções exemplo mult e somar para os testes
int mult(int x, int y) {
    return x * y;
}

int somar(int x, int y) {
    return x + y;
}
