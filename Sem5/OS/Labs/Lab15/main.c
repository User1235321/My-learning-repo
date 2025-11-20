#include <stdio.h>
#include "matrix.h"

int main() 
{
  Matrix * a = create_matrix(3, 3);
  Matrix * b = create_matrix(3, 3);
  
  double values_a[3][3] = {
    {1, 2, 3},
    {0, 1, 4},
    {5, 6, 0}
  };
  
  double values_b[3][3] = {
    {2, 0, 1},
    {1, 2, 3},
    {4, 5, 6}
  };
  
  for (int i = 0; i < 3; ++i) 
  {
    for (int j = 0; j < 3; ++j) 
    {
      a -> data[i][j] = values_a[i][j];
      b -> data[i][j] = values_b[i][j];
    }
  }
  
  printf("Matrix A:\n");
  print_matrix(a);
  
  printf("\nMatrix B:\n");
  print_matrix(b);
  
  Matrix * sum = add_matrices(a, b);
  printf("\nA + B:\n");
  print_matrix(sum);
  
  Matrix * product = multiply_matrices(a, b);
  printf("\nA * B:\n");
  print_matrix(product);
  
  Matrix * transposed = transpose(a);
  printf("\nTranspose of A:\n");
  print_matrix(transposed);
  
  double det = determinant(a);
  printf("\nDeterminant of A: %.2f\n", det);
  
  if (det != 0.0) 
  {
    Matrix * inverse_a = inverse(a);
    printf("\nInverse of A:\n");
    print_matrix(inverse_a);
    
    Matrix * identity_check = multiply_matrices(a, inverse_a);
    printf("\nA * A^(-1):\n");
    print_matrix(identity_check);
    
    free_matrix(inverse_a);
    free_matrix(identity_check);
  }
  
  Matrix * scaled = multiply_matrix_by_scalar(a, 2.5);
  printf("\nA * 2.5:\n");
  print_matrix(scaled);
  
  Matrix * identity = identity_matrix(3);
  printf("\nIdentity matrix 3x3:\n");
  print_matrix(identity);
  
  free_matrix(a);
  free_matrix(b);
  free_matrix(sum);
  free_matrix(product);
  free_matrix(transposed);
  free_matrix(scaled);
  free_matrix(identity);
  
  return 0;
}
