#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

#include "printTemplate.hpp"
#include "solve.hpp"
#include "draw.hpp"

int main()
{
  setlocale(LC_ALL, "Russian");

/*  std::vector< int > arr = {
    43, 49, 25, 22,
    28, 36, 36, 28,
    45, 21, 48, 49,
    29, 25, 31, 23,
    31, 40, 35, 32,
    18, 26, 43, 33,
    36, 25, 38, 27,
    39, 33, 26, 43,
    32, 34, 35, 35,
    44, 21, 31, 37
    };
*/
  std::vector< int > arr = {11, 11, 11, 11, 11, 11, 22, 22, 22, 22, 22, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 44, 44, 55, 55, 55, 55, 55, 55, 55, 55};
  std::sort(arr.begin(), arr.end());
  const int numOfInterval = 10;

  //Find h
  int h = ((arr[arr.size() - 1] - arr[0]) / numOfInterval);
  h += ((h * numOfInterval) < (arr[arr.size() - 1] - arr[0]));

  //Find statistical and discrete series
  using table = std::vector< std::vector< double > >;
  table series(6);
  findStatisticalandDiscreteSeries(arr, series, h, numOfInterval);

  //Print statistical and discrete series
  std::cout << "\033[1;32m1. Интервальный статистический ряд и распределение относительных частот:\033[0m\n";
  std::cout << "(xi, xi + 1)\tni\t\tni/n\n";
  for (size_t i = 0; i < series[0].size(); ++i)
  {
    std::cout << series[0][i] << '-' << (series[0][i] + h) << "\t\t";
    std::cout << series[2][i] << "\t\t";
    std::cout << series[4][i] << "\t\t";
    std::cout << '\n';
  }
  printTable(table{series[0], series[2], series[4]}, std::vector< std::string >{"(xi, xi + 1)", "ni", "ni/n"}, "1. Интервальный статистический ряд и распределение относительных частот", std::cout);
  std::cout << "\033[1;32m2. Размах вариации: " << (arr[arr.size() - 1] - arr[0]) << "\033[0m\n";
  printTable(table{series[1], series[4]}, std::vector< std::string >{"xi", "ni/n"}, "3. Дискретный ряд распределения", std::cout);
  printTable(table{series[1], series[3], series[5]}, std::vector< std::string >{"xi", "ni", "ni/n"}, "Накопленные частоты", std::cout);

  //Draw polygon, histogram and cumulative
  drawPolygon(table{series[1], series[4]});
  drawHistogram(table{series[1], series[4]});
  drawCumulative(table{series[1], series[5]});

  //Find empirical distribution function
  std::cout << "\033[1;32m4. Эмпирическая функция распределения: \033[0m\n";
  std::cout << "Если x ≤ " << series[1][0] << ", то F(x) = 0\n";
  for (size_t i = 0; i < (numOfInterval - 1); ++i)
  {
    std::cout << "Если x < " << series[1][i] << " ≤ " << series[1][i + 1] << ", то F(x) = " << series[5][i] << '\n';
  }
  std::cout << "Если x > " << series[1][(numOfInterval - 1)] << ", то F(x) = " << series[5][(numOfInterval - 1)] << '\n';
  drawEmpiricalFunctionGraph(table{series[1], series[5]});

  //Find specifications
  int moda = findModa(series, h);
  double median = findMedian(series, h);
  std::cout << "\033[1;32m5. Числовые характеристики:\033[0m\n";
  std::cout << "Мода = " << moda << '\n';
  std::cout << "Медиана = " << median << '\n';

  //Find sample values
  table sampleValues(8);
  findSampleValues(series, h, sampleValues);
  printTable(sampleValues, std::vector< std::string >{"xi", "ni", "ui", "niui", "niui^2", "niui^3", "nuui^4", "ni(ui + 1)^4"}, "Таблица выборочных моментов", std::cout);
  int sum1 = 0, sum2 = 0;
  sum1 = sampleValues[7][sampleValues[0].size() - 1];
  sum2 += sampleValues[1][sampleValues[0].size() - 1];
  sum2 += 4 * sampleValues[3][sampleValues[0].size() - 1];
  sum2 += 6 * sampleValues[4][sampleValues[0].size() - 1];
  sum2 += 4 * sampleValues[5][sampleValues[0].size() - 1];
  sum2 += sampleValues[6][sampleValues[0].size() - 1];
  std::cout << "Сумма ni(ui + 1)^4 = " << sum1 << "; Сумма остальных выборочных моментов = " << sum2 << '\n';

  //Find moments, dispercion etc
  std::vector< double > initialMoments(4);
  for (size_t i = 0; i < 4; ++i)
  {
    initialMoments[i] = sampleValues[3 + i][sampleValues[0].size() - 1] / arr.size();
  }
  double C = 0, maxN = 0;
  for (size_t i = 0; i < series[0].size(); ++i)
  {
    if (maxN < series[2][i])
    {
      maxN = series[2][i];
      C = series[1][i];
    }
  }
  double sampleAverage = initialMoments[0] * h + C;
  double dispersion = (initialMoments[1] - std::pow(initialMoments[0], 2)) * std::pow(h, 2);
  double squareDeviationAverage = std::sqrt(dispersion);
  std::vector< double > centralMoments(2);
  centralMoments[0] = initialMoments[2];
  centralMoments[0] -= 3 * initialMoments[0] * initialMoments[1];
  centralMoments[0] += 2 * std::pow(initialMoments[0], 3);
  centralMoments[0] *= std::pow(h, 3);
  centralMoments[1] = initialMoments[3];
  centralMoments[1] -= 4 * initialMoments[0] * initialMoments[2];
  centralMoments[1] += 6 * std::pow(initialMoments[0], 2) * initialMoments[1];
  centralMoments[1] -= 3 * std::pow(initialMoments[0], 4);
  centralMoments[1] *= std::pow(h, 4);
  double coefficientOfVariation = squareDeviationAverage / sampleAverage;
  double asymmetry = centralMoments[0] / std::pow(squareDeviationAverage, 3);
  double excess = centralMoments[1] / std::pow(squareDeviationAverage, 4) - 3;
  std::cout << "Начальные моменты:\n";
  for (size_t i = 0; i < initialMoments.size(); ++i)
  {
    std::cout << "M*" << (i + 1) << " = " << initialMoments[i] << '\n';
  }
  std::cout << "Выборочная средняя = " << sampleAverage << '\n';
  std::cout << "Дисперсия = " << dispersion << '\n';
  std::cout << "Среднее квадратичное отклонение = " << squareDeviationAverage << '\n';
  std::cout << "Центральные моменты:\n";
  std::cout << "m3 = " << centralMoments[0] << '\n';
  std::cout << "m4 = " << centralMoments[1] << '\n';
  std::cout << "Коэффициент вариации = " << coefficientOfVariation << '\n';
  std::cout << "Ассимтрия = " << asymmetry << '\n';
  std::cout << "Эксцесс = " << excess << '\n';

  //Find normal curve
  table normalCurve(6);
  findNormalCurve(series, normalCurve, sampleAverage, squareDeviationAverage, arr.size(), h);
  printTable(normalCurve, std::vector< std::string >{"xi", "ni", "xi - xb", "ui", "fi(ui)", "n'i"}, "Таблица для нормальной кривой", std::cout);
  drawNormalCurveGraph(normalCurve);

  //Proof the hypothesis
  table HypothesisTable(7);
  findHypothesisTable(normalCurve, HypothesisTable);
  std::cout << "\033[1;32m7. Проверка гипотезы: \033[0m\n";
  printTable(HypothesisTable, std::vector< std::string >{"ni", "n'i", "ni - n'i", "(ni - n'i)^2", "(ni - n'i)^2/n'i", "ni^2", "ni^2/n'i"}, "Таблица для проверки гипотезы", std::cout);
  double xNabl = 0, lastSum = 0;
  for(size_t i = 0; i < HypothesisTable[0].size(); ++i)
  {
    xNabl += HypothesisTable[4][i];
    lastSum += HypothesisTable[6][i];
  }
  std::cout << "X^2набл = " << xNabl << "; Сумма последнего столбца = " << lastSum << '\n';

  //Finding the confidence interval
  std::cout << "\033[1;32m8. Доверительный интервал: \033[0m\n";
  double t = 1.96;
  double accuracyOfAssessment = (t * squareDeviationAverage) / sqrt(arr.size());
  std::cout << "Точность оценки = " << accuracyOfAssessment << '\n';
  std::cout << sampleAverage - accuracyOfAssessment << " < M(X) < " << sampleAverage + accuracyOfAssessment << '\n';
  double q = 0.24;
  std::cout << squareDeviationAverage * (1 - q) << " < sigma < " << squareDeviationAverage * (1 + q) << '\n';
  return 0;
}
