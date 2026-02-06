#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "matrix.h"

Matrix * create_matrix(int rows, int cols) 
{
  if (rows <= 0 || cols <= 0) 
  {
    return NULL;
  }
  
  Matrix * m = (Matrix *)malloc(sizeof(Matrix));
  if (m == NULL) 
  {
    return NULL;
  }
  
  m -> rows = rows;
  m -> cols = cols;
  
  m -> data = (double **)malloc(rows * sizeof(double *));
  if (m -> data == NULL) 
  {
    free(m);
    return NULL;
  }
  
  for (int i = 0; i < rows; ++i) 
  {
    m -> data[i] = (double *)calloc(cols, sizeof(double));
    if (m -> data[i] == NULL) 
    {
      for (int j = 0; j < i; ++j) 
      {
        free(m -> data[j]);
      }
      free(m -> data);
      free(m);
      return NULL;
    }
  }
  
  return m;
}

void free_matrix(Matrix * m) 
{
  if (m == NULL) 
  {
    return;
  }
  
  if (m -> data != NULL) 
  {
    for (int i = 0; i < m -> rows; ++i) 
    {
      if (m -> data[i] != NULL) 
      {
        free(m -> data[i]);
      }
    }
    free(m -> data);
  }
  free(m);
}

void print_matrix(Matrix * m) 
{
  if (m == NULL || m -> data == NULL) 
  {
    printf("Invalid matrix\n");
    return;
  }
  
  for (int i = 0; i < m -> rows; ++i) 
  {
    for (int j = 0; j < m -> cols; ++j) 
    {
      printf("%8.2f ", m -> data[i][j]);
    }
    printf("\n");
  }
}

int copy_matrix(Matrix * dest, Matrix * src) 
{
  if (dest == NULL || src == NULL || dest -> rows != src -> rows || dest -> cols != src -> cols) 
  {
    return 0;
  }
  
  for (int i = 0; i < src -> rows; ++i) 
  {
    for (int j = 0; j < src -> cols; ++j) 
    {
      dest -> data[i][j] = src -> data[i][j];
    }
  }
  
  return 1;
}

Matrix * add_matrices(Matrix * a, Matrix * b) 
{
  if (a == NULL || b == NULL || a -> rows != b -> rows || a -> cols != b -> cols) 
  {
    return NULL;
  }
  
  Matrix * result = create_matrix(a -> rows, a -> cols);
  if (result == NULL) 
  {
    return NULL;
  }
  
  for (int i = 0; i < a -> rows; ++i) 
  {
    for (int j = 0; j < a -> cols; ++j) 
    {
      result -> data[i][j] = a -> data[i][j] + b -> data[i][j];
    }
  }
  
  return result;
}

Matrix * subtract_matrices(Matrix * a, Matrix * b) 
{
  if (a == NULL || b == NULL || a -> rows != b -> rows || a -> cols != b -> cols) 
  {
    return NULL;
  }
  
  Matrix * result = create_matrix(a -> rows, a -> cols);
  if (result == NULL) 
  {
    return NULL;
  }
  
  for (int i = 0; i < a -> rows; ++i) 
  {
    for (int j = 0; j < a -> cols; ++j) 
    {
      result -> data[i][j] = a -> data[i][j] - b -> data[i][j];
    }
  }
  
  return result;
}

Matrix * multiply_matrices(Matrix * a, Matrix * b) 
{
  if (a == NULL || b == NULL || a -> cols != b -> rows) 
  {
    return NULL;
  }
  
  Matrix * result = create_matrix(a -> rows, b -> cols);
  if (result == NULL) 
  {
    return NULL;
  }
  
  for (int i = 0; i < a -> rows; ++i) 
  {
    for (int j = 0; j < b -> cols; ++j) 
    {
      result -> data[i][j] = 0.0;
      for (int k = 0; k < a -> cols; ++k) 
      {
        result -> data[i][j] += a -> data[i][k] * b -> data[k][j];
      }
    }
  }
  
  return result;
}

Matrix * multiply_matrix_by_scalar(Matrix * m, double scalar) 
{
  if (m == NULL) 
  {
    return NULL;
  }
  
  Matrix * result = create_matrix(m -> rows, m -> cols);
  if (result == NULL) 
  {
    return NULL;
  }
  
  for (int i = 0; i < m -> rows; ++i) 
  {
    for (int j = 0; j < m -> cols; ++j) 
    {
      result -> data[i][j] = m -> data[i][j] * scalar;
    }
  }
  
  return result;
}

int is_square_matrix(Matrix * m) 
{
  return (m != NULL && m -> rows == m -> cols);
}

Matrix * identity_matrix(int size) 
{
  if (size <= 0) 
  {
    return NULL;
  }
  
  Matrix * result = create_matrix(size, size);
  if (result == NULL) 
  {
    return NULL;
  }
  
  for (int i = 0; i < size; ++i) 
  {
    for (int j = 0; j < size; ++j) 
    {
      result -> data[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }
  
  return result;
}

Matrix * transpose(Matrix * m) 
{
  if (m == NULL) 
  {
    return NULL;
  }
  
  Matrix * result = create_matrix(m -> cols, m -> rows);
  if (result == NULL) 
  {
    return NULL;
  }
  
  for (int i = 0; i < m -> rows; ++i) 
  {
    for (int j = 0; j < m -> cols; ++j) 
    {
      result -> data[j][i] = m -> data[i][j];
    }
  }
  
  return result;
}

static double calculate_determinant(double ** matrix, int n) 
{
  if (n == 1) 
  {
    return matrix[0][0];
  }
  
  if (n == 2) 
  {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  }
  
  double det = 0.0;
  double ** submatrix = (double **)malloc((n - 1) * sizeof(double *));
  
  for (int i = 0; i < n - 1; ++i) 
  {
    submatrix[i] = (double *)malloc((n - 1) * sizeof(double));
  }
  
  for (int x = 0; x < n; ++x) 
  {
    int subi = 0;
    for (int i = 1; i < n; ++i) 
    {
      int subj = 0;
      for (int j = 0; j < n; ++j) 
      {
        if (j == x) 
        {
          continue;
        }
        submatrix[subi][subj] = matrix[i][j];
        ++subj;
      }
      ++subi;
    }
    
    double sign = (x % 2 == 0) ? 1.0 : -1.0;
    det += sign * matrix[0][x] * calculate_determinant(submatrix, n - 1);
  }
  
  for (int i = 0; i < n - 1; ++i) 
  {
    free(submatrix[i]);
  }
  free(submatrix);
  
  return det;
}

double determinant(Matrix * m) 
{
  if (!is_square_matrix(m)) 
  {
    return 0.0;
  }
  
  return calculate_determinant(m -> data, m -> rows);
}

Matrix * inverse(Matrix * m) 
{
  if (!is_square_matrix(m)) 
  {
    return NULL;
  }
  
  int n = m -> rows;
  double det = determinant(m);
  
  if (fabs(det) < 1e-10) 
  {
    return NULL;
  }
  
  Matrix * inv = create_matrix(n, n);
  if (inv == NULL) 
  {
    return NULL;
  }
  
  if (n == 1) 
  {
    inv -> data[0][0] = 1.0 / m -> data[0][0];
    return inv;
  }
  
  Matrix * adj = create_matrix(n, n);
  if (adj == NULL) 
  {
    free_matrix(inv);
    return NULL;
  }
  
  Matrix * temp = create_matrix(n - 1, n - 1);
  if (temp == NULL) 
  {
    free_matrix(inv);
    free_matrix(adj);
    return NULL;
  }
  
  for (int i = 0; i < n; ++i) 
  {
    for (int j = 0; j < n; ++j) 
    {
      int row = 0;
      for (int k = 0; k < n; ++k) 
      {
        if (k == i) 
        {
          continue;
        }
        
        int col = 0;
        for (int l = 0; l < n; ++l) 
        {
          if (l == j) 
          {
            continue;
          }
          
          temp -> data[row][col] = m -> data[k][l];
          ++col;
        }
        ++row;
      }
      
      double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
      adj -> data[j][i] = sign * determinant(temp);
    }
  }
  
  for (int i = 0; i < n; ++i) 
  {
    for (int j = 0; j < n; ++j) 
    {
      inv -> data[i][j] = adj -> data[i][j] / det;
    }
  }
  
  free_matrix(adj);
  free_matrix(temp);
  
  return inv;
}
