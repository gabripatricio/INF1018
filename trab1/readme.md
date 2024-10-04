# Leitor de Arquivos UTF-32

Este documento descreve como usar o script `leitor.py` para ler arquivos codificados em UTF-32. Como você pode saber, UTF-8 é um armazenamento de tamanho de bytes variável, o que o torna amplamente utilizado.

## Considerações sobre UTF-8

O UTF-8 possui diferentes casos de codificação:
- **Caso 1:** Se o byte é menor que `0x0080`, ele é considerado um caractere ASCII normal.

## Problemas com UTF-32

Caso você não possua um leitor de UTF-32, como é o meu caso, é recomendável usar o script Python abaixo. Ele permite que você visualize o conteúdo do arquivo UTF-32 diretamente no terminal.

### Código do Script

```python
# leitor.py

def ler_utf32(caminho_arquivo):
    with open(caminho_arquivo, 'rb') as arquivo:
        conteudo = arquivo.read()
    # Decodifica o conteúdo para UTF-32
    texto = conteudo.decode('utf-32')
    return texto

if __name__ == "__main__":
    caminho = input("Digite o caminho do arquivo UTF-32: ")
    try:
        resultado = ler_utf32(caminho)
        print("Conteúdo do arquivo:")
        print(resultado)
    except Exception as e:
        print(f"Ocorreu um erro: {e}")
