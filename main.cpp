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
  // cout << "watch# " << i << " launched, now going to sleep for" << x << endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(x));
  {
    std::lock_guard<std::mutex> lk(cv_m);
    q.push(i);
  }
  // // cout << "notifying" << endl;
  cv.notify_one();
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // cout << i << endl;
  // std::// cout << "watch thread ending " << i << std::endl;
}

void reaper()
{
  // cout << "reaper thread started" << endl;
  int count = 10;
  while (count--)
  {
    {
      std::unique_lock<std::mutex> lk(cv_m);
      // cout << "waiting for condition, count=" << count << endl;
      cv.wait(lk, []
              { return !q.empty(); });
      // cout << q.front() << endl;
      {
        std::lock_guard<std::mutex> lk(cv_w);
        watches[q.front()].join();
        q.pop();
      }
    }
    // {
    //   std::unique_lock<std::mutex> lk(cv_m);
    //   // cout << "reaper trying to join watch thread# " << idx << endl;
    //   if (watches[idx].joinable())
    //     watches[idx].join();
    //   // cout << "reaper successfully joined watch thread# " << idx << endl;
    // }
  }
  // cout << "reaper thread ending" << endl;
}

int main()
{
  thread r(reaper);
  for (int i = 0; i < 10; i++)
  {
    std::lock_guard<std::mutex> lk(cv_w);
    watches.push_back(thread(watch, i));
  }
  // for (int i = 0; i < 10; i++)
  // {
  // 	if (watches[i].joinable())
  // 	{
  // 		watches[i].join();
  // 	}
  // }
  // // cout << "main thread going to sleep";
  // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  // // cout << "main thread woke up";
  if (r.joinable())
    r.join();
  return 0;
}
