#ifndef PRIME_DB_HPP
#define PRIME_DB_HPP

#include "big-int.hpp"
#include "channels.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
namespace fs = std::filesystem;

struct LastVisited {
  BigInts number{0};
  BigInts last_visited{0};
};
class PrimeNumberDB {

  fs::path directory = "./primes";
  fs::path old_file_p = "";
  fs::path new_file_p = "";

public:
  PrimeNumberDB(BigInts current_batch) {
    fs::create_directories(directory);

    for (const auto &entry : fs::directory_iterator(directory)) {
      old_file_p = entry;
      break;
    }
    new_file_p = directory / FormattedNow();
  }

  // something thats preferable is for the program to say if it should stop at
  // specific positions
  ~PrimeNumberDB() {
    if (old_file_p == "")
      return;
    fs::remove(old_file_p);
  }
  // i should first verify i
  void ReadFromOld(Channel<LastVisited> &numbers) {
    if (old_file_p == "") {

      numbers.close();
      return;
    }
    std::ifstream old_file_f(old_file_p);
    try {
      if (!old_file_f.is_open())
        numbers.close();
      std::string line = "";
      std::getline(old_file_f, line);

      // the first line is important

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
    } catch (const std::runtime_error &err) {
    }
    numbers.wait_until_emtpy_sender();
    numbers.close();
  }
  void WriteToNew(BigInts batch, Channel<LastVisited> &numbers) {
    std::ofstream new_file_f(new_file_p, std::ios::app);
    if (!new_file_f.is_open()) {
      throw std::runtime_error("something weird is happening");
    }

    try {
      new_file_f << batch.ToString() << "\n";
      // lets flush every 10 lines
      size_t since_flushing = 1;

      while (numbers.expect_more()) {
        numbers.wait_until_values_or_closed();
        if (numbers.is_closed() && numbers.is_empty()) {
          break;
        }
        LastVisited v = numbers.receive_value();
        new_file_f << v.number.ToString() << ":" << v.last_visited.ToString()
                   << "\n";
        if (since_flushing % 10 == 0) {
          since_flushing = 0;
          new_file_f.flush();
        }
        since_flushing++;
      }
      new_file_f.flush();
    } catch (const std::runtime_error &e) {
    }
    if (new_file_f.is_open())
      new_file_f.close();
  }
};

#endif