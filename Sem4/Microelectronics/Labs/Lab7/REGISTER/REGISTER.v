module DTrigger(Result, Data, Clock, Reset, Ewr, bbuf);

      input  Data, Clock, Reset, Ewr;
      output Result, bbuf;
      reg Res, Buf;

      always @(negedge Clock)
        begin
          if (Reset)
      begin
              Res = 0;
        Buf = 0;
      end
          else
            begin
               if (!Ewr)
                begin
                  Res = Data;
                  Buf = Data;
                end
               else
                  Res = Buf;
            end
        end

       assign Result = !Res;
     assign bbuf = Buf;
endmodule

module lab_6_2(OUTRESULT , EWR, CLOCK, RESET, DATA , EDY, BUF);

parameter numbits=7;

       input [numbits:0] DATA;
       input EWR, CLOCK, RESET, EDY;
       output [numbits:0] OUTRESULT ;
       output [numbits:0] BUF;
       reg [numbits:0] res;
       wire[numbits:0] Res ;
     wire[numbits:0] bbuf;

       integer i;

       DTrigger Dtrig1 (Res[0], DATA[0], CLOCK, RESET, EWR, bbuf[0]);
       DTrigger Dtrig2 (Res[1], DATA[1], CLOCK, RESET, EWR, bbuf[1]);
       DTrigger Dtrig3 (Res[2], DATA[2], CLOCK, RESET, EWR, bbuf[2]);
       DTrigger Dtrig4 (Res[3], DATA[3], CLOCK, RESET, EWR, bbuf[3]);
       DTrigger Dtrig5 (Res[4], DATA[4], CLOCK, RESET, EWR, bbuf[4]);
       DTrigger Dtrig6 (Res[5], DATA[5], CLOCK, RESET, EWR, bbuf[5]);
       DTrigger Dtrig7 (Res[6], DATA[6], CLOCK, RESET, EWR, bbuf[6]);
       DTrigger Dtrig8 (Res[7], DATA[7], CLOCK, RESET, EWR, bbuf[7]);
       always @ (EDY or CLOCK)
         begin
           if (RESET)
              for (i=0; i<=7; i=i+1)
                  res[i] = 0;
           else
             begin
                if (!EDY)
                   res = ~Res;
                else res = 'b0;
             end
         end

       assign OUTRESULT = res;
     assign BUF = bbuf;
endmodule
