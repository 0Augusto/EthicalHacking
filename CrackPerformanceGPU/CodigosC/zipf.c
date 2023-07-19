/*
A ordem de complexidade do código fornecido pode ser analisada em relação às duas principais funções: `generalizedHarmonic()` e `generateZipfSequence()`.

A função `generalizedHarmonic()` contém um loop `for` que executa `n` iterações. Portanto, sua complexidade é O(n).

A função `generateZipfSequence()` também possui um loop `for` que executa `length` iterações. Dentro desse loop, há um loop `while` que, em média, é executado várias vezes proporcionalmente à probabilidade cumulativa. A quantidade exata de iterações do loop `while` pode variar dependendo dos valores gerados aleatoriamente e da distribuição Zipf. No entanto, a complexidade média esperada desse loop `while` é O(1), pois o número de iterações geralmente não cresce linearmente com o tamanho dos dados. Portanto, podemos considerar a complexidade de `generateZipfSequence()` como O(length).

Dado que a função `generateZipfSequence()` é chamada no programa principal com `sequenceLength` igual a 10, a ordem de complexidade total do código é O(length) = O(10) = O(1). Isso significa que a complexidade do código não aumenta à medida que o tamanho da sequência a ser gerada aumenta.

*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 100 // Número total de elementos
#define S 1.0 // Parâmetro de inclinação da distribuição Zipf

// Função para calcular a função harmônica generalizada H(N, s)
double generalizedHarmonic(int n, double s) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / pow(i, s);
    }
    return sum;
}

// Função para gerar uma sequência de elementos com base na distribuição Zipf
void generateZipfSequence(int *sequence, int length) {
    double harmonic = generalizedHarmonic(N, S); // Calcula a função harmônica geral com base em N e S
    for (int i = 0; i < length; i++) {
        double randomValue = (double)rand() / RAND_MAX; // Gera um valor aleatório entre 0 e 1
        double cumulativeProbability = 0.0;
        int element = 1;
        while (cumulativeProbability < randomValue) {
            cumulativeProbability += (1.0 / (pow(element, S) * harmonic)); // Calcula a probabilidade cumulativa para o elemento atual
            element++;
        }
        sequence[i] = element - 1; // Armazena o elemento gerado na sequência (subtraindo 1 para ajustar à base 0)
    }
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com base no tempo atual

    int sequenceLength = 10; // Comprimento da sequência a ser gerada
    int sequence[sequenceLength];

    generateZipfSequence(sequence, sequenceLength); // Gera a sequência de elementos com base na distribuição Zipf

    printf("Sequência gerada: ");
    for (int i = 0; i < sequenceLength; i++) {
        printf("%d ", sequence[i]); // Imprime os elementos da sequência gerada
    }
    printf("\n");

    return 0;
}

