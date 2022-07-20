#include <iomanip>
#include <iostream>
#include <sstream>
#include <numeric>

namespace lia
{
  extern int argc;
  extern char **argv;
  auto RoundMe = [](auto number, auto dPlaces) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(dPlaces) << number;
    ss >> number;
    return number;
  };
} // namespace lia
