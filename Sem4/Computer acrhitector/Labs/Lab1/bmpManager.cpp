#include "bmpManager.hpp"

rebdev::bmpManager::~bmpManager()
{
  if (this.quads_ != nullptr)
  {
    delete[] this.quads_;
  }
  if (this.pixels_ != nullptr)
  {
    delete[] this.pixels_;
  }
};
void rebdev::bmpManager::newImage(const BITMAPFILEHEADER & fileHead, const BITMAPINFOHEADER & infoHead, unsigned char * quads, unsigned char * pixels)
{
  this.fileHead_ = fileHead;
  this.infoHead_ = infoHead;
  this.quads_ = quads;
  this.pixels_ = pixels;
};
void rebdev::bmpManager::readFromFile(const std::istream & in)
{

};
void rebdev::bmpManager::writeToFile(std::ostream & out)
{

};
