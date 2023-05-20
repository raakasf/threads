#include "lib.hpp"

#include <fmt/core.h>

library::library()
    : name{fmt::format("{}", "cmake")}
{
    done.reserve(k_number_of_threads);
    reaper_thread = std::thread(&library::reaper, this);
    // std::fill(done.begin(), done.end(), false);
}

library::~library()
{
    SPDLOG_INFO("library destructor");
    if (reaper_thread.joinable())
        reaper_thread.join();
}
std::mt19937 Random::seeded_eng()
{
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    return std::mt19937(seed);
}

auto Random::operator()(int a, int b)
{
    std::uniform_int_distribution<int> dist(a, b);
    return dist(eng);
}

void library::watch(int i)
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

void library::watch_add(const int &i)
{
    std::lock_guard<std::mutex> lk(cv_w);
    watches.push_back(
        std::thread(&library::watch, this, i));
}

void library::reaper()
{
    SPDLOG_INFO("reaper thread started");
    auto count = k_number_of_threads;
    while (count--)
    {
        {
            std::unique_lock<std::mutex> lk(cv_m);
            // SPDLOG_INFO("waiting for condition, count={}", count);
            cv.wait(lk, [&]
                    { return !q.empty(); });
            SPDLOG_INFO(q.front());
            {
                std::lock_guard<std::mutex> lk_w(cv_w);
                watches[q.front()].join();
                q.pop();
            }
        }
    }
    SPDLOG_INFO("reaper thread ending");
}
