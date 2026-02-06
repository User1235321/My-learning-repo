#ifndef STOPWATCHCONTROLLERTEST_H
#define STOPWATCHCONTROLLERTEST_H

#include <iostream>
#include <systemc.h>

SC_MODULE(stopwatchControllerTest)
{
  sc_in< bool > clk;
  sc_out< bool > reset;
  sc_out< bool > start;
  sc_out< bool > stop;
  sc_in< sc_uint< 6 > > time;

  unsigned int controllerTime;

  void generate_signals();

  SC_CTOR(stopwatchControllerTest):
    controllerTime(0)
  {
    SC_THREAD(generate_signals);
    sensitive << clk.pos();
  }
};

#endif
