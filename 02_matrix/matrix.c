#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ERROR_MSG_INPUT "Hiba a bemenet olvasásakor! (ENTER - újrapróbálkozás, CTRL + C - kilépés)\n"
#define ERROR_MSG_NUM "Érvényes, pozitív számot adjon meg!\n"
#define MATRIX_MULTI_PROMPT "\nA két összeszorzandó mátrix:\n"
#define STACK_OVERFLOW "Stack overflow! Nem lehet több mátrixot menteni.\n"
#define STACK_UNDERFLOW "Stack underflow! Nincs több mentett mátrix.\n"

void get_positive_int(const char *prompt, int *value) {
    printf("%s", prompt);
    while (scanf("%d", value) != 1 || *value <= 0) {
        perror(ERROR_MSG_NUM);
        while (getchar() != '\n');
        printf("%s", prompt);
    }
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void get_matrix(const char *prompt, int n, int matrix[n][n]) {
    printf("%s", prompt);
    char line[256];

    for (int i = 0; i < n; i++) {
        
        printf("%d. sor: ", i + 1);
        
        if (!fgets(line, sizeof(line), stdin)) {
            perror(ERROR_MSG_INPUT);
            exit(1);
        }

        char *token = strtok(line, " \n");
        for (int j = 0; j < n; j++) {
            if (token == NULL) {
                perror(ERROR_MSG_INPUT);
                clear_input_buffer();
                get_matrix(prompt, n, matrix); // Helytelen input esetén újra megadható
                return;
            }
            matrix[i][j] = atoi(token);
            token = strtok(NULL, " \n");
        }
    }
}

void print_matrix(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        printf("|");
        for (int j = 0; j < n; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

void init_zero_matrix(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}

void init_identity_matrix(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void scalar_multiply(int n, int matrix[n][n], int scalar) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] *= scalar;
        }
    }
}

void multiply_matrices(int n, int res[n][n], int m1[n][n], int m2[n][n]) {
    printf(MATRIX_MULTI_PROMPT);
    print_matrix(n, m1);
    print_matrix(n, m2);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

void transform_point(HomPoint *point, int n, int t_matrix[n][n]) {
    float x = point->x;
    float y = point->y;
    float w = point->weight;

    point->x = t_matrix[0][0] * x + t_matrix[0][1] * y + t_matrix[0][2] * w;
    point->y = t_matrix[1][0] * x + t_matrix[1][1] * y + t_matrix[1][2] * w;
}

void scale(int n, int matrix[n][n], int multip) {
    for (int i = 0; i < n - 1; i++) {
        matrix[i][i] *= multip;
    }
}

void shift(int n, int matrix[n][n], int x, int y) {
    int translation[3][3] = {
        {1, 0, x},
        {0, 1, y},
        {0, 0, 1}
    };

    int result[3][3] = {0};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] += matrix[i][k] * translation[k][j];
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = result[i][j];
        }
    }
}

void rotate(int n, int matrix[n][n], int rotate_deg) {
    float rad = rotate_deg * M_PI / 180.0; // Az itteni M_PI hibaüzenet (ha megjelenik) fals pozitív
    float cosA = cos(rad);
    float sinA = sin(rad);

    float rotation[3][3] = {
        {cosA, -sinA, 0},
        {sinA,  cosA, 0},
        {   0,     0, 1}
    };

    float result[3][3] = {0};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result[i][j] += rotation[i][k] * matrix[k][j];
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrix[i][j] = result[i][j];
        }
    }
}

int stack[MAX_STACK][N][N];
int stack_pointer = -1;

void push_matrix(int n, int matrix[n][n]) {
    if (stack_pointer < MAX_STACK - 1) {
        stack_pointer++;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                stack[stack_pointer][i][j] = matrix[i][j];
            }
        }
        return;
    }
    
    perror(STACK_OVERFLOW);
}

void pop_matrix(int n, int matrix[n][n]) {
    if (stack_pointer >= 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matrix[i][j] = stack[stack_pointer][i][j];
            }
        }
        stack_pointer--;
        return;
    }
    
    perror(STACK_UNDERFLOW);
}