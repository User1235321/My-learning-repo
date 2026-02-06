#ifndef STOPWATCHCONTROLLER_H
#define STOPWATCHCONTROLLER_H

#include <systemc.h>

SC_MODULE(stopwatchController)
{
  sc_in< bool > clk;
  sc_in< bool > reset;
  sc_in< bool > start;
  sc_in< bool > stop;
  sc_out< sc_uint< 6 > > time;

  sc_signal< bool > working;
  sc_signal< sc_uint< 6 > > counter;

  void update_state();

  SC_CTOR(stopwatchController)
  {
    SC_METHOD(update_state);
    sensitive << clk.pos();
    dont_initialize();
  }
};

#endif
