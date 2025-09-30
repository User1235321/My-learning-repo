#include <iostream>

int main()
{
  int Imax = 0, Emax= 0, maxProfit = 0;
  for (int A = 0; A <= 6; ++A)
  {
    for (int B = 0; B <= 8; ++B)
    {
      int Enow = std::min(A, B / 2);
      int Inow = std::min((A - Enow) / 2, B - Enow * 2);
      if ((Inow <= 2) && (Inow <= (Enow + 1)) && (Inow > 0) && (Enow > 0))
      {
        if (maxProfit < (3 * Enow + 2 * Inow))
        {
          maxProfit = 3 * Enow + 2 * Inow;
          Imax = Inow;
          Emax = Enow;
        }
      }
    }
  }
  std::cout << "Profit: " << maxProfit << "; E: " << Emax << "; I: " << Imax << '\n';
  return 0;
}
