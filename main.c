#include <stdio.h>
#include <stdlib.h>
#include "mrtx.h"
#include "matrix.h"

void check_and_free(struct matrix *a, struct matrix *b, struct matrix *c) {
    if (a) matrix_free(a);
    if (b) matrix_free(b);
    if (c) matrix_free(c);
}

int main() {
    struct matrix *a = NULL;
    struct matrix *b = NULL;
    struct matrix *c = NULL;
    struct matrix *A = NULL;
    int n, mk;

    printf("Enter the size of matrix:\n");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    a = null_alloc(n, n);
    if (!a) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter matrix a:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double x;
            if (scanf("%lf", &x) != 1) {
                printf("Invalid input\n");
                check_and_free(a, b, c);
                return 1;
            }
            change(a, i, j, x);
        }
    }

    printf("Write eps:\n");
    double y;
    if (scanf("%lf", &y) != 1) {
        printf("Invalid input\n");
        check_and_free(a, b, c);
        return 1;
    }

    b = m_exp(a, y);
    if (!b) {
        printf("Memory allocation failed\n");
        matrix_free(a);
        return 1;
    }

    printf("Resulting matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double x = poind(b, i, j);
            printf("%lf ", x);
        }
        printf("\n");
    }
    matrix_free(b);

    printf("Write the sizes:\n");
    if (scanf("%d%d", &mk, &n) != 2) {
        printf("Invalid input\n");
        matrix_free(a);
        return 1;
    }

    matrix_free(a);
    a = alloc(mk, n);
    if (!a) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Write a:\n");
    for (int i = 0; i < mk; i++) {
        for (int j = 0; j < n; j++) {
            double x;
            if (scanf("%lf", &x) != 1) {
                printf("Invalid input\n");
                matrix_free(a);
                return 1;
            }
            change(a, i, j, x);
        }
    }

    a = make_tr_m(a);
    if (!a) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Transposed matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < mk; j++) {
            double x = poind(a, i, j);
            printf("%lf ", x);
        }
        printf("\n");
    }
    matrix_free(a);

    printf("Write size:\n");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    a = alloc(n, n + 1);
    if (!a) {
        printf("Memory allocation failed\n");
        return 1;
    }

    A = alloc(n, n);
    if (!A) {
        printf("Memory allocation failed\n");
        matrix_free(a);
        return 1;
    }

    printf("Enter the matrix with augmented column:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            double x;
            if (scanf("%lf", &x) != 1) {
                printf("Invalid input\n");
                check_and_free(a, b, A);
                return 1;
            }
            change(a, i, j, x);
            if (j != n) {
                change(A, i, j, x);
            }
        }
    }

    b = gauss(a);
    if (!b) {
        printf("Memory allocation failed\n");
        check_and_free(a, A, NULL);
        return 1;
    }

    double X = poind(b, 0, 0);
    if (X != X) { // Проверка на NaN
        printf("No solutions\n");
    } else {
        printf("Solution:\n");
        for (int j = 0; j < n; j++) {
            double x = poind(b, j, 0);
            printf("%lf ", x);
        }
        printf("\n");
    }

    c = alloc(n, 1);
    if (!c) {
        printf("Memory allocation failed\n");
        check_and_free(a, b, A);
        return 1;
    }

    c = matrix_multiplie(A, b, c);
    if (!c) {
        printf("Memory allocation failed\n");
        check_and_free(a, b, A);
        return 1;
    }

    matrix_free(A);
    matrix_free(c);
    matrix_free(a);
    matrix_free(b);

    return 0;
}
