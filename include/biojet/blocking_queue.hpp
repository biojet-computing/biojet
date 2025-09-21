#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace biojet
{
template<typename T>
class blocking_queue
{
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  std::queue<T> data_;

public:
  void push(T value)
  {
    {
      std::unique_lock<std::mutex> lock{mutex_};
      data_.push(std::move(value));
    }
    condition_.notify_one();
  }

  T pop()
  {
    std::unique_lock<std::mutex> lock{mutex_};
    condition_.wait(lock, [&]{ return !data_.empty(); });
    auto r = std::move(data_.front());
    data_.pop();
    return r;
  }

  template<typename Predicate>
  std::optional<T> pop_unless(Predicate&& predicate)
  {
    std::unique_lock<std::mutex> lock{mutex_};
    condition_.wait(lock, [&] { return std::forward<Predicate>(predicate)() || !data_.empty(); });
    if (data_.empty()) return std::nullopt;
    auto r = std::move(data_.front());
    data_.pop();
    return {r};
  }

  bool empty() const noexcept
  {
    return data_.empty();
  }

  void wake()
  {
    { std::unique_lock<std::mutex> lock{mutex_}; }
    condition_.notify_all();
  }


  blocking_queue() = default;
  ~blocking_queue() = default;
  blocking_queue(blocking_queue&&) noexcept = default;
  blocking_queue& operator=(blocking_queue&&) noexcept = default;
  blocking_queue(const blocking_queue& other) = delete;
  blocking_queue& operator=(const blocking_queue& other) = delete;
};
} // namespace biojet