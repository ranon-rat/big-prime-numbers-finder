#include "big-int.hpp"
#include <iostream>
#include <string>
#include <vector>

struct sum_parts {
  std::string a = "";
  std::string b = "";
};
int main() {
  std::vector<sum_parts> testing_numbers = {
      sum_parts{"9", "9"},
      sum_parts{"900900000", "9000080000"},
      sum_parts{"900900000900900000", "900900000900900000"},

  };
  // error with the sum and addition
  for (auto &p : testing_numbers) {
    std::cout << "adding the numbers: " << p.a << " + " << p.b << "\n";
    BigInts a(p.a);
    BigInts b(p.b);
    auto result = a + b;
    std::cout << "result: " << result.ToString() << "\n";
    std::cout << "subtracting the numbers: " << p.a << " + " << p.b << "\n";
    result = a - b;
    std::cout << "result: " << result.ToString() << "\n";
  }

  std::cout << "hello world\n";
}