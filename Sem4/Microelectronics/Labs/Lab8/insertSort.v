module insert_sort(clk, start, reset, inData, done, outData);
  parameter size = 8;
  input clk, start, reset;
  input [31 : 0] inData;
  output reg done;
  output reg [31 : 0] outData;

  integer localData[(size - 1) : 0];
  integer i = 0, j = 0, temp = 0;
  reg isWorking = 0, inFlag = 0, outFlag = 0;

  always @(posedge clk)
  begin
    if (reset)
    begin
      outData = 0;
      done = 0;
      isWorking = 0;
      inFlag = 0;
      outFlag = 0;
    end
    else if (start)
    begin
      outData = 0;
      done = 0;
      inFlag = 1;
      localData[0] = inData;
      i = 1;
      j = 0;
    end
    else if (inFlag)
    begin
      localData[i] = inData;
      i = i + 1;
      if (i == size)
      begin
        i = 0;
        inFlag = 0;
        isWorking = 1;
      end
    end
    else if (isWorking == 1)
    begin
      if (i < (size - 1))
      begin
        if ((j >= 0) && (localData[j] > localData[j + 1]))
        begin
          temp = localData[j];
          localData[j] = localData[j + 1];
          localData[j + 1] = temp;
          j = j - 1;
        end
        else
        begin
          j = i + 1;
          i = i + 1;
        end
      end
      else
      begin
        done = 1;
        outFlag = 1;
        isWorking = 0;
        i = 0;
        j = 0;
      end
    end
    else if (outFlag)
    begin
      outData = localData[i];
      i = i + 1;
      if (i == size)
      begin
        outFlag = 0;
        i = 0;
      end
    end
  end
endmodule
