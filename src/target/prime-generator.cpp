
#include "PrimesDB.hpp"
#include "big-int.hpp"
#include "channels.hpp"
#include <cstdint>
#include <future>
#include <iostream>

#include <vector>

// now the idea is to do it through a series of batches

struct ErathostonesBatches {

  uint32_t batch_size{100};

  ErathostonesBatches(size_t batch) : batch_size(batch) {};
  struct ResultBatchSearch {
    size_t total_found{0};
    size_t new_found{0};
  };
  ResultBatchSearch
  find_in_batch(BigInts start_from,
                uint32_t batch_to_use) { // lets say we start from 1 then
    size_t total_found = 0;
    size_t new_found = 0;
    PrimeNumberDB DB(start_from);
    // from 100 then from whatever,
    using std::vector;
    // i am a fucking retard i forgot to fill the shit
    vector<BigInts> numbers(batch_to_use);
    // using the uint8_t because the commitee is fucking retarded and i cannot
    // simply use std::vector because those retards said "oh yeah we should
    // totally use a fucking string of bits to store the bytes, what a great
    // idea "
    // fucking retards
    vector<uint8_t> numbers_states(batch_to_use);

    for (uint32_t i{0}; i < batch_to_use; i++) {
      numbers_states[i] = false;
      numbers[i] = start_from + i;
    }

    // std::iota(numbers.begin(), numbers.end(), start_from);
    // first of all we need to verify from the beginning
    Channel<LastVisited> from_old_channel(batch_size);
    Channel<LastVisited> new_channel(batch_size);
    auto new_thread =
        std::async(std::launch::async, [&start_from, &DB, &new_channel]() { //
          DB.WriteToNew(start_from, new_channel);
        });
    auto from_old_t =
        std::async(std::launch::async, [&DB, &from_old_channel]() {
          std::cout << "starting old thread\n";
          DB.ReadFromOld(from_old_channel);
        });
    while (from_old_channel.expect_more()) {
      from_old_channel.wait_until_values_or_closed();
      if (from_old_channel.is_closed() && from_old_channel.is_empty()) {
        break;
      }
      /// this is just to know if we can verify this specific batch with a
      /// prime previously discovered

      auto prime = from_old_channel.receive_value();
      // only mark multiples when this prime actually lands inside the batch,
      // otherwise we forward it untouched so it isnt lost for future batches
      if (prime.number + prime.last_visited <=
          start_from + BigInts(batch_to_use)) {
        // 100-101=>101 oh okay but wha tif 102-101->1 basically this just
        // makes it easier
        for (BigInts j{prime.last_visited + prime.number - (start_from)};
             j < batch_to_use; j = j + prime.number) {
          numbers_states[j.ToSize()] = true;
          prime.last_visited = start_from + j;
        }
      }
      new_channel.send(prime);
      total_found++;
    }

    for (BigInts i{0}; i < batch_to_use; i = i + 1) {
      if (numbers_states[i.ToSize()] || numbers[i.ToSize()] <= 1) {
        continue;
      }
      BigInts current_prime = numbers[i.ToSize()];
      BigInts last_visited = start_from + i;
      for (BigInts j{i + current_prime}; j < batch_to_use;
           j = j + current_prime) {
        numbers_states[j.ToSize()] = true;
        last_visited = start_from + j;
      }

      new_channel.send(LastVisited{current_prime, last_visited});
      new_found++;
    }

    new_channel.close();
    new_channel.wait_until_emtpy_sender();
    new_thread.get();
    from_old_t.get();
    return ResultBatchSearch{
        .total_found = total_found,
        .new_found = new_found,
    };
  }
  void find_primes(BigInts n) {
    uint32_t last_from = 1;
    for (BigInts i{0}; i < n; i = i + batch_size) {
      uint32_t batch_to_use =
          (BigInts(batch_size) > n - i ? n - 1 : batch_size).ToSize();
      auto result = find_in_batch(i + 1, batch_to_use);
      std::cout << "batch " << i.ToString() << " total primes "
                << result.total_found + result.new_found << "\n";
      if (last_from % 10 == 0) {
        last_from = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
      last_from++;
    }
  };
};

int main() {
  ErathostonesBatches eb(100);
  eb.find_primes(BigInts("100000000"));
}