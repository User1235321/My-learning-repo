#ifndef MYFUNCS_HPP
#define MYFUNCS_HPP

#include <vector>

double baseFunc(double x);
double LagrangePolynomial(const unsigned pointsNum, const std::vector< double > & basePoints, double x);
double splineNum(const unsigned pointsNum, const std::vector< double > & basePoints, double x);

#endif
