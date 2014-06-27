#include <concurrency/thread_pool.hpp>

concurrency::ThreadPool::ThreadPool(size_t k, bool g)
  : worker_count(k), workers(), active_workers(0), stop(false), graceful_shutdown(g), active_mutex(), queue_mutex(),
    cond(), queue() {
}

concurrency::ThreadPool::ThreadPool(size_t k) : worker_count(k), workers(), active_workers(0), stop(false),
  graceful_shutdown(false), active_mutex(), queue_mutex(),
  cond(), queue() {
}

concurrency::ThreadPool::ThreadPool() : worker_count(5), workers(), active_workers(0), stop(false),
  graceful_shutdown(false), active_mutex(), queue_mutex(),
  cond(), queue() {
}

void concurrency::ThreadPool::start() {
  init(worker_count);
}

void concurrency::ThreadPool::init(size_t k) {
  for (size_t i = 0; i < k; i++) {
    // creates a thread by using a lambda function
    // this is the actual worker logic
    workers.emplace_back([this, i] {
      while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // wait as long as stop is false and queue is empty
        while (!stop && queue.size() == 0) {
          cond.wait(lock);
        }

        if (stop && graceful_shutdown && queue.size() == 0) {
          break;
        } else if (stop && !graceful_shutdown) {
          break;
        }

        std::function<void()> task(queue.front());
        queue.pop();
        lock.unlock();

        // make a call to the callback which was set in push()
        task();
      }
    });
  }
}

concurrency::ThreadPool::~ThreadPool() {
  if (!stop) {
    shutdown();
  }
}

void::concurrency::ThreadPool::shutdown() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }

  cond.notify_all();

  for (size_t i = 0; i < workers.size(); ++i) {
    if (workers[i].joinable()) {
      workers[i].join();
    }
  }
}
