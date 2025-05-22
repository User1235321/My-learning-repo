#include <systemc.h>
#include "stopwatchController.h"
#include "stopwatchControllerTest.h"
#include "stopwatchControllerRTL.h"

int sc_main(int argc,char *argv[])
{
  stopwatchController igen("igen");
  //stopwatchControllerRTL igen("igen");
  stopwatchControllerTest igentest("igen_test");

  sc_clock s_clk("clk", 5 ,SC_NS);
  sc_signal< bool > s_reset("reset");
  sc_signal< bool > s_start("start");
  sc_signal< bool > s_stop("stop");
  sc_signal< sc_uint< 6 > > s_time("time");

  igentest.clk(s_clk);
  igentest.reset(s_reset);
  igentest.start(s_start);
  igentest.stop(s_stop);
  igentest.time(s_time);

  igen.clk(s_clk);
  igen.reset(s_reset);
  igen.start(s_start);
  igen.stop(s_stop);
  igen.time(s_time);

  sc_trace_file * tf = sc_create_vcd_trace_file("lab13");
  tf->set_time_unit(1, SC_NS);

  sc_trace(tf,s_clk, "clk");
  sc_trace(tf,s_reset, "reset");
  sc_trace(tf,s_start, "start");
  sc_trace(tf,s_stop," stop");
  sc_trace(tf,s_time, "time");

  sc_start(20000, SC_NS);
  sc_close_vcd_trace_file(tf);

  return 0;
}
