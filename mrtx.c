#include "matrix.h"
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>

typedef double element_t;

struct matrix {
    size_t MAKS;
    size_t m;
    size_t n;
    element_t *mat;
};

size_t matrix_m(struct matrix *a) {
    return a->m;
}

size_t matrix_n(struct matrix *a) {
    return a->n;
}

struct matrix *alloc(size_t M, size_t N) {
    struct matrix *a = malloc(sizeof(struct matrix));
    if (!a) return NULL;
    size_t x = M * N;
    if (x < 10) {
        x = 10;
    }
    a->MAKS = x;
    a->mat = malloc(x * sizeof(element_t));
    if (!a->mat) {
        free(a);
        return NULL;
    }
    a->m = M;
    a->n = N;

    for (size_t i = 0; i < x; i++) {
        a->mat[i] = 0;
    }
    return a;
}

struct matrix *null_alloc(size_t M, size_t N) {
    struct matrix *a = alloc(M, N);
    if (!a) return NULL;
    for (size_t i = 0; i < M * N; i++) {
        a->mat[i] = 0;
    }
    return a;
}

struct matrix *ed_d_alloc(size_t M, size_t N) {
    struct matrix *a = alloc(M, N);
    if (!a) return NULL;
    for (size_t i = 0; i < M * N; i++) {
        size_t x = i / N;
        size_t y = i - x * N;
        if (x != y) {
            a->mat[i] = 0;
        } else {
            a->mat[i] = 1;
        }
    }
    return a;
}

void change(struct matrix *a, size_t i, size_t j, element_t x) {
    size_t N = a->n;
    a->mat[i * N + j] = x;
}

element_t poind(struct matrix *a, size_t i, size_t j) {
    size_t N = a->n;
    return a->mat[i * N + j];
}

struct matrix *matr_alloc(struct matrix *a, size_t M, size_t N) {
    if (M * N > a->MAKS) {
        size_t x = M * N;
        if (x < 2 * a->MAKS) {
            x = 2 * a->MAKS;
        }
        element_t *new_mat = realloc(a->mat, x * sizeof(element_t));
        if (!new_mat) return NULL;
        a->mat = new_mat;
        a->MAKS = x;
    }
    a->m = M;
    a->n = N;
    return a;
}

void matrix_free(struct matrix *a) {
    if (a) {
        free(a->mat);
        free(a);
    }
}




