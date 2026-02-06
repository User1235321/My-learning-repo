#ifndef SOLVE_HPP
#define SOLVE_HPP

#include <vector>

using table = std::vector< std::vector< double > >;

void findStatisticalandDiscreteSeries(const std::vector< int > & arr, table & series, int h, int numOfInterval);
int findModa(const table & series, int h);
double findMedian(const table & series, int h);
void findSampleValues(const table & series, int h, table & sampleValues);
void findNormalCurve(const table & series, table & normalCurve, double sampleAverage, double squareDeviationAverage, int n, int h);
void findHypothesisTable(const table & normalCurve, table & HypothesisTable);

#endif
