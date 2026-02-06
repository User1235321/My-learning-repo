#include "inits.hpp"

std::vector< source > initSources(size_t numOfSrc, const std::vector< long double > & lambda, long double delay, printer * print)
{
  std::vector< source > src;
  for (size_t i = 0; i < numOfSrc; ++i)
  {
    src.emplace_back(i, (i + 1), delay, lambda[i], print);
  }
  return src;
}

std::vector< buffer > initBuffers(size_t numOfBuffs, size_t buffSize, printer * print)
{
  std::vector< buffer > buff;
  for (size_t i = 0; i < numOfBuffs; ++i)
  {
    buff.emplace_back(i, buffSize, print);
  }
  return buff;
}

std::vector< handler > initHandlers(size_t numOfHands, long double delay, const std::vector< long double > & handWork, printer * print)
{
  std::vector< handler > handlers;
  for (size_t i = 0; i < numOfHands; ++i)
  {
    handlers.emplace_back(i, delay, handWork[i], print);
  }
  return handlers;
}

std::vector< sourceBufferDispatcher > initFDisps(size_t numOfFDisps, std::vector< source > * sources, std::vector< buffer > * buff, long double delay)
{
  std::vector< sourceBufferDispatcher > firstDisp;
  for (size_t i = 0; i < numOfFDisps; ++i)
  {
    firstDisp.emplace_back(delay, sources, buff);
  }
  return firstDisp;
}

std::vector< bufferHandlerDispatcher > initSDisps(size_t numOfSDisps, std::vector< handler > * handlers, std::vector< buffer > * buff, long double delay)
{
  std::vector< bufferHandlerDispatcher > secondDisp;
  for (size_t i = 0; i < numOfSDisps; ++i)
  {
    secondDisp.emplace_back(delay, handlers, buff);
  }
  return secondDisp;
}
