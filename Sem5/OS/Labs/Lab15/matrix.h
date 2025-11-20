#ifndef MATRIX_H
#define MATRIX_H

typedef struct
{
  double ** data;
  int rows;
  int cols;
} Matrix;

Matrix * create_matrix(int rows, int cols);
void free_matrix(Matrix * m);
void print_matrix(Matrix * m);
int copy_matrix(Matrix * dest, Matrix * src);

Matrix * add_matrices(Matrix * a, Matrix * b);
Matrix * subtract_matrices(Matrix * a, Matrix * b);
Matrix * multiply_matrices(Matrix * a, Matrix * b);
Matrix * multiply_matrix_by_scalar(Matrix * m, double scalar);

double determinant(Matrix * m);
Matrix * transpose(Matrix * m);
Matrix * inverse(Matrix * m);
Matrix * identity_matrix(int size);
int is_square_matrix(Matrix * m);

#endif
