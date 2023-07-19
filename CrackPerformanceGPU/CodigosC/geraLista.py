import itertools
import string

combinations = []

# Define os caracteres a serem utilizados nas combinações
characters = string.ascii_letters + string.digits + string.punctuation

# Gera as combinações
for length in range(1, 6):  # Gera combinações de tamanho 1 até 5
    for combination in itertools.product(characters, repeat=length):
        combinations.append(''.join(combination))

# Salva as combinações em um arquivo de texto
with open('passwords.txt', 'w') as file:
    for combination in combinations[:10000]:
        file.write(combination + '\n')

print("Arquivo 'passwords.txt' salvo com sucesso!")

