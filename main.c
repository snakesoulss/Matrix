#include <stdio.h>
#include <stdlib.h>
#include "mrtx.h"
#include "matrix.h"

int main() {
    struct matrix *a;
    struct matrix *b;
    int n, mk;
    printf("Enter the size of matrix:\n");
    scanf("%d", &n);
    a = null_alloc(n, n);
    printf("Enter matrix a:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double x;
            scanf("%lf", &x);
            change(a, i, j, x);
        }
    }
    printf("Write eps:\n");
    double y;
    scanf("%lf", &y);
    b = m_exp(a, y);
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
    scanf("%d%d", &mk, &n);
    matrix_free(a);
    a = alloc(mk, n);
    printf("Write a:\n");
    for (int i = 0; i < mk; i++) {
        for (int j = 0; j < n; j++) {
            double x;
            scanf("%lf", &x);
            change(a, i, j, x);
        }
    }
    a = make_tr_m(a);
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
    scanf("%d", &n);
    a = alloc(n, n + 1);
    struct matrix *A = alloc(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            double x;
            scanf("%lf", &x);
            change(a, i, j, x);
            if (j != n) {
                change(A, i, j, x);
            }
        }
    }
    b = gauss(a);
    double X = poind(b, 0, 0);
    if (X != X) {
        printf("No solutions\n");
    } else {
        printf("Solution:\n");
        for (int j = 0; j < n; j++) {
            double x = poind(b, j, 0);
            printf("%lf ", x);
        }
        printf("\n");
    }
    struct matrix *c = alloc(n, 1);
    c = matrix_multiplie(A, b, c);
    printf("Resulting vector:\n");
    for (int j = 0; j < n; j++) {
        double x = poind(c, j, 0);
        printf("%lf ", x);
    }
    matrix_free(A);
    matrix_free(c);
    matrix_free(a);
    matrix_free(b);
    return 0;
}
