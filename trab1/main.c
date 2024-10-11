#include <stdio.h>
#include <stdlib.h>
#include "converteutf832.h"

int total_passed = 0;  // Contador de casos que passaram

void test_convUtf8p32() {
    FILE *input = fopen("teste_escrito.txt", "rb");
    FILE *output = fopen("output_utf32_le.bin", "wb");
    
    if (input == NULL || output == NULL) {
        fprintf(stderr, "Erro ao abrir arquivos de teste para convUtf8p32\n");
        exit(EXIT_FAILURE);
    }

    int result = convUtf8p32(input, output);
    
    if (result == 0) {
        total_passed++;
        printf("Test convUtf8p32: PASSED\n");
    } else {
        printf("Test convUtf8p32: FAILED\n");
    }

    fclose(input);
    fclose(output);
}

void test_convUtf32p8() {
    FILE *input = fopen("output_utf32_le.bin", "rb");
    FILE *output = fopen("output_utf8.txt", "wb");

    if (input == NULL || output == NULL) {
        fprintf(stderr, "Erro ao abrir arquivos de teste para convUtf32p8\n");
        exit(EXIT_FAILURE);
    }

    int result = convUtf32p8(input, output);

    if (result == 0) {
        total_passed++;
        printf("Test convUtf32p8: PASSED\n");
    } else {
        printf("Test convUtf32p8: FAILED\n");
    }

    fclose(input);
    fclose(output);
}

int main() {
    printf("Iniciando testes...\n");

    test_convUtf8p32();
    test_convUtf32p8();

    printf("Passou em %d casos.\n", total_passed);

    return 0;
}