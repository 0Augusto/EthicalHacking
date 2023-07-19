#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_PASSWORD_LENGTH 100

int main() {
    char dictionary[MAX_PASSWORD_LENGTH][MAX_PASSWORD_LENGTH];
    int dictionarySize = 0;

    // Read passwords from a text file
    FILE* inputFile = fopen("passwords.txt", "r");
    if (inputFile == NULL) {
        printf("Failed to open the input file.\n");
        return 1;
    }

    char password[MAX_PASSWORD_LENGTH];
    while (fgets(password, MAX_PASSWORD_LENGTH, inputFile) != NULL) {
        int len = strlen(password);
        if (len > 0 && password[len - 1] == '\n') {
            password[len - 1] = '\0'; // Remove the newline character
        }
        strcpy(dictionary[dictionarySize++], password);
    }
    fclose(inputFile);

    char target[] = "target_password";

    #pragma omp parallel for num_threads(8) schedule(dynamic, 2)
    for (int i = 0; i < dictionarySize; i++) {
        if (strcmp(target, dictionary[i]) == 0) {
            #pragma omp critical
            {
                printf("Password found: %s\n", dictionary[i]);
                printf("Number of i is: %d\n", i);
                printf("Number of thread is: %d\n", omp_get_thread_num());
                printf("Total number of passwords is: %d\n", dictionarySize);
            }
        }
    }

    return 0;
}

