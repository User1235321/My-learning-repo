#include "familyPets.hpp"
#include <iostream>

rebdev::FamilyPets::FamilyPets():
  maxNumOfPets_(0),
  numOfPets_(0),
  pets_(nullptr)
{};

void rebdev::FamilyPets::voice()
{
  for (size_t i = 0; i < numOfPets_; ++i)
  {
    std::cout << (pets_[i]->voice()) << " ";
  }
  std::cout << '\n';
};
void rebdev::FamilyPets::name()
{
    for (size_t i = 0; i < numOfPets_; ++i)
  {
    std::cout << (pets_[i]->name()) << " ";
  }
  std::cout << '\n';
};
rebdev::FamilyPets * rebdev::FamilyPets::operator+(Pet * newPet)
{
  Pet * * newPets = new Pet *[numOfPets_ + 1];
  for (size_t i = 0; i < numOfPets_; ++i)
  {
    newPets[i] = pets_[i];
  }
  newPets[numOfPets_] = newPet;
  delete[] pets_;
  pets_ = newPets;
  newPets = nullptr;

  numOfPets_ += 1;
  return this;
};
