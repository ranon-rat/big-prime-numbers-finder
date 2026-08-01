
#include <iostream>
#include <list>
#include <numeric>
#include <vector>

struct NumberProperties {
  bool already_visited = false;
};
struct LastVisited {
  uint64_t number{0};
  uint64_t last_visited{0};
};
// now the idea is to do it through a series of batches

struct ErathostonesBatches {

  std::list<LastVisited> primes;
  size_t batch_size{0};

  ErathostonesBatches(size_t batch) : batch_size(batch) {};

  void find_in_batch(uint64_t start_from,
                     size_t batch_to_use) { // lets say we start from 1 then
    // from 100 then from whatever,
    using std::vector;
    vector<uint64_t> numbers(batch_to_use);
    vector<NumberProperties> numbers_states(batch_to_use);
    std::iota(numbers.begin(), numbers.end(), start_from);
    // first of all we need to verify from the beginning
    for (LastVisited &prime : primes) {
      /// this is just to know if we can verify this specific batch with a prime
      /// previously discovered
      if (prime.number + prime.last_visited > start_from + batch_to_use)
        continue;
      // 100-101=>101 oh okay but wha tif 102-101->1 basically this just makes
      // it easier
      for (size_t j{prime.last_visited + prime.number - (start_from)};
           j < numbers.size(); j += prime.number) {
        numbers_states[j].already_visited = true;
        prime.last_visited = start_from + j;
      }
    }

    for (size_t i{0}; i < numbers.size(); i++) {
      if (numbers_states[i].already_visited || numbers[i] <= 1) {
        continue;
      }
      uint64_t current_prime = numbers[i];
      size_t last_visited = start_from + i;
      for (size_t j{i + current_prime}; j < numbers.size();
           j += current_prime) {
        numbers_states[j].already_visited = true;
        last_visited = start_from + j;
      }
      primes.emplace_back(LastVisited{current_prime, last_visited});
    }
  }
  void find_primes(size_t n) {
    for (size_t i{0}; i < n; i += batch_size) {
      size_t batch_to_use = std::min(batch_size, n - i);
      find_in_batch(i + 1, batch_to_use);
      std::cout << "batch " << i << " total primes " << primes.size() << "\n";
    }
    for (auto prime : primes) {
      std::cout << "p:" << prime.number << " l_v: " << prime.last_visited
                << " ";
    }
    std::cout << "\ntotal primes found: " << primes.size() << "\n";
  };
};

int main() {
  ErathostonesBatches eb(100);
  eb.find_primes(1000000);
}