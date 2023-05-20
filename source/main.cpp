#include "lib.hpp"

#include <thread>

int main()
{
  library lib;

  for (auto i = 0; i < k_number_of_threads; i++)
  {
    lib.watch_add(i);
  }

  return 0;
}
