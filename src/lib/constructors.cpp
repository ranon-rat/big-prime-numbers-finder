#include "big-int.hpp"
#include <stdexcept>

// not much to say
BigInts::BigInts(uint32_t number) { int_parts.emplace_back(number); }
// simple conversion of ints
BigInts::BigInts(int32_t number) {
  is_negative = number < 0;
  if (is_negative) // if is negative we only need to do this so we dont have any
                   // weird problem
    number *= -1;
  int_parts.emplace_back(number);
}
// now for the most complicated and annoying part
// we assume that the string is completely made of integers, if there is
// something weird you are a retard xD
BigInts::BigInts(std::string number) {
  // for this we only need to check something simple
  int i = number.size() - 1;
  uint64_t power = 1;
  uint32_t current_number = 0;
  // n or -

  for (; i >= 0; --i) {
    char digit_char = number[i];

    //
    if ((digit_char < '0' || digit_char > '9')) {
      if (digit_char == '-' && i == 0) {
        is_negative = true;
        break;
      }
      // You should kill yourself
      throw std::runtime_error(
          "non valid symbol inside a string that must represent any integer");
    }
    const uint32_t digit = (digit_char - '0');
    if (digit > (MAX_VALUE_FOR_EACH_BIG_INT_CELL / power) ||
        (digit * power) > (MAX_VALUE_FOR_EACH_BIG_INT_CELL - current_number)) {

      int_parts.emplace_back(current_number);
      current_number = 0;
      power = 1;
    }

    current_number += digit * power;
    power *= 10;
    if (power > MAX_VALUE_FOR_EACH_BIG_INT_CELL) {
      int_parts.emplace_back(current_number);
      current_number = 0;
      power = 1;
    }
  }
  if (current_number != 0) {
    int_parts.emplace_back(current_number);
  }
}

BigInts::BigInts(const BigInts &number) {
  is_negative = number.is_negative;
  int_parts.assign(number.int_parts.begin(), number.int_parts.end());
}

BigInts &BigInts::operator=(const BigInts &number) {
  is_negative = number.is_negative;
  int_parts.assign(number.int_parts.begin(), number.int_parts.end());
  return *this;
}