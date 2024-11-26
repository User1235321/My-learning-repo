#ifndef DOG_HPP
#define DOG_HPP
#include "pet.hpp"
namespace rebdev
{
  class Dog: public Pet
  {
    public:
    Dog();
    Dog(char * voice, char * name);

    char * voice();
    char * name();

    private:
    char * name_;
    char * voice_;
  };
}
#endif
