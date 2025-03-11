#include "myBMPFuncs.hpp"

#include <stdexcept>
#include <vector>
#include <cmath>

#pragma pack(push, 1) // Важно для правильного чтения структур
struct BITMAPFILEHEADER
{
  std::uint16_t bfType; //file type
  std::uint32_t bfSize; //file size
  std::uint16_t bfReserved1;
  std::uint16_t bfReserved2;
  std::uint32_t bfOffBits; //size of BITMAPINFOHEADER and RGBQUADS
};
struct BITMAPINFOHEADER
{
  std::uint32_t biSize; //size of BITMAPINFOHEADER
  std::uint32_t biWidth; //width of image
  std::uint32_t biHeight; //height of image
  std::uint16_t biPlanes; //number of surfaces
  std::uint16_t biBitCount; //numbers of bits to 1 pixel
  std::uint32_t biCompression; //type of compression (BI_RGB, BI_RLE8, BI_RLE4, BI_BITFIELDS)
  std::uint32_t biSizeImage; //size of image
  std::uint32_t biXPelsPerMeter; //width of screen
  std::uint32_t biYPelsPerMeter; //height of screen
  std::uint32_t biClrUsed; //number of color indexes
  std::uint32_t biClrImportant; //number of color indexes
};
#pragma pack(pop)

BITMAPFILEHEADER fillFileHead(int biSizeImage)
{
  BITMAPFILEHEADER fileHead;
  fileHead.bfType = 0x4D42;
  fileHead.bfSize = biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  fileHead.bfReserved1 = 0;
  fileHead.bfReserved2 = 0;
  fileHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  return fileHead;
}
BITMAPINFOHEADER fillInfoHead(int width, int height, int bitsPerPixel)
{
  BITMAPINFOHEADER infoHead;
  infoHead.biSize = sizeof(BITMAPINFOHEADER);
  infoHead.biWidth = width;
  infoHead.biHeight = height;
  infoHead.biPlanes = 1;
  infoHead.biBitCount = bitsPerPixel;
  infoHead.biCompression = 0;
  infoHead.biSizeImage = (width * height * bitsPerPixel / 8);
  infoHead.biXPelsPerMeter = 0;
  infoHead.biYPelsPerMeter = 0;
  infoHead.biClrUsed = 0;
  if ((bitsPerPixel != 16) && (bitsPerPixel != 24) && (bitsPerPixel != 32))
  {
    infoHead.biClrUsed = std::pow(2, bitsPerPixel);
  }
  infoHead.biClrImportant = infoHead.biClrUsed;
  return infoHead;
}

void generate16Bit(const std::string & imageName, int width, int height)
{
  std::ofstream file(imageName, std::ios::binary);
  if (!file)
  {
    throw std::logic_error("Can't open file! file: myBMPFuncs.cpp, func: generate16Bit, line: 68");
  }

  int bitsPerPixel = 16;
  int bytesPerPixel = bitsPerPixel / 8;

  BITMAPFILEHEADER fileHead = fillFileHead(width * height * bytesPerPixel);
  file.write(reinterpret_cast< const char * >(&fileHead), sizeof(BITMAPFILEHEADER));
  BITMAPINFOHEADER infoHead = fillInfoHead(width, height, bitsPerPixel);
  file.write(reinterpret_cast< const char * >(&infoHead), sizeof(BITMAPINFOHEADER));
  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      for (int q = 0; q < bytesPerPixel; ++q)
      {
        file << (char)((i * width + j + q * 50) % 256);
      }
    }
  }
  file.close();
}
void getSizeAndBits(const std::string & imageName, std::ostream & out)
{
  std::ifstream file(imageName, std::ios::binary);
  if (!file.is_open() || !file.good())
  {
    throw std::logic_error("Can't open file! file: myBMPFuncs.cpp, func: getSizeAndBits, line: 95");
  }

  BITMAPFILEHEADER fileHead;
  file.read(reinterpret_cast< char * >(&fileHead), sizeof(fileHead));
  BITMAPINFOHEADER infoHead;
  file.read(reinterpret_cast< char * >(&infoHead), sizeof(infoHead));
  file.close();

  out << "file size: " << fileHead.bfSize << '\n';
  out << "file width: " << infoHead.biWidth << '\n';
  out << "file height: " << infoHead.biHeight << '\n';
  out << "Bits per pixel: " << infoHead.biBitCount << '\n';
}
void binarization(const std::string & inImageName, const std::string & outImageName, int binLevel)
{
  std::ifstream file(inImageName, std::ios::binary);
  std::ofstream binFile(outImageName, std::ios::binary);
  if (!file || !binFile)
  {
    throw std::logic_error("Can't open file! file: myBMPFuncs.cpp, func: binarization, line: 115");
  }

  BITMAPFILEHEADER fileHead;
  file.read(reinterpret_cast< char * >(&fileHead), sizeof(fileHead));
  BITMAPINFOHEADER infoHead;
  file.read(reinterpret_cast< char * >(&infoHead), sizeof(infoHead));
  std::vector< char > imageData(infoHead.biWidth * infoHead.biHeight * (infoHead.biBitCount / 8));
  file.read(reinterpret_cast< char * >(imageData.data()), infoHead.biWidth * infoHead.biHeight * (infoHead.biBitCount / 8));
  file.close();

  BITMAPFILEHEADER binFileHead = fillFileHead(infoHead.biWidth * infoHead.biHeight);
  binFileHead.bfOffBits += 256 * 4;
  binFileHead.bfSize += 256 * 4;
  binFile.write(reinterpret_cast< const char * >(&binFileHead), sizeof(BITMAPFILEHEADER));
  BITMAPINFOHEADER binInfoHead = fillInfoHead(infoHead.biWidth, infoHead.biHeight, 8);
  binFile.write(reinterpret_cast< const char * >(&binInfoHead), sizeof(BITMAPINFOHEADER));
  for (int i = 0; i < 256; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      binFile << (char)(i);
    }
    binFile << (char)(0);
  }
  for (size_t i = 0; i < (infoHead.biWidth * infoHead.biHeight); ++i)
  {
    char grayScale = 0;
    for (size_t j = 0; j < (infoHead.biBitCount / binInfoHead.biBitCount); ++j)
    {
      grayScale += imageData[i * (infoHead.biBitCount / binInfoHead.biBitCount) + j] / (infoHead.biBitCount / binInfoHead.biBitCount);
    }

    binFile << (char)((grayScale > binLevel) ? 255 : 0);
  }
  binFile.close();
}
void convert16To4(const std::string & inImageName, const std::string & outImageName)
{
    std::ifstream inFile(inImageName, std::ios::binary);
    if (!inFile)
    {
      return;
    }

    BITMAPFILEHEADER fileHeader;
    inFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    BITMAPINFOHEADER infoHeader;
    inFile.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (infoHeader.biBitCount != 16) {
        return;
    }

    std::uint32_t pixelDataSize = infoHeader.biWidth * infoHeader.biHeight * 2; // 16-bit = 2 bytes per pixel

    std::vector<std::uint8_t> pixelData(pixelDataSize);
    inFile.read(reinterpret_cast<char*>(pixelData.data()), pixelDataSize);

    inFile.close();

    std::vector<std::uint8_t> colorTable(64);
    for (int i = 0; i < 16; ++i)
    {
        colorTable[i * 4] = i * 16;
        colorTable[i * 4 + 1] = i * 16;
        colorTable[i * 4 + 2] = i * 16;
        colorTable[i * 4 + 3] = 0;
    }
    std::vector<std::uint8_t> newPixelData((infoHeader.biWidth * infoHeader.biHeight + 1) / 2);
    for (std::uint32_t i = 0; i < infoHeader.biWidth * infoHeader.biHeight; ++i)
    {
        std::uint16_t pixel16 = *reinterpret_cast<std::uint16_t*>(&pixelData[i * 2]);
        std::uint8_t pixel4 = (pixel16 >> 12) & 0x0F;
        if (i % 2 == 0) {
            newPixelData[i / 2] = pixel4 << 4;
        } else {
            newPixelData[i / 2] |= pixel4;
        }
    }

    infoHeader.biBitCount = 4;
    infoHeader.biClrUsed = 16;
    infoHeader.biClrImportant = 16;
    infoHeader.biSizeImage = newPixelData.size();

    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTable.size() + newPixelData.size();
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTable.size();

    std::ofstream outFile(outImageName, std::ios::binary);
    if (!outFile)
    {
        return;
    }

    outFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    outFile.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    outFile.write(reinterpret_cast<char*>(colorTable.data()), colorTable.size());
    outFile.write(reinterpret_cast<char*>(newPixelData.data()), newPixelData.size());

    outFile.close();
}
