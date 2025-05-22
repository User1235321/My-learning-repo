#ifndef STOPWATCHCONTROLLERRTL_H
#define STOPWATCHCONTROLLERRTL_H

#include <systemc.h>

SC_MODULE(stopwatchControllerRTL)
{
  sc_in< bool > clk;
  sc_in< bool > reset;
  sc_in< bool > start;
  sc_in< bool > stop;
  sc_out< sc_uint< 6 > > time;

  sc_signal< bool > running;
  sc_signal< bool > next_running;
  sc_signal< sc_uint< 6 > > counter;
  sc_signal< sc_uint< 6 > > next_counter;

  void comb_logic();
  void seq_logic();

  SC_CTOR(stopwatchControllerRTL)
  {
    SC_METHOD(seq_logic);
    sensitive << clk.pos();
    SC_METHOD(comb_logic);
    sensitive << reset << start << stop << running << counter;
  }
};

#endif
