#include "stopwatchControllerRTL.h"

void stopwatchControllerRTL::comb_logic()
{
  next_running = running;
  next_counter = counter;
  time = counter;

  if (reset.read())
  {
    next_running = false;
    next_counter = 0;
  }
  else
  {
    if (start.read())
    {
      next_running = true;
    }
    else if (stop.read())
    {
      next_running = false;
    }
    else if (running.read())
    {
      sc_uint< 6 > new_count = counter.read() + 1;
      if (new_count > 60)
      {
        new_count = 0;
      }
      next_counter = new_count;
    }
  }
}

void stopwatchControllerRTL::seq_logic()
{
  if(reset.read())
  {
    running.write(false);
    counter.write(0);
  }
  else
  {
    running.write(next_running);
    counter.write(next_counter);
  }
}
