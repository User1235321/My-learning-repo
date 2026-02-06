#ifndef PRINTTEMPLATE_HPP
#define PRINTTEMPLATE_HPP

#include <vector>
#include <iostream>
#include <string>

void printTable(const std::vector< std::vector< double > > & table, const std::vector < std::string > & names, std::string tableName, std::ostream & out);

#endif
