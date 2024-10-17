#include <iostream>
#include "pet.hpp"
#include "dog.hpp"
#include "cat.hpp"
#include "familyPets.hpp"

int main()
{
  rebdev::Dog dog;
  rebdev::Dog * pDog = &dog;
  rebdev::Cat cat;
  rebdev::Cat * pCat = &cat;
  rebdev::Pet * pPet = nullptr;
  std::cout << pDog->name() << " " << pDog->voice() << '\n';
  std::cout << pCat->name() << " " << pCat->voice() << '\n';

  pPet = &cat;
  std::cout << (*pPet).name() << " " << (*pPet).voice() << '\n';

  pPet = &dog;
  std::cout << (*pPet).name() << " " << (*pPet).voice() << '\n';

  rebdev::FamilyPets family;
  family + pCat;
  family + pDog;

  family.voice();
  family.name();
  return 0;
}
