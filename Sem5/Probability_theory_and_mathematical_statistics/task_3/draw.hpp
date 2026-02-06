#ifndef DRAW_HPP
#define DRAW_HPP

#include <vector>

void drawPolygon(const std::vector< std::vector< double > > & table);
void drawHistogram(const std::vector< std::vector< double > > & table);
void drawCumulative(const std::vector< std::vector< double > > & table);
void drawEmpiricalFunctionGraph(const std::vector< std::vector< double > > & table);
void drawNormalCurveGraph(const std::vector< std::vector< double > > & table);

#endif
