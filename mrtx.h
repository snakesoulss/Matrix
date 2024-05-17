#ifndef MRTX_H_INCLUDED
#define MRTX_H_INCLUDED

typedef struct matrix matrix;
typedef double element_t;
struct matrix;
matrix *summa(matrix *a, matrix *b);
matrix *sum(matrix *a, matrix *b, matrix *c);
matrix *matrix_copy(matrix *a, matrix *b);
void e_swap(element_t *a, element_t *b);
matrix *transportirovanie(matrix *a, matrix *b);
matrix *make_tr_m(matrix *a);
matrix *matrix_multiplie(matrix *a, matrix *b, matrix *c);
element_t m_norm(matrix *a);
matrix *mult_mtrx_on_number(matrix *a, element_t b);
matrix *m_exp(matrix *a, element_t eps);
void replacement(matrix *a, size_t x, size_t y);
void subsrt(matrix *a, size_t x, size_t y, element_t c);
void strumn(matrix *a, size_t x, element_t c);
matrix *gauss (matrix *a);

#endif // MRTX_H_INCLUDED
