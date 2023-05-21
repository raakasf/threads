#include <string>

#include "threads/threads.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Name is threads", "[library]")
{
  auto const exported = exported_class {};
  REQUIRE(std::string("threads") == exported.name());
}
