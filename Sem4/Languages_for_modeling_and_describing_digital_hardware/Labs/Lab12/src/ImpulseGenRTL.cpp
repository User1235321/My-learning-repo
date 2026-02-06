#include "ImpulseGenRTL.h"

void ImpulseGenRTL::comb_logic()
{
    if(count_left.read() != 0)
    {
        if(impulses.read() != 0)
        {
            new_count_left.write(count_left.read() - 1);
            new_impulses.write(0);
        }
        else
        {
            new_count_left.write(count_left.read());
            new_impulses.write(1);
        }
    }
    else
    {
        if(sync.read())
        {
            new_count_left.write(count.read());
            new_impulses.write(count.read()!=0);
        }
        else
        {
            new_count_left.write(count_left.read());
            new_impulses.write(0);
        }
    }
}

void ImpulseGenRTL::seq_logic()
{
    if(reset)
    {
        count_left.write(0);
        impulses.write(0);
    }
    else
    {
        count_left.write(new_count_left);
        impulses.write(new_impulses);        
    }
}
