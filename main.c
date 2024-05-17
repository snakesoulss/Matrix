#include <stdio.h>
#include <stdlib.h>
#include "mrtx.h"
#include "matrix.h"

int main() {

    matrix *a;
    matrix *b;

    printf("write size \n");
    int n;
    scanf("%d", &n);
    a = null_alloc(n, n);
    printf("Write a \n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            double x;
            scanf("%lf", &x);
            change(a, i, j, x);
        }
    }
    printf("Write eps\n");
    double y;
    scanf("%lf", &y);
    b = m_exp(a, y);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            double x = poind(b, i, j);
            printf("%lf ", x);
        }
        printf("\n");
    }
    matrix_free(b);
    printf("Write a sizes \n");
    int mk;
    scanf("%d%d", &mk, &n);
    matrix_free(a);
    a = alloc(mk, n);
    printf("Write a\n");
    for(int i = 0; i < mk; i++) {
        for(int j = 0; j < n; j++) {
            double x;
            scanf("%lf", &x);
            change(a, i, j, x);
        }
    }
    a = make_tr_m(a);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < mk; j++) {
            double x = poind(a, i, j);
            printf("%lf ", x);
        }
        printf("\n");
    }
    matrix_free(a);
    printf("Write size \n");
    scanf("%d", &n);
    a = alloc(n, n+1);
    matrix *A = alloc(n, n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j <= n; j++) {
            double x;
            scanf("%lf", &x);
            change(a, i, j, x);
            if(j != n) {
                change(A, i, j, x);
            }
        }
    }
  b = gauss(a);
    double X = poind(b, 0, 0);
    if(X != X) {
        printf("no solutions\n");    }
    else {
        for(int j = 0; j < n; j++) {
            double x = poind(b, j, 0);
            printf("%lf ", x);
        }
        printf("\n");
    }
    matrix *c = alloc(n, 1);
    c = matrix_multiplie(A, b, c);
    for(int j = 0; j < n; j++) {
        double x = poind(c, j, 0);
        printf("%lf ", x);
    }
    matrix_free(A);
    matrix_free(c);
    matrix_free(a);
    matrix_free(b);
    return 0;
}
