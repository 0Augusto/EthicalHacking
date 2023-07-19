#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PASSWORD_LENGTH 16

// Função para gerar uma senha aleatória com base em uma máscara
void generatePasswordWithMask(char *mask, char *password) {
    int i = 0;
    while (mask[i] != '\0') {
        switch (mask[i]) {
            case 'L': // Letra minúscula
                password[i] = 'a' + (rand() % 26);
                break;
            case 'U': // Letra maiúscula
                password[i] = 'A' + (rand() % 26);
                break;
            case 'D': // Dígito
                password[i] = '0' + (rand() % 10);
                break;
            case 'S': // Símbolo
                password[i] = '!' + (rand() % 15);
                break;
            default: // Outros caracteres
                password[i] = mask[i];
                break;
        }
        i++;
    }
    password[i] = '\0';
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com base no tempo atual

    char mask[] = "LDDD-SS-LLDU"; // Exemplo de máscara para a senha (L: Letra minúscula, U: Letra maiúscula, D: Dígito, S: Símbolo)
    char password[MAX_PASSWORD_LENGTH + 1];

    generatePasswordWithMask(mask, password);

    printf("Senha gerada: %s\n", password);

    return 0;
}

