#ifndef INITS_HPP
#define INITS_HPP

#include <string>
#include <vector>

#include "../mainSystem/buffer.hpp"
#include "../mainSystem/bufferHandlerDispatcher.hpp"
#include "../mainSystem/handler.hpp"
#include "printer.hpp"
#include "../mainSystem/source.hpp"
#include "../mainSystem/sourceBufferDispatcher.hpp"

std::vector< source > initSources(size_t numOfSrc, const std::vector< long double > & lambda, long double delay, printer * print);
std::vector< buffer > initBuffers(size_t numOfBuffs, size_t buffSize, printer * print);
std::vector< handler > initHandlers(size_t numOfHands, long double delay, const std::vector< long double > & handWork, printer * print);
std::vector< sourceBufferDispatcher > initFDisps(size_t numOfFDisps, std::vector< source > * sources, std::vector< buffer > * buff, long double delay);
std::vector< bufferHandlerDispatcher > initSDisps(size_t numOfSDisps, std::vector< handler > * handlers, std::vector< buffer > * buff, long double delay);

#endif
