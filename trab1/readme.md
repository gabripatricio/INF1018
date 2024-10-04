# Arquivos UTF-32


## Considerações sobre UTF-8

O UTF-8 possui diferentes casos de codificação:
- **Caso 1:** Se o byte é menor que `0x0080`, ele é considerado um caractere ASCII normal.

## Problemas com interpretadores de UTF-32

Caso você não possua um leitor de UTF-32, como é o meu caso, é recomendável usar o script Python abaixo. Ele permite que você visualize o conteúdo do arquivo UTF-32 diretamente no terminal.

### leitor.py

```python


file_path = 'saida.bin'

# Open the file in binary mode
with open(file_path, 'rb') as f:
# Read the binary content

    binary_content = f.read()

# Decode the binary content using UTF-32
decoded_content = binary_content.decode('utf-32')

# Print the decoded content
print(decoded_content)
