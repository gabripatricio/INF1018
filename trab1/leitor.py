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
