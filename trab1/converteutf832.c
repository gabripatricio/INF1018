#include <stdio.h>
#include <stdlib.h>

// algumas informacoes importantes que nos vamos precisar:

/*

Código UNICODE Representação UTF-8 (byte a byte)
U+0000 a U+007F (0 a 127) 0xxxxxxx
U+0080 a U+07FF (128 a 2047) 110xxxxx 10xxxxxx
U+0800 a U+FFFF (2048 a 65535) 1110xxxx 10xxxxxx 10xxxxxx
U+10000 a U+10FFFF (65536 a 1114111) 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx


O símbolo © tem código UNICODE U+00A9.
Em binário A9 é 1010 1001. Usando a codificação de 2 bytes para a faixa U+0080 a U+07FF temos:
11000010 10101001 = 0xC2 0xA9

O primeiro byte começa com 110, indicando que a sequência é composta por dois bytes. A seguir vêm os cinco primeiros bits do código UNICODE (note o preenchimento com zeros à esquerda para completar a porção do código do caractere colocada no primeiro byte da sequência).
O segundo byte começa com 10, indicando que é um byte de continuação. A seguir vêm os próximos seis bits do código UNICODE.

*/

int convUtf8p32(FILE* arquivo_entrada, FILE* arquivo_saida)
{
	unsigned char byte1, byte2, byte3, byte4;

	if (arquivo_entrada == NULL || arquivo_saida == NULL)
	{
		fprintf(stderr, "Erro de abertura do arquivo\n");
		return -1;
	}

	unsigned int bom = 0x0000FEFF; // considerando arquiterura little endian
	fwrite(&bom, sizeof(unsigned int), 1, arquivo_saida);


	while (fread(&byte1, sizeof(unsigned char), 1, arquivo_entrada) == 1)
	{
		unsigned int utf32 = 0; //esse que vamos escrever

		if (byte1 < 0x0080) // caso 1 (ASCII)
		{
			utf32 |= byte1;
			// ok, funciona
		}

		else if ((byte1 >> 5) == 0b110) // caso 2 (tem que ser 5 pois estamos considerando 110xxxxx como primeiro byte)
		{

			if (fread(&byte2, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.\n");
				return -1;
			}

			utf32 |= (byte1 & 0x1F) << 6;  // 0x1F = 0b00011111 --> Os 5 bits uteis do byte1
			utf32 |= (byte2 & 0x3F);      // 0x3F = 0b00111111 Os 6 bits uteis do byte2

		}

		else if ((byte1 >> 4) == 0b1110) //caso 3
		{
			// aqui tem que dar 2 freads
			if (fread(&byte2, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo. 67\n");
				return -1;
			}

			if (fread(&byte3, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo. 73\n");
				return -1;
			}
			//fazendo as operacoes de mascara
			utf32 |= (byte1 & 0x0F) << 12; // 0b00011111
			utf32 |= (byte2 & 0x3F) << 6;
			utf32 |= (byte3 & 0x3F);
		}

		else if (byte1 >> 3 == 0b11110) // caso 4
		{
			// aqui tem que dar 3 freads
			if (fread(&byte2, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.87\n");
				return -1;
			}

			if (fread(&byte3, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.93\n");
				return -1;
			}

			if (fread(&byte4, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.99\n");
				return -1;
			}
			//fazendo as operacoes de mascara
			utf32 = (byte1 & 0x07) << 18; // 3 bits de byte1
			utf32 |= (byte2 & 0x3F) << 12; // 6 bits de byte2
			utf32 |= (byte3 & 0x3F) << 6;  // 6 bits de byte3
			utf32 |= (byte4 & 0x3F);       // 6 bits de byte4
		}

		else
		{
			printf("Arquvio mal formatado.\n");
			return -1;
		}

		// escrever no final
		fwrite(&utf32, sizeof(unsigned int), 1, arquivo_saida);
		printf("%d\n", byte1);
	}

	return 0; //sucesso
}

int convUtf32p8(FILE* input, FILE* output) {
    // Implementação futura
    return -1;
}
