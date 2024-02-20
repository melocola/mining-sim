cmake_minimum_required(VERSION 3.10)
project(mining-sim)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Adding executable paths and include direcotries
add_executable(mining-sim src/main.cpp src/utils/tickhandler.cpp src/assets/miner.cpp src/assets/station.cpp src/utils/metricshandler.cpp src/utils/minermanager.cpp src/utils/stationmanager.cpp)

target_include_directories(mining-sim PRIVATE ${PROJECT_SOURCE_DIR}/src/utils ${PROJECT_SOURCE_DIR}/src/assets)

include(FetchContent)

# Json integration
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.9.1  # Specify the version you want to use
)

FetchContent_MakeAvailable(json)

# Now link against the nlohmann_json::nlohmann_json target as needed
target_link_libraries(mining-sim PRIVATE nlohmann_json::nlohmann_json)

# GoogleTest integration
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.11.0.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Testing setup
enable_testing()
add_executable(mining_sim_tests src/tests/miner_test.cpp)
target_link_libraries(mining_sim_tests gtest_main)
target_include_directories(mining_sim_tests PRIVATE ${PROJECT_SOURCE_DIR}/src/utils ${PROJECT_SOURCE_DIR}/src/assets ${gtest_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(mining_sim_tests)