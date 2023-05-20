cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    threads
    VERSION 0.1.0
    DESCRIPTION "Learn how to control threads using c++11 or later"
    HOMEPAGE_URL "https://github.com/raakasf/threads"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    threads_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    threads_lib ${warning_guard}
    PUBLIC
    include_directories(build/vcpkg_installed/x64-osx/include)
    include_directories(build/vcpkg_installed/x64-osx/lib)
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(threads_lib PUBLIC cxx_std_17)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(threads_lib PRIVATE fmt::fmt)
target_link_libraries(threads_lib PRIVATE spdlog::spdlog)

# ---- Declare executable ----

add_executable(threads_exe source/main.cpp)
add_executable(threads::exe ALIAS threads_exe)

set_property(TARGET threads_exe PROPERTY OUTPUT_NAME threads)

target_compile_features(threads_exe PRIVATE cxx_std_17)

target_link_libraries(threads_exe PRIVATE threads_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT threads_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of cmake"
  )
endif()

include(cmake/dev-mode.cmake)
