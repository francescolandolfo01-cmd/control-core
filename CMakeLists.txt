cmake_minimum_required(VERSION 3.22)           # versione minima di CMake richiesta
project(control_core VERSION 0.1.0 LANGUAGES CXX)   # nome del progetto, linguaggio C++

# La libreria: per ora solo header, quindi INTERFACE (niente da compilare)
add_library(control_core INTERFACE)
target_include_directories(control_core INTERFACE include)   # dove stanno gli header
target_compile_features(control_core INTERFACE cxx_std_17)   # chi la usa compila in C++17

# I test, attivi di default
include(CTest)
if(BUILD_TESTING)
  include(FetchContent)                        # scarica GoogleTest alla configurazione
  FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip)
  FetchContent_MakeAvailable(googletest)

  add_executable(test_smoke tests/test_smoke.cpp)            # un eseguibile di test
  target_link_libraries(test_smoke PRIVATE control_core GTest::gtest_main)
  target_compile_options(test_smoke PRIVATE -Wall -Wextra -Wpedantic -Werror)  # warning = errore
  include(GoogleTest)
  gtest_discover_tests(test_smoke)             # registra ogni TEST() con ctest
endif()
