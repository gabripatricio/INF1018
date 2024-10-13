#include <stdio.h>
#include <stdlib.h>
#include "converteutf832.h"

/*

Alunos:

Gabriel Patricio de Oliveira: 2310806 3WA
Joao Vitor Mallet Malheiros: 2310604 3WA

*/

#define TRUE 1
#define FALSE 0

enum qtdBytes {
	ERRO = -1,
	UM_BYTE = 1,
	DOIS_BYTES = 2,
	TRES_BYTES = 3,
	QUATRO_BYTES = 4,
};


int classificaCasoUTF8(unsigned char byte)
{
	if (byte < 0x0080)
	{
		return 1;
	}
	else if ((byte >> 5) == 0b110)
	{
		return 2;
	}
	else if ((byte >> 4) == 0b1110)
	{
		return 3;
	}
	else if ((byte >> 3) == 0b11110)
	{
		return 4;
	}
	else
	{
		return -1;
	}
}

int classificaCasoUTF32(unsigned int numero)
{
	if (numero <= 0x007F)
	{
		return 1;
	}
	else if (numero <= 0x07FF)
	{
		return 2;
	}
	else if (numero <= 0xFFFF)
	{
		return 3;
	}
	else if (numero <= 0x10FFFF)
	{
		return 4;
	}
	else
	{
		return -1;
	}
}

int convUtf8p32(FILE* arquivo_entrada, FILE* arquivo_saida)
{
	unsigned char byte1 = 0, byte2 = 0, byte3 = 0, byte4 = 0;

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

		int caso = classificaCasoUTF8(byte1);

		switch (caso)
		{
		case UM_BYTE:
			// (ASCII)
			utf32 |= byte1;
			break;

		case DOIS_BYTES:
			// caso 2 (tem que ser 5 pois estamos considerando 110xxxxx como primeiro byte)
			if (fread(&byte2, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.\n");
				return -1;
			}

			utf32 |= (byte1 & 0x1F) << 6;  // 0x1F = 0b00011111 --> Os 5 bits uteis do byte1
			utf32 |= (byte2 & 0x3F);      // 0x3F = 0b00111111 Os 6 bits uteis do byte2
			break;

		case TRES_BYTES:
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
			break;

		case QUATRO_BYTES:
			// aqui tem que dar 3 freads
			if (fread(&byte2, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.c4\n");
				return -1;
			}

			if (fread(&byte3, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.c4\n");
				return -1;
			}

			if (fread(&byte4, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.c4\n");
				return -1;
			}
			//fazendo as operacoes de mascara
			utf32 = (byte1 & 0x07) << 18; // 3 bits de byte1
			utf32 |= (byte2 & 0x3F) << 12; // 6 bits de byte2
			utf32 |= (byte3 & 0x3F) << 6;  // 6 bits de byte3
			utf32 |= (byte4 & 0x3F);       // 6 bits de byte4
			break;

		case ERRO:
			printf("Arquvio mal formatado.\n");
			return -1;
			break;
		}
		// escrever no final
		fwrite(&utf32, sizeof(unsigned int), 1, arquivo_saida);

		//apenas exibindo o bytes para ver o que esta acontencendo... (mini dump)
		unsigned char* utf32Bytes = (unsigned char*)&utf32;
		printf("Bytes: %02X %02X %02X %02X\n", utf32Bytes[0], utf32Bytes[1], utf32Bytes[2], utf32Bytes[3]);
	}
	return 0; //sucesso
}

unsigned int trocaOrdem(unsigned int num)
{
	unsigned int byte1, byte2, byte3, byte4;

	byte1 = 0x000000FF;
	byte2 = 0x0000FF00;
	byte3 = 0x00FF0000;
	byte4 = 0xFF000000;

	byte1 = byte1 & num;
	byte2 = byte2 & num;
	byte3 = byte3 & num;
	byte4 = byte4 & num;

	unsigned int resp = 0;

	resp |= (byte1 << 24);
	resp |= (byte2 << 8);
	resp |= (byte3 >> 8);
	resp |= (byte4 >> 24);

	return resp;
}

int convUtf32p8(FILE* input, FILE* output)
{
	int littleEndian = FALSE, bigEndian = FALSE;
	unsigned int utf32, bom;

	fread(&bom, sizeof(unsigned int), 1, input);
	printf("BOM: %02x\n", bom);

	if (bom == 0x0000FEFF)
	{
		printf("BOM: LE\n");
		littleEndian = TRUE;
	}
	else if (bom == 0xFFFE0000)
	{
		printf("BOM: BE\n");
		bigEndian = TRUE;
	}
	else
	{
		printf("BOM invalido ou ausente.\n");
		return -1;
	}

	while (fread(&utf32, sizeof(unsigned int), 1, input) == 1)
	{
		unsigned char b1, b2, b3, b4; // byte1..4 (so pra ser mais facil de escrever)
		if (bigEndian)
		{
			utf32 = trocaOrdem(utf32);
		}

		int caso = classificaCasoUTF32(utf32);

		switch (caso)
		{
		case UM_BYTE:
			b1 = utf32;
			fwrite(&b1, sizeof(unsigned char), 1, output);
			printf("Escrevendo 1 byte: %02x\n", b1);//mensagem de debug
			break;

		case DOIS_BYTES:
			b1 = 0b11000000 | ((utf32 >> 6) & 0x1F); //
			b2 = 0b10000000 | (utf32 & 0x3F);
			fwrite(&b1, sizeof(unsigned char), 1, output);
			fwrite(&b2, sizeof(unsigned char), 1, output);

			printf("Escrevendo 2 bytes: %02x %02x\n", b1, b2); //mensagem de debug 
			break;

		case TRES_BYTES:
			b1 = 0xE0 | ((utf32 >> 12) & 0x0F);
			b2 = 0x80 | ((utf32 >> 6) & 0x3F);
			b3 = 0x80 | (utf32 & 0x3F);
			fwrite(&b1, sizeof(unsigned char), 1, output);
			fwrite(&b2, sizeof(unsigned char), 1, output);
			fwrite(&b3, sizeof(unsigned char), 1, output);

			printf("Escrevendo 3 bytes: %02x %02x %02x\n", b1, b2, b3); //mensagem de debug
			break;

		case QUATRO_BYTES:
			b1 = 0b11110000 | ((utf32 >> 18) & 0x07);
			b2 = 0b10000000 | ((utf32 >> 12) & 0x3F);
			b3 = 0b10000000 | ((utf32 >> 6) & 0x3F);
			b4 = 0b10000000 | (utf32 & 0x3F);
			fwrite(&b1, sizeof(unsigned char), 1, output);
			fwrite(&b2, sizeof(unsigned char), 1, output);
			fwrite(&b3, sizeof(unsigned char), 1, output);
			fwrite(&b4, sizeof(unsigned char), 1, output);
			printf("Escrevendo 4 bytes: %02x %02x %02x %02x\n", b1, b2, b3, b4); //mensagem de debug
			break;

		case ERRO:
			printf("Arquivo mal formatado.\n");
			return -1;
			break;
		}
	}
	return 0; //sucesso
}
