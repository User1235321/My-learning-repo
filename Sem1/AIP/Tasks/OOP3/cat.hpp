#ifndef CAT_HPP
#define CAT_HPP
#include "pet.hpp"
namespace rebdev
{
  class Cat: public Pet
  {
    public:
    Cat();
    Cat(char * voice, char * name);

    char * voice();
    char * name();

    private:
    char * name_;
    char * voice_;
  };
}
#endif
