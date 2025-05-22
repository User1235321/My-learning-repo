#ifndef IMPULSEGEN_RTL_H
#define IMPULSEGEN_RTL_H

#include <systemc.h>

/**
  Impulse generator:
   creates <count> impulses every <sync>
  */
SC_MODULE(ImpulseGenRTL)
{
    /*** INPUT SIGNALS: ***/
    sc_in<bool> clk;            // input clock
    sc_in<bool> reset;          // input reset
    sc_in<bool> sync;           // input synchronisation signal
    sc_in<sc_uint<4> > count;   // impulse count
    
    /*** OUTPUT SIGNALS: ***/
    sc_out<bool> impulses;      // output impulses

    /*** INTERNAL SIGNALS: ***/
    sc_signal<bool> new_impulses;
    sc_signal<sc_uint<4> > count_left, new_count_left; // impulses left
    
    void comb_logic();
    void seq_logic();

    // constructor of module
    SC_CTOR(ImpulseGenRTL)
    {
        SC_METHOD(seq_logic);
        sensitive << clk.pos();
        
        SC_METHOD(comb_logic);
        sensitive << sync << count_left << impulses << count;
    }
};

#endif // IMPULSEGEN_RTL_H
