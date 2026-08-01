#ifndef BIG_NUMBER_HPP
#define BIG_NUMBER_HPP
#include <cstdint>
#include <string>
#include <vector>

// 4 294 967 296
#define MAX_VALUE_FOR_EACH_BIG_INT_CELL 999999999 // 1000000000
#define MAX_VALUE_CARRIER_MINUS 1000000000
class BigInts { // we asume that we are working with
private:
  bool is_negative = false;
  std::vector<uint32_t> int_parts = {};
  // we only need this
public:
  BigInts() {};
  BigInts(int32_t number);
  BigInts(uint32_t number);
  BigInts(std::string number);
  BigInts(const BigInts &number);

  //
public:
  std::string ToString() const;
  BigInts abs() const;
  // this will only get the first digits
  uint32_t ToSize() const;

public:
  BigInts &operator=(const BigInts &other);

  // basic arithmetic operations
  BigInts operator+(const BigInts &) const;
  BigInts operator-(const BigInts &) const;
  // this will  be implemented in future implementations, rn its a pain in the
  // ass
  //
  // we could just avoid the multiplier operator for now on
  // BigInts operator*(const BigInts &) const;
  // BigInts operator/(const BigInts &) const;
  // BigInts operator%(const BigInts &) const;
  //  comparison operators
  bool operator>(const BigInts &) const;
  bool operator<(const BigInts &) const;
  bool operator==(const BigInts &) const;
  bool operator>=(const BigInts &) const;
  bool operator<=(const BigInts &) const;
  // bitwise and logical operators i will avoid them for now
  // for them i would imagine that it would be mostly designed for role
  // management or other stuff
  // but for that it doesnt have much sense to manage it in that way due to a
  // simple reason
  // its better to have a specialized class that could manage any of that. this
  // has a specific limit in the way it manages the numbers
};
#endif
