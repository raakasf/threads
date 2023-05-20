#pragma once

#include <string>
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

const unsigned long k_number_of_threads = 100;

/**
 * @brief The core implementation of the executable
 *
 * This class makes up the library part of the executable, which means that the
 * main logic is implemented here. This kind of separation makes it easy to
 * test the implementation for the executable, because the logic is nicely
 * separated from the command-line logic implemented in the main function.
 */
class library
{
  /**
   * @brief Simply initializes the name member to the name of the project
   */
  public: 
  library();
  ~library();
  void watch(int i);
  void watch_add(const int& i);
  void reaper();

private:
  std::mutex cv_w;
  vector<thread> watches;
  std::thread reaper_thread;
  condition_variable cv;
  std::mutex cv_m;
  queue<int> q;

  std::mutex cv_done;
  vector<bool> done;

  std::string name;
};


class Random
{
  std::mt19937 seeded_eng();
  std::mt19937 eng = seeded_eng();

public:
  auto operator()(int a, int b);
};
