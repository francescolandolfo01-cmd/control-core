cmake_minimum_required(VERSION 3.22)           # versione minima di CMake richiesta
project(control_core VERSION 0.1.0 LANGUAGES CXX)   # nome del progetto, linguaggio C++
find_package(Eigen3 3.4 REQUIRED NO_MODULE)

# La libreria: per ora solo header, quindi INTERFACE (niente da compilare)
add_library(control_core INTERFACE)
target_include_directories(control_core INTERFACE include)   # dove stanno gli header
target_compile_features(control_core INTERFACE cxx_std_17)   # chi la usa compila in C++17
target_link_libraries(control_core INTERFACE Eigen3::Eigen)

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
  add_executable(test_first_order_zoh tests/test_first_order_zoh.cpp)            # un eseguibile di test
  target_link_libraries(test_first_order_zoh PRIVATE control_core GTest::gtest_main)
  target_compile_options(test_first_order_zoh PRIVATE -Wall -Wextra -Wpedantic -Werror)  # warning = errore
  gtest_discover_tests(test_first_order_zoh)             # registra ogni TEST() con ctest
  add_executable(test_state_space tests/test_state_space.cpp)
  target_link_libraries(test_state_space PRIVATE control_core GTest::gtest_main)
  target_compile_options(test_state_space PRIVATE -Wall -Wextra -Wpedantic -Werror)
  gtest_discover_tests(test_state_space)
  add_executable(test_rk4 tests/test_rk4.cpp)
  target_link_libraries(test_rk4 PRIVATE control_core GTest::gtest_main)
  target_compile_options(test_rk4 PRIVATE -Wall -Wextra -Wpedantic -Werror)
  gtest_discover_tests(test_rk4)
  add_executable(test_cart_pole tests/test_cart_pole.cpp)
  target_link_libraries(test_cart_pole PRIVATE control_core GTest::gtest_main)
  target_compile_options(test_cart_pole PRIVATE -Wall -Wextra -Wpedantic -Werror)
  gtest_discover_tests(test_cart_pole)
  add_executable(test_dlqr tests/test_dlqr.cpp)
  target_link_libraries(test_dlqr PRIVATE control_core GTest::gtest_main)
  target_compile_options(test_dlqr PRIVATE -Wall -Wextra -Wpedantic -Werror)
  gtest_discover_tests(test_dlqr)
  add_executable(test_kalman tests/test_kalman.cpp)
  target_link_libraries(test_kalman PRIVATE control_core GTest::gtest_main)
  target_compile_options(test_kalman PRIVATE -Wall -Wextra -Wpedantic -Werror)
  gtest_discover_tests(test_kalman)
endif()
