#include "cat.hpp"

rebdev::Cat::Cat():
  voice_("Miau"),
  name_("Barsik")
{};
rebdev::Cat::Cat(char * voice, char * name):
  voice_(voice),
  name_(name)
{};


char * rebdev::Cat::voice()
{
  return voice_;
};
char * rebdev::Cat::name(){
  return name_;
};
