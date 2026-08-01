#include "channels.hpp"
#include <future>
#include <iostream>
#include <thread>
int main() {

  using namespace std::chrono_literals;
  Channel<int> jobs(5);
  Channel<bool> done;

  auto a = std::async(std::launch::async, [&jobs, &done] {
    while (true) {
      auto [j, more] = jobs.receive_more();
      if (more) {
        std::cout << "received job: " << j << "\n";
        std::this_thread::sleep_for(100ms);

        continue;
      }
      std::cout << "received all jobs\n";
      done.send(true);
      return;
    }
  });

  for (int j = 1; j <= 20; j++) {
    jobs.send(j);
    std::cout << "sent job " << j << "\n";
  }

  jobs.close();
  done.receive_value();
  a.get();
  auto [_, more] = jobs.receive_more();
  std::cout << "received more jobs: " << std::boolalpha << more << "\n";
}
