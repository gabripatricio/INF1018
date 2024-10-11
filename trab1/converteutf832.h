#define CONVERTEUTF832_H

#include <stdio.h>

// Função que converte de UTF-8 para UTF-32 (little-endian)
int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida);

// Função que converte de UTF-32 (little-endian ou big-endian) para UTF-8
int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida);

