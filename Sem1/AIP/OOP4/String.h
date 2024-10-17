#ifndef STRING_H
#define STRING_H
#include <cstddef>
namespace rebdev
{
  class String
  {
    public:
      String();
      String(const char * str);
      String(const String& str);
      String(String&& str) noexcept;
      ~String ();

      String& operator= (const char* str);
      String& operator= (String&& str) noexcept;
      char& operator[] (size_t pos) const;
      String& append (const String& str);
      String& operator+ (const String& str);
      String& insert (size_t pos, const char* str);
      int compare (const String& str);
    private:
      size_t size_;
      size_t capacity_;
      char * pointer_;
  };
}
#endif
