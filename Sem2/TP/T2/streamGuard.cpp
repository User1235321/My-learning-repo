#include "streamGuard.hpp"

rebdev::StreamGuard::StreamGuard(cbios & ios):
  ios_(ios),
  precision_(ios.precision()),
  flags_(ios.flags())
{}

rebdev::StreamGuard::~StreamGuard()
{
  ios_.precision(precision_);
  ios_.flags(flags_);
}
