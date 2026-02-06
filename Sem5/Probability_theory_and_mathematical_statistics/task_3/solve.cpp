#include "solve.hpp"
#include <iostream>
#include <cmath>

void findStatisticalandDiscreteSeries(const std::vector< int > & arr, table & series, int h, int numOfInterval)
{
  series[0].push_back(arr[0] - h / 2);
  series[1].push_back(arr[0]);
  series[2].push_back(0);
  int q = 0;
  while (arr[q] < (series[0][0] + h))
  {
    ++series[2][0];
    ++q;
  }
  series[3].push_back(series[2][0]);
  series[4].push_back(series[2][0] / arr.size());
  series[5].push_back(series[4][0]);
  for (size_t i = 1; i < numOfInterval; ++i)
  {
    series[0].push_back(series[0][i - 1] + h);
    series[1].push_back(series[1][i - 1] + h);
    series[2].push_back(0);
    while (arr[q] < (series[0][i] + h) && (q < arr.size()))
    {
      ++series[2][i];
      ++q;
    }
    series[3].push_back(series[3][i - 1] + series[2][i]);
    series[4].push_back(series[2][i] / arr.size());
    series[5].push_back(series[5][i - 1] + series[4][i]);
  }
}

int findModa(const table & series, int h)
{
  int maxI = 0, maxFrequency = 0, result = 0;
  for (size_t i = 0; i < series[0].size(); ++i)
  {
    if (maxFrequency < series[2][i])
    {
      maxFrequency = series[2][i];
      maxI = i;
    }
  }
  result = series[0][maxI] + h * ((series[2][maxI] - series[2][maxI - 1]) / (2 * series[2][maxI] - series[2][maxI - 1] - series[2][maxI + 1]));
  return result;
}

double findMedian(const table & series, int h)
{
  int medI = 0;
  double result = 0;
  while (series[3][medI] < (series[3][series[3].size() - 1] / 2))
  {
    ++medI;
  }
  result = series[0][medI] + h * ((series[3][series[3].size() - 1] / 2) - series[3][medI - 1]) / series[2][medI];
  return result;
}

void findSampleValues(const table & series, int h, table & sampleValues)
{
  double maxN = 0, C = 0;
  sampleValues[0] = series[1];
  sampleValues[1] = series[2];
  for (size_t i = 0; i < series[0].size(); ++i)
  {
    if (maxN < series[2][i])
    {
      maxN = series[2][i];
      C = series[1][i];
    }
    //sampleValues[0].push_back(series[1][i]);
    //sampleValues[1].push_back(series[2][i]);
  }
  for (size_t i = 0; i < series[0].size(); ++i)
  {
    sampleValues[2].push_back((sampleValues[0][i] - C) / h);
    for (size_t j = 1; j < 5; ++j)
    {
      sampleValues[2 + j].push_back(sampleValues[1][i] * std::pow(sampleValues[2][i], j));
    }
    sampleValues[7].push_back(sampleValues[1][i] * std::pow(sampleValues[2][i] + 1, 4));
  }
  for (size_t i = 0; i < sampleValues.size(); ++i)
  {
    double sum = 0;
    for (size_t j = 0; j < sampleValues[i].size(); ++j)
    {
      sum += sampleValues[i][j];
    }
    sampleValues[i].push_back(sum);
  }
}

void findNormalCurve(const table & series, table & normalCurve, double sampleAverage, double squareDeviationAverage, int n, int h)
{
  normalCurve[0] = series[1];
  normalCurve[1] = series[2];
  for (size_t i = 0; i < normalCurve[0].size(); ++i)
  {
    normalCurve[2].push_back(normalCurve[0][i] - sampleAverage);
    normalCurve[3].push_back(normalCurve[2][i] / squareDeviationAverage);
    normalCurve[4].push_back((1 / std::sqrt(2 * 3.14)) * (std::exp(-(normalCurve[3][i] * normalCurve[3][i]) / 2)));
    normalCurve[5].push_back((n * h / squareDeviationAverage) * normalCurve[4][i]);
  }
}

void findHypothesisTable(const table & normalCurve, table & HypothesisTable)
{
  HypothesisTable[0] = normalCurve[1];
  //HypothesisTable[1] = normalCurve[5];
  for (size_t i = 0; i < HypothesisTable[0].size(); ++i)
  {
    HypothesisTable[1].push_back((int)(normalCurve[5][i]));
    if (((normalCurve[5][i]) * 10) - (((int)(normalCurve[5][i])) * 10) > 5)
    {
      ++HypothesisTable[1][i];
    }
  }
  for (size_t i = 0; i < HypothesisTable[0].size(); ++i)
  {
    HypothesisTable[2].push_back(HypothesisTable[0][i] - HypothesisTable[1][i]);
    HypothesisTable[3].push_back(std::pow(HypothesisTable[2][i], 2));
    HypothesisTable[4].push_back(HypothesisTable[3][i] / HypothesisTable[1][i]);
    HypothesisTable[5].push_back(std::pow(HypothesisTable[0][i], 2));
    HypothesisTable[6].push_back(HypothesisTable[5][i] / HypothesisTable[1][i]);
  }
}
