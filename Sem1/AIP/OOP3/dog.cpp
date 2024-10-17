#include "dog.hpp"

rebdev::Dog::Dog():
  voice_("Gav"),
  name_("Polkan")
{};
rebdev::Dog::Dog(char * voice, char * name):
  voice_(voice),
  name_(name)
{};


char * rebdev::Dog::voice()
{
  return voice_;
};
char * rebdev::Dog::name(){
  return name_;
};
