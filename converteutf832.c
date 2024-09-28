#include <stdio.h>
#include <stdlib.h>

// algumas informacoes importantes que nos vamos precisar:

/*

Código UNICODE	                           Representação UTF-8 (byte a byte)
U+0000 a U+007F (0 a 127)	               0xxxxxxx
U+0080 a U+07FF	(128 a 2047)               110xxxxx 10xxxxxx 
U+0800 a U+FFFF	(2048 a 65535)             1110xxxx 10xxxxxx 10xxxxxx
U+10000 a U+10FFFF (65536 a 1114111)       11110xxx 10xxxxxx 10xxxxxx 10xxxxxx


O símbolo © tem código UNICODE U+00A9.
Em binário A9 é 1010 1001. Usando a codificação de 2 bytes para a faixa U+0080 a U+07FF temos:
11000010 10101001 = 0xC2 0xA9

O primeiro byte começa com 110, indicando que a sequência é composta por dois bytes. A seguir vêm os cinco primeiros bits do código UNICODE (note o preenchimento com zeros à esquerda para completar a porção do código do caractere colocada no primeiro byte da sequência).
O segundo byte começa com 10, indicando que é um byte de continuação. A seguir vêm os próximos seis bits do código UNICODE.

*/

int convUtf8p32(FILE* arquivo_entrada, FILE* arquivo_saida)
{
	unsigned char byte, lixo;

	if (arquivo_entrada == NULL || arquivo_saida == NULL)
	{
		fprintf(stderr, "Erro de abertura do arquivo\n");
		return -1;
	}

	unsigned int bom = 0xFFFE0000;
    fwrite(&bom, sizeof(unsigned int), 1, arquivo_saida);

	while (fread(&byte, sizeof(unsigned char), 1, arquivo_entrada)  == 1)
	{
		unsigned char utf32 = 0; //apenas para nao dar erro por enquanto, 25/9 7:28pm
		unsigned char utf32_2 = 0;
		unsigned char utf32_3 = 0;
		unsigned char utf32_4 = 0;

		if (byte < 0x0080) // caso 1 (ASCII)
		{
			unsigned int mascara = 0x00;
			utf32 = byte | mascara;
			//parece ok

			fwrite(&utf32, sizeof(unsigned char), 1, arquivo_saida);
		}

		else if (  (byte >> 5) == 0b110  ) // caso 2 (tem que ser 5 pois estamos considerando 110xxxxx como primeiro byte)
		{
			unsigned char byte2;

			if (fread(&byte2, sizeof(unsigned char), 1, arquivo_entrada) != 1)
			{
				printf("Erro na leitura do arquivo.\n");
				return -1;
			}

			//caso a leitura seja bem sucedida, nos temos que escrever no lugar dos x o numero: 110xxxxx 10xxxxxx, mas devemos manter os 1 e 0 no lugar

			unsigned int mascara1 = 0b11100000; //1110 0000
			unsigned int mascara2 = 0b11000000; //1100 0000

			//na mascara 1 vai ficar os 5 primeiros bytes do numero e na mascara 2 os 6 restantes (apenas nesse caso tera a explicacao mais detalhada)
			//como nesse caso, so vai ate 2048, sao apenas 11 posiveis bytes, 2^11 = 2048

			mascara1 = ~mascara1; //para eliminiar os 3 primeiros bits
			mascara2 = ~mascara2; //para eliminar o 2 primeiros bits

			utf32 = mascara1 & byte;
			utf32_2 = mascara2 & byte2;

			mascara1 = 0b11000000;
			mascara2 = 0b10000000;

			utf32 = mascara1 | utf32;
			utf32_2 = mascara2 | utf32_2;

			fwrite(&utf32, sizeof(unsigned char), 1, arquivo_saida);
			fwrite(&utf32_2, sizeof(unsigned char), 1, arquivo_saida);
		}

		else if ( (byte>>4) == 0b1110)
		{
			fwrite(&utf32, sizeof(unsigned char), 1, arquivo_saida); 
			fwrite(&utf32_2, sizeof(unsigned char), 1, arquivo_saida);
			fwrite(&utf32_3, sizeof(unsigned char), 1, arquivo_saida);
		}

		else if ( (byte >>3) == 11110  )
		{
			fwrite(&utf32, sizeof(unsigned char), 1, arquivo_saida); 
			fwrite(&utf32_2, sizeof(unsigned char), 1, arquivo_saida);
			fwrite(&utf32_3, sizeof(unsigned char), 1, arquivo_saida);
			fwrite(&utf32_4, sizeof(unsigned char), 1, arquivo_saida);
		}

		else 
		{
			printf("Arquvio mal formatado.\n");
			return -1;
		}
		printf("%d\n", utf32);
	}

	return 0; //sucesso
}

int convUtf32p8(FILE* input, FILE* output) {
    // Implementação futura
    return -1;
}
