#include "model.hpp"

#include <stdexcept>

namespace lab1
{

Model make_model(const std::string & name, double left, double right)
{
  Model model;
  model.k = [](double) { return 1.0; };
  model.q = [](double) { return 0.0; };
  if (name == "zero_error_test")
  {
    model.q = [](double) { return 1.0; };
    model.f = [](double) { return 1.0; };
    model.exact = [](double) { return 1.0; };
  }
  else if (name == "nonzero_error_test")
  {
    model.f = [](double r) { return -9.0 * r; };
    model.exact = [](double r) { return r * r * r; };
    model.nu1 = -3.0 * left * left;
    model.chi2 = 1.0;
    model.nu2 = right * right * right + 3.0 * right * right;
  }
  else
  {
    throw std::invalid_argument("Unknown model: " + name);
  }
  return model;
}

}
