#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <tuple>
template <typename T> class Channel {
private:
  std::mutex mtx;

  std::condition_variable data_cv;     // receiving information
  std::condition_variable consumed_cv; // sending information

  bool closed = false;

  std::queue<T> channel_queue;
  size_t max_size = 1;

public:
  Channel() {};
  Channel(size_t max_size) : max_size(max_size) {
    if (max_size == 0)
      throw std::invalid_argument("max_size must be >= 1");
  }
  ~Channel() { close(); }
  void send(const T &value) {

    std::unique_lock lock(mtx);

    consumed_cv.wait(lock,
                     [&] { return closed || channel_queue.size() < max_size; });
    if (closed) {
      throw std::runtime_error("send on closed channel");
    }

    channel_queue.push(value);

    data_cv.notify_one();
  }
  std::tuple<T, bool> receive_more() {
    T value;
    bool more = receive(value);
    return std::make_tuple(value, more);
  };
  T receive_value() {
    T value;
    receive(value);
    return value;
  }

  bool receive(T &out) {
    std::unique_lock lock(mtx);
    data_cv.wait(lock, [&] { return !channel_queue.empty() || closed; });
    if (!channel_queue.empty()) {
      out = channel_queue.front();
      channel_queue.pop();

      consumed_cv.notify_one();
      // i can still consume even when its been closed
      return !channel_queue.empty();
    }
    return false;
  }
  void close() {
    std::unique_lock lock(mtx);
    closed = true;
    consumed_cv.notify_all();
    data_cv.notify_all();
  }
  bool is_closed() {
    std::unique_lock lock(mtx);
    return closed;
  }
  bool expect_more() {
    std::unique_lock lock(mtx);
    return !channel_queue.empty() || !closed;
  }
  void wait_until_finished_sender() {
    std::unique_lock lock(mtx);
    consumed_cv.wait(lock,
                     [&]() -> bool { return channel_queue.empty() || closed; });
  }
  void wait_until_finished_receiver() {
    std::unique_lock lock(mtx);
    data_cv.wait(lock,
                 [&]() -> bool { return channel_queue.empty() || closed; });
  }
  // this should be used for the consumer
  void wait_until_emtpy_sender() {
    std::unique_lock lock(mtx);
    consumed_cv.wait(lock, [&]() -> bool { return channel_queue.empty(); });
  }
  void wait_until_empty_receiver() {
    std::unique_lock lock(mtx);
    data_cv.wait(lock, [&]() -> bool { return channel_queue.empty(); });
  }
  void wait_until_values_or_closed() {
    std::unique_lock lock(mtx);
    data_cv.wait(lock,
                 [&]() -> bool { return !channel_queue.empty() || closed; });
  }
  void wait_until_values() {
    std::unique_lock lock(mtx);
    data_cv.wait(lock, [&]() -> bool { return !channel_queue.empty(); });
  }
  bool is_empty() {
    std::unique_lock lock(mtx);

    return channel_queue.empty();
  }
};
