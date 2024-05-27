#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <malloc.h>
#include "mrtx.h"
#include "matrix.h"

struct matrix *summa(struct matrix *a, struct matrix *b) {
    if (!a || !b) return NULL;
    size_t m_a = matrix_m(a);
    size_t m_b = matrix_m(b);
    size_t n_a = matrix_n(a);
    size_t n_b = matrix_n(b);

    if (m_a != m_b || n_a != n_b) return NULL;

    for (size_t i = 0; i < m_a; i++) {
        for (size_t j = 0; j < n_a; j++) {
            element_t x = poind(a, i, j);
            element_t y = poind(b, i, j);
            change(a, i, j, x + y);
        }
    }

    return a;
}

struct matrix *matrix_copy(struct matrix *a, struct matrix *b) {
    if (!a) return NULL;

    size_t m_a = matrix_m(a);
    size_t n_a = matrix_n(a);

    if (!b) {
        b = alloc(m_a, n_a);
    } else {
        b = matr_alloc(b, m_a, n_a);
    }

    if (!b) return NULL;

    for (size_t i = 0; i < m_a; i++) {
        for (size_t j = 0; j < n_a; j++) {
            element_t x = poind(a, i, j);
            change(b, i, j, x);
        }
    }

    return b;
}

struct matrix *sum(struct matrix *a, struct matrix *b, struct matrix *c) {
    c = matrix_copy(a, c);
    c = summa(c, b);
    if (!c) return NULL;
    return c;
}

void e_swap(element_t *a, element_t *b) {
    element_t c = *a;
    *a = *b;
    *b = c;
}

struct matrix *transportirovanie(struct matrix *a, struct matrix *b) {  // Выполняет транспонирование матрицы
    size_t m_a = matrix_m(a);
    size_t n_a = matrix_n(a);
    if (!b) {
        b = alloc(n_a, m_a);
    } else {
        if ((matrix_m(b) != n_a) || (matrix_n(b) != m_a)) {
            b = matr_alloc(b, n_a, m_a);
        }
    }
    if (!b) return NULL;
    for (size_t i = 0; i < m_a; i++) {
        for (size_t j = 0; j < n_a; j++) {
            element_t x = poind(a, i, j);
            change(b, j, i, x);
        }
    }
    return b;
}

struct matrix *make_tr_m(struct matrix *a) {  // Создает транспонированную матрицу и освобождает память под оригинальную
    if (!a) return NULL;

    size_t m_a = matrix_m(a);
    size_t n_a = matrix_n(a);
    struct matrix *b = null_alloc(n_a, m_a);
    b = transportirovanie(a, b);
    if (!b) return NULL;

    struct matrix *c = a;
    a = b;
    matrix_free(c);
    return a;
}

struct matrix *matrix_multiplie(struct matrix *a, struct matrix *b, struct matrix *c) {  // Умножает две матрицы
    size_t m_a = matrix_m(a);
    size_t m_b = matrix_m(b);
    size_t n_a = matrix_n(a);
    size_t n_b = matrix_n(b);
    if (n_a != m_b) {
        return NULL;
    }
    if (!c) {
        c = alloc(m_a, n_b);
    } else {
        c = matr_alloc(c, m_a, n_b);
    }
    if (!c) {
        return NULL;
    }
    for (size_t i = 0; i < m_a; i++) {
        for (size_t j = 0; j < n_b; j++) {
            change(c, i, j, 0);
            for (size_t k = 0; k < n_a; k++) {
                element_t xc = poind(c, i, j);
                element_t xa = poind(a, i, k);
                element_t xb = poind(b, k, j);
                change(c, i, j, xa * xb + xc);
            }
        }
    }
    return c;
}

element_t m_norm(struct matrix *a) {  // Вычисляет норму матрицы
    size_t m_a = matrix_m(a);
    size_t n_a = matrix_n(a);
    element_t norm = 0;
    for (size_t i = 0; i < n_a; i++) {
        element_t curnorm = 0;
        for (size_t j = 0; j < m_a; j++) {
            curnorm += fabs(poind(a, j, i));
        }
        if (curnorm > norm) {
            norm = curnorm;
        }
    }
    return norm;
}

struct matrix *mult_mtrx_on_number(struct matrix *a, element_t b) {  // Умножает матрицу на число
    if (!a) {
        return NULL;
    }
    size_t m_a = matrix_m(a);
    for (size_t i = 0; i < m_a; i++) {
        for (size_t j = 0; j < m_a; j++) {
            element_t x = poind(a, i, j);
            change(a, i, j, x * b);
        }
    }
    return a;
}

struct matrix *m_exp(struct matrix *a, element_t eps) {
    if (eps != eps) {
        return NULL;
    }
    size_t m_a = matrix_m(a);
    size_t na = matrix_n(a);
    if (m_a != na) {
        return NULL;
    }
    element_t A = 1.0 / 0.0;
    if (A == eps) {
        return NULL;
    }
    struct matrix *deg = ed_d_alloc(m_a, na);
    struct matrix *cur = null_alloc(m_a, na);
    double N = 1;
    while (m_norm(deg) >= eps) {
        struct matrix *x = null_alloc(m_a, na);
        cur = summa(cur, deg);
        x = matrix_multiplie(deg, a, x);
        x = mult_mtrx_on_number(x, 1 / N);
        N++;
        struct matrix *y = deg;
        deg = x;
        matrix_free(y);
    }
    cur = summa(cur, deg);
    matrix_free(deg);
    return cur;
}

void replacement(struct matrix *a, size_t x, size_t y) {  // Замена строк
    size_t na = matrix_n(a);
    element_t *b = malloc(na * sizeof(element_t));
    for (size_t j = 0; j < na; j++) {
        b[j] = poind(a, x, j);
    }
    for (size_t j = 0; j < na; j++) {
        element_t w = poind(a, y, j);
        change(a, x, j, w);
    }
    for (size_t j = 0; j < na; j++) {
        change(a, y, j, b[j]);
    }
    free(b);
}

void subsrt(struct matrix *a, size_t x, size_t y, element_t c) {  // Вычитание строк
    size_t na = matrix_n(a);
    for (size_t j = 0; j < na; j++) {
        element_t k = poind(a, x, j);
        element_t l = poind(a, y, j);
        change(a, x, j, k - c * l);
    }
}

void str_mult(struct matrix *a, size_t x, element_t c) {  // Умножение строки на число
    size_t na = matrix_n(a);
    for (size_t j = 0; j < na; j++) {
        element_t k = poind(a, x, j);
        change(a, x, j, k * c);
    }
}

struct matrix *gauss(struct matrix *a) {  // Выполняет метод Гаусса
    size_t m_a = matrix_m(a);
    size_t na = matrix_n(a);
    struct matrix *b = alloc(m_a, 1);
    for (size_t i = 0; i < m_a; i++) {
        if (poind(a, i, i) == 0.0) {
            for (size_t j = i; j < m_a; j++) {
                if (poind(a, j, i) != 0.0) {
                    replacement(a, i, j);
                    break;
                }
            }
        }
        if (poind(a, i, i) == 0.0) {
            change(b, 0, 0, 0.0 / 0.0);
            return b;
        }
        element_t k = poind(a, i, i);
        for (size_t j = i + 1; j < m_a; j++) {
            element_t l = poind(a, j, i);
            subsrt(a, j, i, l / k);
        }
    }
    if (poind(a, m_a - 1, m_a - 1) == 0.0) {
        change(b, 0, 0, 0.0 / 0.0);
        return b;
    }
    for (size_t j = m_a; j > 0; j--) {
        size_t i = j - 1;
        element_t x = poind(a, i, na - 1);
        for (size_t j = m_a - 1; j > i; j--) {
            x -= poind(b, j, 0) * poind(a, i, j);
        }
        element_t u = poind(a, i, i);
        change(b, i, 0, x / u);
    }
    return b;
}
