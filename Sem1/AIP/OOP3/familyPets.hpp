#ifndef FAMILYPETS_HPP
#define FAMILYPETS_HPP
#include "pet.hpp"
#include <cstddef>
namespace rebdev
{
  class FamilyPets
  {
    public:
      FamilyPets();
      void voice();
      void name();
      FamilyPets * operator+(Pet * newPet);
    private:
      size_t maxNumOfPets_;
      size_t numOfPets_;
      Pet * * pets_;
  };
}
#endif
