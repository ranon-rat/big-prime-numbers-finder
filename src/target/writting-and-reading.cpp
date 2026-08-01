#include "big-int.hpp"
#include "channels.hpp"
#include <any>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

std::string FormattedNow() {
  auto now = std::chrono::system_clock::now();

  return std::format("{:%Y-%m-%d %H:%M:%S}{}.txt", now, rand());
}
struct LastVisited {
  BigInts number{0};
  BigInts last_visited{0};
};
std::vector<std::string> split_str(const std::string &str, char delim) {
  std::vector<std::string> out;
  std::string part = "";
  std::stringstream ss(str);
  while (std::getline(ss, part, delim)) {
    out.emplace_back(part);
  }
  return out;
}
class PrimeNumberDB {

  fs::path directory = "./primes";
  fs::path old_file_p = "";
  fs::path new_file_p = "";
  PrimeNumberDB(BigInts current_batch) {
    for (const auto &entry : fs::directory_iterator(directory)) {
      old_file_p = entry;
    }
    new_file_p = directory / FormattedNow();
  }

  // something thats preferable is for the program to say if it should stop at
  // specific positions
  ~PrimeNumberDB() {
    if (old_file_p == "") {
      return;
    }

    fs::remove(old_file_p);
  }
  // i should first verify i
  void ReadFromOld(Channel<LastVisited> &numbers) {
    if (old_file_p == "")
      numbers.close();

    std::ifstream old_file_f(old_file_p);
    try {
      if (!old_file_f.is_open())
        numbers.close();
      std::string line = "";
      std::getline(old_file_f, line);
      // the first line is important
      std::cout << "from batch: " << line << "\n";

      while (std::getline(old_file_f, line)) {
        auto parts = split_str(line, ':');
        if (parts.size() < 2) {
          throw std::runtime_error("weird parsing");
        }
        LastVisited lv = {
            .number = BigInts(parts[0]),
            .last_visited = BigInts(parts[1]),
        };
        numbers.send(lv);
      }
    } catch (std::any err) {
    }
    numbers.close();
  }
  void WriteToNew(Channel<LastVisited> &numbers) {
    std::ofstream new_file_f(new_file_p, std::ios::app);
    if (!new_file_f.is_open()) {
      throw std::runtime_error("something weird is happening");
    }
    try {
      // lets flush every 10 lines
      size_t since_flushing = 1;
      do {
        LastVisited v = numbers.receive_value();
        new_file_f << v.number.ToString() << ":" << v.last_visited.ToString()
                   << "\n";
        if (since_flushing % 10 == 0) {
          since_flushing = 0;
          new_file_f.flush();
        }
        since_flushing++;

      } while (!numbers.expect_more());
      new_file_f.flush();
    } catch (std::any e) {
      if (new_file_f.is_open())
        new_file_f.close();
    }
  }
};
// lets think on what we have to make here
// it shouldnt be complicated
int main() {
  /*FIle that i do be expecting
<GENERATED_FROM_BATCH>
<NUMBER>:<LAST_KNOWN_POSITION>
.
.
.
.
.
.
.
*/
}