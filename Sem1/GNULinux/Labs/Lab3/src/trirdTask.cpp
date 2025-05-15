#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<limits.h>
#include<iostream>

int main(int argc, char *argv[])
{
  FILE	*fin, *fout;
  char	buffer[PIPE_BUF];
  
  int n = 0;
  for (int i = 2; i < argc; ++i)
  {
    fin  = popen(argv[i-1], "r");
    fout = popen(argv[i], "w");
    
    while ((n = read(fileno(fin), buffer, PIPE_BUF)) > 0)
    {
      write(fileno(fout), buffer, n);
    } 
    pclose(fin);
    pclose(fout);
  }

  return 0;
}
