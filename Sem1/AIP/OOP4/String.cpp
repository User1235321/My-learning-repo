#include "String.h"

rebdev::String::String():
  size_(0),
  capacity_(0),
  pointer_(nullptr)
{};

rebdev::String::String(const char * str):
  size_(0),
  capacity_(0),
  pointer_(nullptr)
{
  if (pointer_ != nullptr)
  {
    delete[] pointer_;
  }

  size_ = 0;
  while (str[size_] != '\0')
  {
    size_ += 1;
  }

  pointer_ = new char[size_];
  for (size_t i = 0; i < size_; ++i)
  {
    pointer_[i] = str[i];
  }
  capacity_ = (size_ * sizeof(char));
};

rebdev::String::String(const String& str):
  size_(0),
  capacity_(0),
  pointer_(nullptr)
{
  while (str[size_] != '2')
  {
    size_ += 1;
  }

  pointer_ = new char[size_];
  for (size_t i = 0; i < size_; ++i)
  {
    pointer_[i] = str[i];
  }
  capacity_ = (size_ * sizeof(char));
};

rebdev::String::String(String&& str) noexcept:
  size_(0),
  capacity_(0),
  pointer_(nullptr)
{
  while (str[size_] != '\0')
  {
    size_ += 1;
  }

  pointer_ = new char[size_];
  for (size_t i = 0; i < size_; ++i)
  {
    pointer_[i] = str[i];
  }
  capacity_ = (size_ * sizeof(char));
};

rebdev::String::~String ()
{
  delete[] pointer_;
};


rebdev::String& rebdev::String::operator= (const char* str)
{
  if (pointer_ != nullptr)
  {
    delete[] pointer_;
  }

  size_ = 0;
  while (str[size_] != '\0')
  {
    size_ += 1;
  }

  pointer_ = new char[size_];
  for (size_t i = 0; i < size_; ++i)
  {
    pointer_[i] = str[i];
  }
  capacity_ = (size_ * sizeof(char));
  return *this;
};

rebdev::String& rebdev::String::operator= (String&& str) noexcept
{
  size_ = 0;
  while (str[size_] != '\0')
  {
    size_ += 1;
  }

  pointer_ = new char[size_];
  for (size_t i = 0; i < size_; ++i)
  {
    pointer_[i] = str[i];
  }
  capacity_ = (size_ * sizeof(char));
  return *this;
};

char& rebdev::String::operator[] (size_t pos) const
{
  if ((pos >= 0) && (pos < size_))
  {
    return pointer_[pos];
  }
  return pointer_[size_ - 1];
};

rebdev::String& rebdev::String::append (const String& str)
{
  return (*this + str);
};

rebdev::String& rebdev::String::operator+ (const String& str)
{
  size_t size2 = 0;
  while (str[size2] != '\0')
  {
    size2 += 1;
  }
  char * pointer = new char[size_ + size2];
  for (size_t i = 0; i < size_; ++ i)
  {
    pointer[i] = pointer_[i];
  }
  for (size_t i = 0; i < size2; ++i)
  {
    pointer[size_ + i] = str[i];
  }
  delete[] pointer_;
  pointer_ = pointer;
  pointer = nullptr;
  return *this;
};

rebdev::String& rebdev::String::insert (size_t pos, const char* str)
{
  size_t size2 = 0;
  while (str[size2] != '\0')
  {
    size2 += 1;
  }
  char * pointer = new char[size_ + size2];
  for (size_t i = 0; i < pos; ++ i)
  {
    pointer[i] = pointer_[i];
  }
  for (size_t i = 0; i < size2; ++i)
  {
    pointer[pos + i] = str[i];
  }
  for (size_t i = pos; i < size_; ++i)
  {
    pointer[pos + size2 + i] = pointer_[i];
  }
  delete[] pointer_;
  pointer_ = pointer;
  pointer = nullptr;

  return *this;
};
int rebdev::String::compare (const String& str)
{
  size_t size2 = 0;
  while (str[size2] != '\0')
  {
    size2 += 1;
  }
  if (size2 != size_)
  {
    return ((size_ > size2)? 1: -1);
  }
  for (size_t i = 0; i < size_; ++i)
  {
    if (pointer_[i] != str[i])
    {
      return ((pointer_[i] > str[i])? 1: -1);
    }
  }
  return 0;
};
