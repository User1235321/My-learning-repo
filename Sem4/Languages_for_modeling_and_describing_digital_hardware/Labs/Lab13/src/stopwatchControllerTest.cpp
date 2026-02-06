#include "stopwatchControllerTest.h"

void stopwatchControllerTest::generate_signals()
{
  reset.write(1);
  wait();
  reset.write(0);
  wait(10, SC_NS);

  while(true)
  {
    start.write(1);
    wait(5, SC_NS);
    start.write(0);
    wait(10, SC_NS);
    stop.write(1);
    wait(5, SC_NS);
    stop.write(0);
    wait(5, SC_NS);
  }
}
