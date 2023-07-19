#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 5
#define MAX_COMBINATIONS 100000

void generateCombinations(char* characters, int length, FILE* file) {
    int* indices = (int*)malloc(length * sizeof(int));
    memset(indices, 0, length * sizeof(int));

    while (1) {
        char combination[MAX_LENGTH + 1];
        combination[length] = '\0';

        for (int i = 0; i < length; i++) {
            combination[i] = characters[indices[i]];
        }

        fprintf(file, "%s\n", combination);

        int incrementIndex = length - 1;
        while (incrementIndex >= 0 && ++indices[incrementIndex] == strlen(characters)) {
            indices[incrementIndex] = 0;
            incrementIndex--;
        }

        if (incrementIndex < 0) {
            break;
        }
    }

    free(indices);
}

int main() {
    char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+";
    int totalLength = strlen(characters);

    FILE* file = fopen("combinacoes.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    for (int length = 1; length <= MAX_LENGTH; length++) {
        generateCombinations(characters, length, file);
    }

    fclose(file);
    printf("Arquivo 'combinacoes.txt' salvo com sucesso!\n");

    return 0;
}


