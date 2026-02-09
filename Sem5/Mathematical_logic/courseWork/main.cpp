#include <iostream>
#include <cmath>
#include <bdd.h>

constexpr size_t N = 9, M = 4;
constexpr size_t logN = log2(N) + (log2(N) > (int)log2(N));
size_t getVarIndex(size_t obj, size_t prop, size_t bit)
{
  return obj * M * logN + prop * logN + bit;
}
void initBDD(bdd & myBdd)
{
  for (size_t obj = 0; obj < N; ++obj)
  {
    for (size_t prop = 0; prop < M; ++prop)
    {
      bdd prop_bdd = bddfalse;
      for (size_t val = 0; val < N; ++val)
      {
        bdd val_bdd = bddtrue;
        for (size_t bit = 0; bit < logN; ++bit)
        {
          if ((val >> bit) & 1)
          {
            val_bdd &= bdd_ithvar(getVarIndex(obj, prop, bit));
          }
          else
          {
            val_bdd &= bdd_nithvar(getVarIndex(obj, prop, bit));
          }
        }
        prop_bdd |= val_bdd;
      }
      myBdd &= prop_bdd;
    }
  }
}
void makeUnique(bdd & myBdd)
{
  for (size_t prop = 0; prop < M; ++prop)
  {
    for (size_t obj1 = 0; obj1 < N; ++obj1)
    {
      for (size_t obj2 = obj1 + 1; obj2 < N; ++obj2)
      {
        bdd diff = bddfalse;
        for (size_t bit = 0; bit < logN; ++bit)
        {
          diff |= (bdd_ithvar(getVarIndex(obj1, prop, bit)) ^ bdd_ithvar(getVarIndex(obj2, prop, bit)));
        }
        myBdd &= diff;
      }
    }
  }
}
void addCond(bdd & myBdd, size_t obj, size_t prop, size_t value)
{
  for (size_t bit = 0; bit < logN; ++bit)
  {
    if ((value >> bit) & 1)
    {
      myBdd &= bdd_ithvar(getVarIndex(obj, prop, bit));
    }
    else
    {
      myBdd &= bdd_nithvar(getVarIndex(obj, prop, bit));
    }
  }
}
void firstCond(bdd & myBdd)
{
  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j < 3; ++j)
    {
      addCond(myBdd, i, j, i);
    }
  }
  for (size_t i = 0; i < 3; ++i)
  {
     addCond(myBdd, i, 3, i);
  }
}
void addSecondCond(bdd & myBdd, size_t obj, size_t prop1, size_t value1, size_t prop2, size_t value2)
{
  bdd cond1 = bddtrue;
  addCond(cond1, obj, prop1, value1);
  bdd cond2 = bddtrue;
  addCond(cond2, obj, prop2, value2);
  myBdd &= !(cond1 ^ cond2);
}
void secondCond(bdd & myBdd)
{
  for (size_t obj = 0; obj < N; ++obj)
  {
    addSecondCond(myBdd, obj, 0, 4, 3, 4);
    addSecondCond(myBdd, obj, 0, 5, 3, 5);
    addSecondCond(myBdd, obj, 0, 6, 3, 6);
    addSecondCond(myBdd, obj, 0, 7, 3, 7);
  }
}
void addThirdCond(bdd & myBdd, size_t prop_obj, size_t value_obj, size_t prop_left, size_t value_left)
{
  for (size_t obj = 0; obj < N; ++obj)
  {
    if ((obj % (size_t)(std::sqrt(N))) == 0) continue;
    bdd cond_obj = bddtrue;
    addCond(cond_obj, obj, prop_obj, value_obj);
    bdd cond_left = bddtrue;
    addCond(cond_left, (obj - 1), prop_left, value_left);
    myBdd &= !cond_obj | cond_left;
  }
}
void thirdCond(bdd & myBdd)
{
  addThirdCond(myBdd, 0, 8, 0, 7);
  addThirdCond(myBdd, 0, 5, 0, 4);
  addThirdCond(myBdd, 0, 2, 0, 1);
  addThirdCond(myBdd, 0, 7, 0, 6);
}
void addFourthCond(bdd & myBdd, size_t prop_obj, size_t value_obj, size_t prop_neighbor, size_t value_neighbor, bool vert)
{
  for (size_t obj = 0; obj < N; ++obj)
  {
    size_t row = obj / 3, col = obj % 3;
    bool has_left = (col > 0), has_top_right = (((row > 0) || vert) && col < 2);
    if (!has_left && !has_top_right) continue;
    bdd cond_obj = bddtrue;
    addCond(cond_obj, obj, prop_obj, value_obj);
    bdd neighbor_condition = bddfalse;
    if (has_left)
    {
      bdd cond_left = bddtrue;
      addCond(cond_left, obj - 1, prop_neighbor, value_neighbor);
      neighbor_condition |= cond_left;
    }
    if (has_top_right)
    {
      bdd cond_top_right = bddtrue;
      size_t top_row = (row - 1);
      if (vert)
      {
        top_row = (row == 0) ? 2 : (row - 1);
      }
      addCond(cond_top_right, top_row * 3 + (col + 1), prop_neighbor, value_neighbor);
      neighbor_condition |= cond_top_right;
    }
    myBdd &= !cond_obj | neighbor_condition;
  }
}
void fourthCond(bdd & myBdd)
{
  addFourthCond(myBdd, 0, 8, 0, 7, false);
  addFourthCond(myBdd, 0, 5, 0, 4, false);
  addFourthCond(myBdd, 0, 2, 0, 1, false);
  addFourthCond(myBdd, 0, 7, 0, 6, false);
}
void fourthCondwithVert(bdd & myBdd)
{
  addFourthCond(myBdd, 0, 8, 0, 7, true);
  addFourthCond(myBdd, 0, 5, 0, 4, true);
  addFourthCond(myBdd, 0, 2, 0, 1, true);
  addFourthCond(myBdd, 0, 7, 0, 6, true);
}
void printCombination(bdd & solution)
{
  for (size_t obj = 0; obj < N; ++obj)
  {
    std::cout << "Object " << obj + 1 << ": ";
    for (size_t prop = 0; prop < M; ++prop)
    {
      int val = 0;
      for (size_t bit = 0; bit < logN; ++bit)
      {
        if ((solution & bdd_ithvar(getVarIndex(obj, prop, bit))) != bddfalse)
        {
          val += (1 << bit);
        }
      }
      std::cout << val << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

int main()
{
  int size = 5000000;
  bdd_init(size * 10, size);
  bdd_setvarnum(N * M * logN);
  bdd myCoolBDD = bddtrue;
  firstCond(myCoolBDD);
  secondCond(myCoolBDD);
  thirdCond(myCoolBDD);
  fourthCond(myCoolBDD);
  makeUnique(myCoolBDD);
  initBDD(myCoolBDD);
  int count = 0;
  bdd current = myCoolBDD;
  while (current != bddfalse)
  {
    bdd solution = bdd_satone(current);
    ++count;
    printCombination(solution);
    current &= !solution;
  }
  std::cout << "Total combinations: " << count << "\n";
  bdd_done();
  return 0;
}
