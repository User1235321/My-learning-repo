#include "printTemplate.hpp"

#include <iomanip>

void printTable(const std::vector< std::vector< double > > & table, const std::vector < std::string > & names, std::string tableName, std::ostream & out)
{
  if (!tableName.empty())
  {
    out << "\033[1;32m" << tableName << ":\033[0m\n";
  }
  for (size_t i = 0; i < names.size(); ++i)
  {
    out << names[i] << "\t\t";
  }
  out << '\n';
  for (size_t i = 0; i < table[0].size(); ++i)
  {
    for (size_t j = 0; j < table.size(); ++j)
    {
      out << std::setprecision(4) << table[j][i] << "\t\t";
    }
    out << '\n';
  }
}
