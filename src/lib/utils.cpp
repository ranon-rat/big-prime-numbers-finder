#include "utils.hpp"
#include <chrono>
#include <format>

std::string FormattedNow() {
  auto now = std::chrono::system_clock::now();

  return std::format("{:%Y-%m-%d %H:%M:%S}{}.txt", now, rand());
}

std::vector<std::string> split_str(const std::string &str, char delim) {
  std::vector<std::string> out;
  std::string part = "";
  std::stringstream ss(str);
  while (std::getline(ss, part, delim)) {
    out.emplace_back(part);
  }
  return out;
}
