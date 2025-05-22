#include "stopwatchController.h"

void stopwatchController::update_state()
{
  if(reset.read())
  {
    counter.write(0);
    working.write(false);
  }
  else if (start.read())
  {
    working.write(true);
  }
  else if (stop.read())
  {
    working.write(false);
  }
  else if (working.read())
  {
    sc_uint< 6 > new_val = counter.read() + 1;
    if (new_val > 60)
    {
      new_val = 0;
    }
    counter.write(new_val);
    time.write(new_val);
  }
}
