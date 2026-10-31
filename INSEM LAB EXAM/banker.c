
#include <stdio.h>

#define P 5
#define R 3

int allocation[P][R] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

int maximum[P][R] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

int available[R] = {3, 3, 2};
int need[P][R];
int sequence[P];
int used[P] = {0};
int safeCount = 0, unsafeCount = 0;

void printSequence() {
    int i;

    for (i = 0; i < P; i++) {
        printf("P%d", sequence[i]);
        if (i < P - 1)
            printf(" -> ");
    }
}

void checkSequence() {
    int work[R], i, j, k;

    for (j = 0; j < R; j++)
        work[j] = available[j];

    for (i = 0; i < P; i++) {
        int p = sequence[i];

        for (j = 0; j < R; j++) {
            if (need[p][j] > work[j]) {
                unsafeCount++;
                printf("Unsafe ordering %d: ", unsafeCount);
                printSequence();
                printf(" [stuck at P%d]\n", p);
                return;
            }
        }

        for (k = 0; k < R; k++)
            work[k] += allocation[p][k];
    }

    safeCount++;
    printf("Safe sequence %d: ", safeCount);
    printSequence();
    printf("\n");
}

void generate(int level) {
    int i;

    if (level == P) {
        checkSequence();
        return;
    }

    for (i = 0; i < P; i++) {
        if (!used[i]) {
            used[i] = 1;
            sequence[level] = i;

            generate(level + 1);

            used[i] = 0;
        }
    }
}

int main() {
    int i, j;

    printf("=== Banker's Algorithm ===\n");

    for (i = 0; i < P; i++) {
        for (j = 0; j < R; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
    }

    printf("\nNeed Matrix:\n");
    for (i = 0; i < P; i++) {
        for (j = 0; j < R; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    printf("\nChecking all 120 process orderings:\n\n");
    generate(0);

    printf("\n=== Final Result ===\n");
    printf("Safe sequences: %d\n", safeCount);
    printf("Unsafe orderings: %d\n", unsafeCount);

    if (safeCount > 0)
        printf("System is in a SAFE state.\n");
    else
        printf("System is in an UNSAFE state.\n");

    return 0;
}

