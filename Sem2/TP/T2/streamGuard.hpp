#ifndef STREAMGUARD_HPP
#define STREAMGUARD_HPP

#include <ios>

namespace rebdev
{
  class StreamGuard
  {
    using cbios = std::basic_ios< char >;

    public:
      StreamGuard(cbios & ios);
      ~StreamGuard();

    private:
      cbios & ios_;
      std::streamsize precision_;
      cbios::fmtflags flags_;
  };
}

#endif
