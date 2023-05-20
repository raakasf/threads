#include <thread>
#include <chrono>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <spdlog/spdlog.h>

using namespace std;

std::mt19937 seeded_eng()
{
  std::random_device r;
  std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
  return std::mt19937(seed);
}

class Random
{
  std::mt19937 eng = seeded_eng();

public:
  auto operator()(int a, int b)
  {
    std::uniform_int_distribution<int> dist(a, b);
    return dist(eng);
  }
};

std::mutex cv_w;
vector<thread> watches;

condition_variable cv;
std::mutex cv_m;
queue<int> q;

std::mutex cv_done;
vector<bool> done(10, false);


void watch(int i)
{
  Random random;
  auto x = random(1, 9);
  SPDLOG_INFO("watch# {} launched, now going to sleep for {}", i, x);
  std::this_thread::sleep_for(std::chrono::milliseconds(x));
  {
    std::lock_guard<std::mutex> lk(cv_m);
    q.push(i);
  }
  SPDLOG_INFO("notifying");
  cv.notify_one();
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  SPDLOG_INFO(i);
  // std::SPDLOG_INFO("watch thread ending " << i << std::endl;
}

void reaper()
{
  SPDLOG_INFO("reaper thread started");
  int count = 10;
  while (count--)
  {
    {
      std::unique_lock<std::mutex> lk(cv_m);
      SPDLOG_INFO("waiting for condition, count={}", count);
      cv.wait(lk, []
              { return !q.empty(); });
      SPDLOG_INFO(q.front());
      {
        std::lock_guard<std::mutex> lk(cv_w);
        watches[q.front()].join();
        q.pop();
      }
    }
  }
  SPDLOG_INFO("reaper thread ending");
}

int main()
{
  thread reaper_thread(reaper);
  for (int i = 0; i < 10; i++)
  {
    std::lock_guard<std::mutex> lk(cv_w);
    watches.push_back(thread(watch, i));
  }
  if (reaper_thread.joinable())
    reaper_thread.join();
  return 0;
}
