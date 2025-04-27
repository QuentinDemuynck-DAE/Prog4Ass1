#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
public:
    void Push(T item) {
        std::lock_guard<std::mutex> lk(mutex_);
        queue_.push(std::move(item));
        cv_.notify_one();
    }

    // blocks until an item is available or shutdown
    bool Pop(T& out) {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [&] { return !queue_.empty() || stopped_; });
        if (stopped_) return false;
        out = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void Stop() {
        std::lock_guard<std::mutex> lk(mutex_);
        stopped_ = true;
        cv_.notify_all();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopped_ = false;
};
