#include <iostream>
#include <cmath>
#include <fstream>

#include <bdd.h>

constexpr size_t N = 9, M = 4;
constexpr size_t logN = log2(N) + (log2(N) > (int)log2(N));
constexpr size_t boolNum = N * M * logN;
char var[boolNum];
bdd param[M][N][N];

void initParams()
{
  size_t I = 0;
  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j < N; ++j)
    {
      for (size_t k = 0; k < M; ++k)
      {
        param[k][i][j] = bddtrue;
        for (size_t q = 0; q < logN; ++q)
        {
          if((j >> q) & 1)
          {
            param[k][i][j] &= bdd_ithvar(I + k * logN + q);
          }
          else
          {
            param[k][i][j] &= bdd_nithvar(I + k * logN + q);
          }
        }
      }
    }
    I += logN * M;
  }
}

void applyFirstCond(bdd & myBdd, std::ifstream & in)
{
  for (size_t i = 0; i < 7; ++i)
  {
    int objecNum, paramNum, paramVal;
    in >> objecNum >> paramNum >> paramVal;
    myBdd &= param[paramNum][objecNum][paramVal];
  }
}

void applySecondCond(bdd & myBdd, std::ifstream & in)
{
  int secondParamCond[4][4];
  for (size_t i = 0; i < 4; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      in >> secondParamCond[i][j];
    }
  }

  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      myBdd &= !(param[secondParamCond[j][0]][i][secondParamCond[j][1]] ^ param[secondParamCond[j][2]][i][secondParamCond[j][3]]);
    }
  }
}

void applyThirdCond(bdd & myBdd, std::ifstream & in)
{
  int thirdParamCond[2][4];
  for (size_t i = 0; i < 2; ++i)
  {
    for (size_t j = 0; j < 4; ++j)
    {
      in >> thirdParamCond[i][j];
    }
  }

  constexpr int side = std::sqrt(N);
  for (int i = 0; i < side; ++i)
  {
    for (int j = 0; j < side; ++j)
    {
      for (size_t q = 0; q < 2; ++q)
      {
        myBdd &= !(param[thirdParamCond[q][0]][i * side + j][thirdParamCond[q][1]] ^ param[thirdParamCond[q][2]][i * side + ((side + j - 1) % side)][thirdParamCond[q][3]]);
      }
    }
  }
}

void fourthCondTemp(int paramNum1, int paramVal1, int paramNum2, int paramVal2, bdd & myBdd)
{
  constexpr int side = std::sqrt(N);
  for (int i = 0; i < side; ++i)
  {
    for (int j = 0; j < side; ++j)
    {
      bdd temp = bddfalse;
      temp |= !(param[paramNum1][i * side + j][paramVal1] ^ param[paramNum2][i * side + ((side + j - 1) % side)][paramVal2]);
      temp |= !(param[paramNum1][i * side + j][paramVal1] ^ param[paramNum2][((side + i - 1) % side) * side + ((side + j + 1) % side)][paramVal2]);
      myBdd &= temp;
    }
  }
}

void applyFourthCond(bdd & myBdd, std::ifstream & in)
{
  for (size_t i = 0; i < 6; ++i)
  {
    int paramNum1, paramVal1, paramNum2, paramVal2;
    in >> paramNum1 >> paramVal1 >> paramNum2 >> paramVal2;
    fourthCondTemp(paramNum1, paramVal1, paramNum2, paramVal2, myBdd);
  }
}


void checkCorrectsValues(bdd & myBdd)
{
  for (size_t i = 0; i < N; ++i)
  {
    bdd localBdd[M];
    for (size_t j = 0; j < M; ++j)
    {
      localBdd[j] = bddfalse;
    }
    for (size_t j = 0; j < N; ++j)
    {
      for (size_t q = 0; q < M; ++q)
      {
        localBdd[q] |= param[q][i][j];
      }
    }
    for (size_t j = 0; j < M; ++j)
    {
      myBdd &= localBdd[j];
    }
  }
}

void checkValuesToUnique(bdd & myBdd)
{
  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j < N - 1; ++j)
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

void print()
{
  for (size_t i = 0; i < N; ++i)
  {
    std::cout << i << ": ";
    for (size_t j = 0; j < M; ++j)
    {
      int J = i * M * logN + j * logN;
      int num = 0;
      for (size_t k = 0; k < logN; ++k)
      {
        num += (size_t)(var[J + k] << k);
      }
      std::cout << num << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
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

void fun(char * varset, int size)
{
  build(varset, size, 0);
}

int main()
{
  bdd_init(2000000, 200000);
  bdd_setvarnum(boolNum);

  bdd myBdd = bddtrue;

  initParams();

  std::ifstream paramsBase("params.txt");
  if (!paramsBase)
  {
    std::cerr << "Can't open params file\n";
    return 1;
  }
  applyFirstCond(myBdd, paramsBase);
  applySecondCond(myBdd, paramsBase);
  applyThirdCond(myBdd, paramsBase);
  applyFourthCond(myBdd, paramsBase);

  checkCorrectsValues(myBdd);
  checkValuesToUnique(myBdd);

  size_t satcount = bdd_satcount(myBdd);
  std::cout << "Found " << satcount << " solution\n";
  if (satcount)
  {
    bdd_allsat(myBdd, fun);
  }

  bdd_done();
  return 0;
}
