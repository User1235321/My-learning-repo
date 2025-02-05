#ifndef BMPMANAGER_HPP
#define BMPMANAGER_HPP

#include <fstream>

namespace rebdev
{
  struct BITMAPFILEHEADER
  {
    const unsigned char bfType[2] = {(unsigned char)'B', (unsigned char)'M'}; //file type
    unsigned char bfSize[4]; //file size
    const unsigned char bfReserved1[2] = {0, 0};
    const unsigned char bfReserved2[2] = {0, 0};
    unsigned char BfOffBits[4]; //size of BITMAPINFOHEADER and RGBQUADS
  };
  struct BITMAPINFOHEADER
  {
    unsigned char biSize[4]; //size of BITMAPINFOHEADER
    unsigned char biWidth[4]; //width of image
    unsigned char biHeight[4]; //height of image
    unsigned char biPlanes[2]; //number of surfaces
    unsigned char biBitCount[2]; //numbers of bits to 1 pixel
    unsigned char biCompression[4]; //type of compression (BI_RGB, BI_RLE8, BI_RLE4, BI_BITFIELDS)
    unsigned char biSizeImage[4]; //size of image
    unsigned char biXPelsPerMeter[4]; //width of screen
    unsigned char biYPelsPerMeter[4]; //height of screen
    unsigned char biClrUsed[4]; //number of color indexes
    unsigned char biClrImportant[4]; //number of color indexes
  };
  class bmpManager
  {
    public:
      ~bmpManager();
      void newImage(const BITMAPFILEHEADER & fileHead, const BITMAPINFOHEADER & infoHead, unsigned char * quads, unsigned char * pixels);
      void readFromFile(const std::istream & in);
      void writeToFile(std::ostream & out);
    private:
      BITMAPFILEHEADER fileHead_;
      BITMAPINFOHEADER infoHead_;
      unsigned char * quads_;
      unsigned char * pixels_;
  }
}

#endif
