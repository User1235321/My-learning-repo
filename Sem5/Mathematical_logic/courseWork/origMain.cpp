#include <iostream>
#include <cmath>

#include <bdd.h>

void initParams(bdd * param, constexpr size_t N, constexpr size_t logN, constexpr size_t M)
{
  size_t I = 0;
  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j < N; ++j)
    {
      for (size_t q = 0; q < 4; ++q)
      {
        param[q][i][j] = bddtrue;
      }
      for (size_t q = 0; q < logN; ++q)
      {
        if((j >> q) & 1)
        {
          for (size_t k = 0; k < 4; ++k)
          {
            param[k][i][j] &= bdd_ithvar(I + q + logN * k);
          }
        }
        else
        {
          for (size_t k = 0; k < 4; ++k)
          {
            param[k][i][j] &= bdd_nithvar(I + q + logN * k);
          }
        }
      }
    }
    I +=  logN * M;
  }
}

void applyFirstCond(bdd & myBdd, bdd * param)
{
  myBdd &= param[1][0][3];
  myBdd &= param[3][3][4];
  myBdd &= param[0][4][6];
  myBdd &= param[3][5][6];
  myBdd &= param[2][5][0];
  myBdd &= param[2][0][1];
  myBdd &= param[0][2][7];
}

void applySecondCond(bdd & myBdd, bdd * param, constexpr size_t N)
{
  for (size_t i = 0; i < N; ++i)
  {
    myBdd &= !(param[1][i][6] ^ param[2][i][2]);
    myBdd &= !(param[2][i][0] ^ param[3][i][6]);
    myBdd &= !(param[2][i][1] ^ param[2][i][3]);
    myBdd &= !(param[0][i][2] ^ param[2][i][0]);
  }
}

void applyThirdCond(bdd & myBdd, bdd * param, constexpr size_t N)
{
  constexpr int side = std::sqrt(N);
  for (int i = 0; i < side; ++i)
  {
    for (int j = 0; j < side; ++j)
    {
      myBdd &= !(param[2][i * side + j][3] ^ param[2][i * side + ((side + j - 1) % side)][3]);
      myBdd &= !(param[2][i * side + j][4] ^ param[0][i * side + ((side + j - 1) % side)][0]);
    }
  }
}

void fourthCondTemp(int paramNum1, int paramVal1, int paramNum2, intparamVal2, bdd & myBdd, bdd * param, constexpr size_t N)
{
  constexpr int side = std::sqrt(N);
  for (int i = 0; i < side; ++i)
  {
    for (int j = 0; j < side; ++j)
    {
      bdd temp = bddfalse;
      temp |= !(param[paramNum1][i * side + j][paramVal1] ^ param[paramNum2][i * side + ((side + j - 1) % side)][paramVal2]);
      temp |= !(param[paramNum1][i * side + j][paramVal1] ^ param[paramNum2][((side + i - 1) % side) * side + ((side + j + 1) % side)][paramVal2]);
      my_bdd &= temp;
    }
  }
}

void applyFourthCond(bdd & myBdd, bdd * param, size_t N)
{
  fourthCondTemp(0, 0, 2, 5, myBdd, param, N);
  fourthCondTemp(2, 2, 1, 0, myBdd, param, N);
  fourthCondTemp(0, 3, 2, 6, myBdd, param, N);
  fourthCondTemp(3, 5, 0, 1, myBdd, param, N);
  fourthCondTemp(2, 3, 1, 2, myBdd, param, N);
  fourthCondTemp(1, 3, 3, 3, myBdd, param, N);
}


void checkCorrectsValues(bdd & myBdd, bdd * param, constexpr size_t N, constexpr size_t M)
{
  for (size_t i = 0; i < N; ++i)
  {
    bdd localBdd[M];
    for (size_t j = 0; j < M; ++j)
    {
      localBdd[j] = bdd_false;
    }
    for (size_t j = 0; j < N; ++j)
    {
      for (size_t q = 0; q < M; ++q)
      {
        localBdd[q] |= param[q][i][j];
      }
    }
    for (size_t j = 0; j < M; ++j) //?
    {
      myBdd &= localBdd[j];
    }
  }
}

void makeValuesUnique(bdd & myBdd, bdd * param, constexpr size_t N, constexpr size_t M)
{
  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = j; j < N - 1; ++i)
    {
      for (size_t q = j + 1; q < N; ++q)
      {
        for (size_t k = 0; k < M; ++k)
        {
          myBdd &= param[k][j][i] >> !param[k][q][i];
        }
      }
    }
  }
}

void fun(char * varset, int size)
{
	build(varset, size, 0);
}

void build(const char * varset, const size_t n, const size_t I)
{
  if (I == (n - 1))
  {
    if (varset[I] >= 0)
    {
      var[I] = varset[I];
      print();
      return;
    }
    var[I] = 0;
    print();
    var[I] = 1;
    print();
    return;
  }
  if (varset[I] >= 0)
  {
    var[I] = varset[I];
    build(varset, n, I + 1);
    return;
  }
  var[I] = 0;
  build(varset, n, I + 1);
  var[I] = 1;
  build(varset, n, I + 1);
}

void print()
{
  for (size_t i = 0; i < N; ++i)
  {
    std::cout << i << ": ";
    for (size_t j = 0; j < M; ++j)
    {
      int J = i * M * LOG_N + j * LOG_N;
      int num = 0;
      for (size_t k = 0; k < LOG_N; ++k)
      {
        num += (size_t)(var[J + k] << k);
      }
      std::cout << num << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

int main()
{
  constexpr size_t N = 9, M = 4;
  constexpr size_t logN = log2(N) + (log2(N) > (int)log2(N));
  constexpr size_t boolNum = N * M * logN;

  bdd_init(100000, 10000);
  bdd_setvarnum(boolNum);

  bdd myBdd = bddtrue;
  bdd param[M][N][N];

  initParam(param, N, logN, M);

  applyFirstCond(myBdd, param);
  applySecondCond(myBdd, param, N);
  applyThridCond(myBdd, param, N);
  applyFourthCond(myBdd, param, N);

  checkCorrectsValues(myBdd, param, N, M);
  checkValuesToUnique(myBdd, param, N, M);

  size_t satcount = bdd_satcount(myBdd);
  std::cout << "Found " << satcount << " solution\n";
  if (satcount)
  {
    bdd_allsat(myBdd, fun);
  }

  bdd_done();
  return 0;
}

