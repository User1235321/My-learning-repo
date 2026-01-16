#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <atomic>
#include <vector>

#include "buffer.hpp"
#include "handler.hpp"
#include "source.hpp"

void printForStep(std::ostream & out, const std::vector< buffer > & buff, const std::vector< handler > & handlers, size_t step);
void printRes(std::ostream & out, const std::vector< buffer > & buff, const std::vector< handler > & handlers, const std::vector< source > & sources);

#endif
