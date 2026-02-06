#include "draw.hpp"

#include <fstream>
#include <string>
#include <cstdlib>

void drawPolygon(const std::vector< std::vector< double > > & table)
{
  std::ofstream dataFile("temp_data.dat");
  for (size_t i = 0; i < table[0].size(); ++i)
  {
    dataFile << table[0][i] << ' ' << table[1][i] << '\n';
  }
  dataFile.close();
  std::ofstream scriptFile("plot_script.gp");
  scriptFile << "set terminal pngcairo enhanced font 'Arial,12' size 1200,800\n";
  scriptFile << "set output 'polygon.png'\n";
  scriptFile << "set title 'Полигон относительных частот'\n";
  scriptFile << "set grid\n";
  scriptFile << "plot 'temp_data.dat' with linespoints lw 2 pt 9 ps 1.5 lc 'blue' notitle\n";
  scriptFile.close();
  std::string command = "gnuplot plot_script.gp";
  system(command.c_str());
}

void drawHistogram(const std::vector< std::vector< double > > & table)
{
  std::ofstream dataFile("temp_data.dat");
  for (size_t i = 0; i < table[0].size(); ++i)
  {
    dataFile << table[0][i] << ' ' << table[1][i] << '\n';
  }
  dataFile.close();
  std::ofstream scriptFile("plot_script.gp");
  scriptFile << "set terminal pngcairo enhanced font 'Arial,12' size 1200,800\n";
  scriptFile << "set output 'histogram.png'\n";
  scriptFile << "set title 'Гистограмма '\n";
  scriptFile << "set grid\n";
  scriptFile << "set boxwidth 0.8 relative\n";
  scriptFile << "set style fill solid 0.7 border\n";
  scriptFile << "set style histogram cluster gap 1\n";
  scriptFile << "plot 'temp_data.dat' using 2:xtic(1) with boxes lc 'purple' notitle\n";
  scriptFile << "set key off\n";
  scriptFile.close();
  std::string command = "gnuplot plot_script.gp";
  system(command.c_str());
}

void drawCumulative(const std::vector< std::vector< double > > & table)
{
  std::ofstream dataFile("temp_data.dat");
  for (size_t i = 0; i < table[0].size(); ++i)
  {
    dataFile << table[0][i] << ' ' << table[1][i] << '\n';
  }
  dataFile.close();
  std::ofstream scriptFile("plot_script.gp");
  scriptFile << "set terminal pngcairo enhanced font 'Arial,12' size 1200,800\n";
  scriptFile << "set output 'cumulative.png'\n";
  scriptFile << "set title 'Кумулята'\n";
  scriptFile << "set grid\n";
  scriptFile << "plot 'temp_data.dat' with linespoints lw 2 pt 13 ps 1.5 lc 'green' notitle\n";
  scriptFile.close();
  std::string command = "gnuplot plot_script.gp";
  system(command.c_str());
}

void drawEmpiricalFunctionGraph(const std::vector< std::vector< double > > & table)
{
  std::ofstream dataFile("temp_data.dat");
  dataFile << 0 << ' ' << 0 << '\n';
  dataFile << table[0][0] << ' ' << 0 << '\n';
  dataFile << '\n';
  for (size_t i = 0; i < (table[0].size() - 1); ++i)
  {
    dataFile << table[0][i] << ' ' << table[1][i] << '\n';
    dataFile << table[0][i + 1] << ' ' << table[1][i] << '\n';
    dataFile << '\n';
  }
  dataFile << table[0][(table[0].size() - 1)] << ' ' << table[1][(table[0].size() - 1)] << '\n';
  dataFile << (table[0][(table[0].size() - 1)] + 10) << ' ' << table[1][(table[0].size() - 1)] << '\n';
  dataFile << '\n';
  dataFile.close();
  std::ofstream scriptFile("plot_script.gp");
  scriptFile << "set terminal pngcairo enhanced font 'Arial,12' size 1200,800\n";
  scriptFile << "set output 'empiricalFunctionGraph.png'\n";
  scriptFile << "set title 'График эмпирической функции'\n";
  scriptFile << "set grid\n";
  scriptFile << "set xrange [15:55]\n";
  scriptFile << "set yrange [0:1]\n";
  scriptFile << "plot 'temp_data.dat' with lines lw 3 lc 'red' notitle\n";
  scriptFile.close();
  std::string command = "gnuplot plot_script.gp";
  system(command.c_str());
}

void drawNormalCurveGraph(const std::vector< std::vector< double > > & table)
{
  std::ofstream dataFile("temp_data.dat");
  for (size_t i = 0; i < table[0].size(); ++i)
  {
    dataFile << table[0][i] << ' ' << table[1][i] << '\n';
  }
  dataFile.close();
  std::ofstream dataFile2("temp_data2.dat");
  for (size_t i = 0; i < table[0].size(); ++i)
  {
    dataFile2 << table[0][i] << ' ' << table[5][i] << '\n';
  }
  dataFile2.close();
  std::ofstream scriptFile("plot_script.gp");
  scriptFile << "set terminal pngcairo enhanced font 'Arial,12' size 1200,800\n";
  scriptFile << "set output 'normalCurve.png'\n";
  scriptFile << "set title 'Нормальная кривая'\n";
  scriptFile << "set grid\n";
  scriptFile << "plot 'temp_data.dat' with linespoints lw 2 pt 5 ps 1.5 lc 'blue' notitle, \\\n";
  scriptFile << " 'temp_data2.dat' with linespoints lw 2 pt 13 ps 1.5 lc 'red' notitle\n";
  scriptFile.close();
  std::string command = "gnuplot plot_script.gp";
  system(command.c_str());
}
