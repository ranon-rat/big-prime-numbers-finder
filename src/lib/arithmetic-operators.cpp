#include "big-int.hpp"
#include <cstdint>
BigInts BigInts::operator+(const BigInts &b) const {
  const BigInts &a = *this;
  if (a.is_negative && !b.is_negative) {
    return b - a;
  }
  if (!a.is_negative && b.is_negative) {
    return a - b;
  }
  BigInts out;
  out.is_negative = a.is_negative;
  const BigInts *minor, *mayor;
  if (a.abs() < b.abs()) {
    minor = &a;
    mayor = &b;
  } else {
    minor = &b;
    mayor = &a;
  }
  // either both are negative or not xD
  uint32_t carrier = 0;
  for (size_t i = 0; i < mayor->int_parts.size(); ++i) {

    uint32_t a_n = mayor->int_parts[i];
    uint32_t b_n = i < minor->int_parts.size() ? minor->int_parts[i] : 0;
    uint32_t num = a_n + b_n + carrier;
    carrier = 0;
    // carrier is
    if (num > MAX_VALUE_FOR_EACH_BIG_INT_CELL) {
      num -= MAX_VALUE_FOR_EACH_BIG_INT_CELL + 1;
      carrier = 1;
    }
    out.int_parts.emplace_back(num);
  };
  // i can just manage it in this way but the carrier works in a funny way :(
  if (carrier != 0) {
    out.int_parts.emplace_back(carrier);
  }

  // no, this is wrong because it doesnt considers the smaller ones

  return out;
  ;
}
BigInts BigInts::operator-(const BigInts &b) const {
  const BigInts &a = *this;
  if (a.is_negative && !b.is_negative) {
    // -a - b(+) = -(a + b)
    BigInts b_c(b);
    b_c.is_negative = true;
    return a + b_c;
  }
  if (!a.is_negative && b.is_negative) {
    // a - (-b) = a+b
    BigInts b_c(b);
    b_c.is_negative = false;
    return a + b_c;
  }
  BigInts out;
  const BigInts *minor, *mayor;
  if (a.abs() < b.abs()) {
    // a - b será negativo, y |a| < |b|
    mayor = &b;
    minor = &a;
    out.is_negative = true;
  } else {
    // a >= b, resultado no negativo (puede ser cero)
    mayor = &a;
    minor = &b;
    out.is_negative = false;
  }

  uint64_t borrow = 0;

  for (size_t i = 0; i < mayor->int_parts.size(); ++i) {
    uint64_t a_dig = mayor->int_parts[i];
    uint64_t b_dig = (i < minor->int_parts.size()) ? minor->int_parts[i] : 0;

    uint64_t diff = a_dig - b_dig - borrow;

    if (diff > a_dig) {
      diff += MAX_VALUE_FOR_EACH_BIG_INT_CELL;
      borrow = 1;
    } else {
      borrow = 0;
    }
    out.int_parts.push_back(static_cast<uint32_t>(diff));
  }
  // we must delete the 0s
  while (out.int_parts.size() > 1 && out.int_parts.back() == 0) {
    out.int_parts.pop_back();
  }
  // - must not be negative
  if (out.int_parts.size() == 1 && out.int_parts[0] == 0) {
    out.is_negative = false;
  }
  return out;
}
/*
// the multiplications seems the easiest compared to the division :(
BigInts BigInts::operator*(const BigInts &) const {
  BigInts out(0);

  //
  return out;
}
BigInts BigInts::operator/(const BigInts &) const {
  BigInts out(0);

  //
  return out;
}
BigInts BigInts::operator%(const BigInts &) const {
  BigInts out(0);

  //
  return out;
}
*/