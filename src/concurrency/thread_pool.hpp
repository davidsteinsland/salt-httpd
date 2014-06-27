#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <memory>

#include <exceptions.hpp>

namespace concurrency {
  /**
   * A ThreadPool is a manager of workers. It is responsible of starting a specified amount of worker
   * threads, as well as feeding them with work.
   */
  class ThreadPool {
    protected:
      /**
       * The number of workers initiated
       */
      int worker_count;
      
      /**
       * The vector that stores all the workers
       */
      std::vector<std::thread> workers;

      /**
       * The number of active workers, that is the amount of worker threads that are
       * executing a work function
       */
      int active_workers;

      /**
       * Whether or not to stop the workers
       */
      bool stop;

      /**
       * Flag to indicate that whether or not the workers should finish the queue on shutdown or not
       */
      bool graceful_shutdown;

      /**
       * Mutex for active_workers
       */
      std::mutex active_mutex;

      /**
       * Mutex for stop flag and the work queue
       */
      std::mutex queue_mutex;

      /**
       * Condition variable for when the queue is empty
       */
      std::condition_variable cond;

      /**
       * The queue of work functions
       */
      std::queue<std::function<void()>> queue;

    private:
      void init(size_t);

    public:
      /**
       * Creates a new thread pool with a specified amount of workers,
       * and whether or not to shutdown workers right away when shutting down.
       * @param the number of workers
       * @param whether or not to do graceful shutdown
       */
      ThreadPool(size_t, bool);

      /**
       * Creates a new thread pool with a specified amount of workers. Graceful shutdown
       * is set to false.
       * @param k the amount of workers to spawn
       */
      ThreadPool(size_t);

      ThreadPool();

      /**
       * Stops all workers if they havent been stopped yet
       */
      ~ThreadPool();

      /**
       * Starts the thread pool
       */
      void start();

      /**
       * Returns whether or not the worker should quit gracefully
       * @return true if graceful shutdown, false otherwise
       */
      bool gracefulShutdown() {
        return graceful_shutdown;
      }

      /**
       * Shuts down all worker threads.
       */
      void shutdown();

      /**
       * Creates a new task on the queue by supplying a function callback
       * and optional arguments to send to that function.
       * @param f the function
       * @param args the arguments to send to the callback function
       */
      template<class F, class ...Args>
      void push(F&& f, Args&&... args) {
        if (stop) {
          throw ThreadPoolException("push on stopped thread_pool");
        }

        // unpack arguments and bind them to the function
        std::function<void()> task = std::bind(f, args...);

        {
          std::unique_lock<std::mutex> lock(queue_mutex);
          queue.push([this, task] {
            {
              std::lock_guard<std::mutex> lock(this->active_mutex);
              this->active_workers++;
            }

            task();

            {
              std::lock_guard<std::mutex> lock(this->active_mutex);
              this->active_workers--;
            }
          });
        }

        cond.notify_one();
      }

      /**
       * Returns whether or not the workers should quit
       * @return true if the stop flag has been set, false otherwise
       */
      bool shouldStop() {
        return stop;
      }
  };
}
#endif
