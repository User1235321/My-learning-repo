#ifndef PET_HPP
#define PET_HPP
namespace rebdev
{
  class Pet
  {
    public:
      virtual char * voice() = 0;
      virtual char * name() = 0;
  };
}
#endif
