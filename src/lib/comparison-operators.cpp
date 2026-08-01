#include "big-int.hpp"
#include <sys/types.h>

bool BigInts::operator>(const BigInts &other) const {
  // this is going to be a pain in the ass XD
  if (is_negative && !other.is_negative)
    return false;
  if (!is_negative && other.is_negative)
    return true;
  // i should go from bigger to smaller
  bool both_negative = is_negative && other.is_negative;
  if (int_parts.size() > other.int_parts.size()) {
    bool this_larger_size = int_parts.size() > other.int_parts.size();
    return both_negative ? !this_larger_size : this_larger_size;
  }
  for (size_t i = int_parts.size(); i-- > 0;) {
    if (int_parts[i] != other.int_parts[i]) {
      bool this_larger_digit = int_parts[i] > other.int_parts[i];
      return both_negative ? !this_larger_digit : this_larger_digit;
    }
  }
  // equal
  return false;
}

bool BigInts::operator<(const BigInts &other) const { return other > (*this); }
// easy pissy
bool BigInts::operator==(const BigInts &other) const {
  if (other.is_negative != is_negative ||
      other.int_parts.size() != int_parts.size()) {
    return false;
  }
  for (size_t i = 0; i < int_parts.size(); ++i) {
    if (int_parts[i] != other.int_parts[i])
      return false;
  }

  return true;
}

// now lets advance to the rest
bool BigInts::operator>=(const BigInts &other) const {
  return (*this) > other || (*this) == other;
}
bool BigInts::operator<=(const BigInts &other) const {
  return (*this) < other || (*this) == other;
  ;
}