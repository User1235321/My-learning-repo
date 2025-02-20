#ifndef MATRIXFUNC_HPP
#define MATRIXFUNC_HPP

#include <iostream>
#include <vector>

using vec = std::vector< double >;

void fillMatrix(vec & m) noexcept;
void printMatrix(const vec & m, std::ostream & out);
vec antiMatrix(const vec & m);
vec multiMatrix(const vec & first, const vec & second) noexcept;
double calcNorm(const vec & m);

#endif
