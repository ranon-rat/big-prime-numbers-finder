#include "big-int.hpp"
#include <cstdint>
std::string BigInts::ToString() const {
  std::string out = "";
  out.reserve(9 * int_parts.size());
  for (size_t i = 0; i < int_parts.size(); ++i) {
    uint32_t number = (int_parts)[i];

    std::string int_conversion = std::to_string(number);
    if (i != int_parts.size() - 1) {
      std::string base0s = "000000000";
      // int_conversion.size:
      base0s.assign(base0s.begin() + int_conversion.size(), base0s.end());
      int_conversion = base0s + int_conversion;
    }
    //  std::cout << "converting the number: " << number
    //            << " conversion: " << number << "\n";
    out = int_conversion + out;
  }
  return (is_negative ? "-" : "") + out;
}

BigInts BigInts::abs() const {
  BigInts out(*this);
  out.is_negative = false;
  return out;
};
uint32_t BigInts::ToSize() const {
  if (int_parts.size() == 0) {
    return 0;
  }
  return int_parts[0];
}