#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <malloc.h>
#include "mrtx.h"
#include "matrix.h"


matrix* summa (matrix* a, matrix* b) {

    if (!a || !b) {
        return NULL;
    }
    size_t m_a = matrix_m(a);
    size_t m_b = matrix_m(b);
    size_t n_a = matrix_n(a);
    size_t n_b = matrix_n(b);

    if(m_a != m_b || n_a != n_b){
        return NULL;
    }

    for (size_t i = 0; i < m_a; i++) {
        for (size_t j = 0; j < n_a; j++) {
            element_t x = poind(a, i, j);
            element_t y = poind(b, i, j);
            change(a, i, j, x+y);
        }
    }

    return a;
}

matrix* matrix_copy(matrix* a, matrix* b) {
    if (!a) {
        return NULL;
    }

    size_t m_a = matrix_m(a);
    size_t m_b = matrix_m(b);
    size_t n_a = matrix_n(a);
    size_t n_b = matrix_n(b);

    if (!b) {
        b = alloc(m_a, n_a);
    } else {
        if (m_b != m_a || n_b != n_a) {
            b = matr_alloc(b, m_a, n_a);
        }
    }

    if (!b) {
        return NULL;
    }

    for (size_t i = 0; i < m_a; i++) {
        for (size_t j = 0; j < n_a; j++) {
            element_t x = poind(a, i, j);
            change(b, i, j, x);
        }
    }

    return b;
}


matrix *sum(matrix *a, matrix *b, matrix *c) {
    c = matrix_copy(a, c);
    c = summa(c, b);
    if(!c) {
        return NULL;
    }
    return c;
}


void e_swap(element_t *a, element_t *b) {
    element_t c = *a;
    *a = *b;
    *b = c;
}


matrix *transportirovanie (matrix *a, matrix *b){        //выполняет транспонирование матрицы
    size_t m_a = matrix_m(a);
    size_t n_a = matrix_n(a);
    if(!b) {
        b = alloc(n_a, m_a);
    }
    else {
        if((matrix_m(b) != n_a) ||(matrix_n(b) != m_a)) {
            b = matr_alloc(b, n_a, m_a);
        }
    }
    if(!b) {
        return NULL;
    }
    for(size_t i = 0; i < m_a; i++) {
        for(size_t j = 0; j < n_a; j++) {
            element_t x = poind(a, i, j);
            change(b, j, i, x);
        }
    }
    return b;
}
matrix *make_tr_m (matrix *a){         //создает транспонированную матрицу и освобождает память под оригинальную
    if(!a) {
        return NULL;
    }
    size_t m_a = matrix_m(a);
    size_t n_a = matrix_n(a);
    matrix *b = null_alloc(n_a, m_a);
    b = transportirovanie(a, b);
    if(!b) {
        return NULL;
    }
    matrix *c = a;
    a = b;
    matrix_free(c);
    return a;
}


matrix *matrix_multiplie(matrix *a, matrix *b, matrix *c) {   //умножает две матрицы
    size_t m_a = matrix_m(a);
    size_t m_b = matrix_m(b);
    size_t n_a = matrix_n(a);
    size_t n_b = matrix_n(b);
    if(n_a != m_b) {
        return NULL;
    }
    if(!c) {
        c = alloc(m_a, n_b);
    }
    else {
        c = matr_alloc(c, m_a, n_b);
    }
    if(!c) {
        return NULL;
    }
    for(size_t i = 0; i < m_a; i++) {
        for(size_t j = 0; j < n_b; j++) {
            change(c, i, j,0);
            for(size_t k = 0; k < n_a; k++) {
                element_t xc = poind(c, i, j);
                element_t xa = poind(a, i, k);
                element_t xb = poind(b, k, j);
                change(c, i, j, xa*xb+xc);
            }
        }
    }
    return c;
}

element_t m_norm(matrix *a) {       // вычисляет норму матрицы
    size_t m_a = matrix_m(a);
    size_t n_a = matrix_n(a);
    element_t norm = 0;
    for(size_t i = 0; i < n_a; i++) {
        element_t curnorm = 0;
        for(size_t j = 0; j < m_a; j++) {
            curnorm += fabs(poind(a, j, i));
        }
        if(curnorm > norm) {
            norm = curnorm;
        }
    }
    return norm;
}


matrix *mult_mtrx_on_number(matrix* a, element_t b) {      //умножает матрицу на число
    if(!a) {
        return NULL;
    }
    size_t m_a = matrix_m(a);
    for(size_t i = 0; i < m_a; i++) {
        for(size_t j = 0; j < m_a; j++){
            element_t x = poind(a, i, j);
            change(a, i, j, x*b);
        }
    }
    return a;
}


matrix *m_exp(matrix *a, element_t eps) {
    if(eps != eps) {
        return NULL;
    }
    size_t m_a = matrix_m(a);
    size_t na = matrix_n(a);
    if(m_a!=na) {
        return NULL;
    }
    element_t A=1.0/0.0;
    if(A == eps) {
        return NULL;
    }
    matrix *deg = ed_d_alloc(m_a, na);
    matrix *cur = null_alloc(m_a, na);
    double N = 1;
    while(m_norm(deg) >= eps) {
        matrix* x = null_alloc(m_a, na);
        cur = summa(cur, deg);
        x = matrix_multiplie(deg, a, x);
        x = mult_mtrx_on_number(x, 1/N);
        N++;
        matrix *y = deg;
        deg = x;
        matrix_free(y);
    }
    cur = summa(cur, deg);
    matrix_free(deg);
    return cur;
}


void replacement(matrix *a, size_t x, size_t y) {       //замена строк
    size_t na = matrix_n(a);
    element_t *b = malloc(na*sizeof(element_t));
    for(size_t j = 0; j < na; j++) {
        b[j] = poind(a, x, j);
    }
    for(size_t j = 0; j < na; j++) {
        element_t w = poind(a, y, j);
        change(a, x, j, w);
    }
    for(size_t j = 0; j < na; j++) {
        change(a, y, j, b[j]);
    }
    free(b);
}


void subsrt(matrix *a, size_t x, size_t y, element_t c) {       //вычитание строк
    size_t na = matrix_n(a);
    for(size_t j=0; j < na; j++) {
        element_t k = poind(a, x, j);
        element_t l = poind(a, y, j);
        change(a, x, j, k-c*l);
    }
}


void str_mult(matrix *a, size_t x, element_t c) {       //умножение строки на число
    size_t na = matrix_n(a);
    for(size_t j = 0; j < na; j++) {
        element_t k = poind(a, x, j);
        change(a, x, j, k*c);
    }
}


matrix *gauss (matrix *a) {         //выполняет метод Гаусса
    size_t m_a = matrix_m(a);
    size_t na = matrix_n(a);
    matrix *b = alloc(m_a, 1);
    for(size_t i = 0; i < m_a; i++) {
        if(poind(a, i, i) == 0.0) {
            for(size_t j = i; j < m_a; j++) {
                if(poind(a, j, i) != 0.0) {
                    replacement(a, i, j);
                    break;
                }
            }
        }
        if(poind(a, i, i) == 0.0) {
            change(b, 0, 0, 0.0/0.0);
            return b;
        }
        element_t k = poind(a, i, i);
        for(size_t j = i+1; j < m_a; j++) {
            element_t l = poind(a, j, i);
            subsrt(a, j, i, l/k);
        }
    }
    if(poind(a, m_a-1, m_a-1)==0.0) {
        change(b, 0, 0, 0.0/0.0);
        return b;
    }
    for(size_t j = m_a; j>0; j--) {
        size_t i=j-1;
        element_t x = poind(a, i, na-1);
        for(size_t j = m_a-1; j>i; j--) {
            x -= poind(b, j, 0) * poind(a, i, j);
        }
        element_t u = poind(a, i, i);
        change(b, i, 0, x/u);
    }
    return b;
}
