Algumas notas:

UTF-8 eh um armazenamento de tamanho de bytes variavel, por isso eh tao interessante e tao usado.

Existem, basicamente, 4 casos:

byte1 < 0x0080 --> Nesse apenas consideramos como um ASCII normal.


Caso voce nao possua um leitor de UTF-32, como eh o meu caso, sugiro rodar esse script Python:

#Codigo Python

file_path = 'saida.bin'

# Open the file in binary mode
with open(file_path, 'rb') as f:
# Read the binary content

    binary_content = f.read()

# Decode the binary content using UTF-32
decoded_content = binary_content.decode('utf-32')

# Print the decoded content
print(decoded_content)

# End

Nenhum editor de texto que eu tenho instalado no meu computador tem suporte para UTF-32, se tem, nao sei usar. 

Ao rodar esse codigo, no terminal, ao menos no Windows 11, eh possivel ver o conteudo do UTF-32. Assim, pode-se confirmar o sucesso da funcao.