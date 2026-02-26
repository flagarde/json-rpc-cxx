cmake_minimum_required(VERSION 3.12)
project(json-rpc-cxx VERSION 0.0.1 LANGUAGES CXX)


option(COMPILE_TESTS "Enable tests" ON)
option(COMPILE_EXAMPLES "Enable examples" ON)
option(CODE_COVERAGE "Enable coverage reporting" OFF)

include(GNUInstallDirs)

if(NOT TARGET nlohmann_json::nlohmann_json)
find_package(nlohmann_json CONFIG QUIET)
if(NOT nlohmann_json_FOUND)
  include(FetchContent)
  if(NOT DEFINED NLOHMANN_JSON_URL)
    set(NLOHMANN_JSON_URL "https://github.com/nlohmann/json.git")
  endif()
  if(NOT DEFINED NLOHMANN_JSON_VERSION)
    set(NLOHMANN_JSON_VERSION "3.12.0")
  endif()
  FetchContent_Declare(nlohmann_json GIT_REPOSITORY "${NLOHMANN_JSON_URL}" GIT_TAG "v${NLOHMANN_JSON_VERSION}")
  FetchContent_MakeAvailable(nlohmann_json)
endif()
endif()


add_library(json-rpc-cxx INTERFACE)
target_include_directories(json-rpc-cxx INTERFACE include)
target_link_libraries(json-rpc-cxx INTERFACE nlohmann_json::nlohmann_json)
target_compile_features(json-rpc-cxx INTERFACE cxx_std_17)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_library(coverage_config INTERFACE)

# Warning options for the compiler
string(
        APPEND _warning_opts
        "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall;-Wextra;-Weffc++;-Werror;>"
        "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wthread-safety;-Wpedantic;>"
        "$<$<CXX_COMPILER_ID:GNU>:-pedantic;-pedantic-errors;>"
)


if (COMPILE_TESTS)
    if (CODE_COVERAGE)
        message("Enabled coverage flags")
        target_compile_options(coverage_config INTERFACE -O0 -g --coverage)
        target_link_libraries(coverage_config INTERFACE --coverage)
    endif ()
    add_executable(jsonrpccpp-test test/main.cpp test/client.cpp test/typemapper.cpp test/dispatcher.cpp test/server.cpp test/batchclient.cpp test/testclientconnector.hpp examples/warehouse/warehouseapp.cpp test/warehouseapp.cpp test/common.cpp)
    target_compile_options(jsonrpccpp-test PUBLIC "${_warning_opts}")
    target_include_directories(jsonrpccpp-test PRIVATE vendor examples)
    target_link_libraries(jsonrpccpp-test PRIVATE coverage_config json-rpc-cxx)
    enable_testing()
    add_test(NAME test COMMAND jsonrpccpp-test)
endif ()

if (COMPILE_EXAMPLES)
    find_package(Threads)
    add_executable(example-warehouse examples/warehouse/main.cpp examples/warehouse/warehouseapp.cpp examples/warehouse/types.h examples/inmemoryconnector.hpp)
    target_compile_options(example-warehouse PUBLIC "${_warning_opts}")
    target_link_libraries(example-warehouse json-rpc-cxx Threads::Threads)
    target_include_directories(example-warehouse SYSTEM PRIVATE vendor)
    target_include_directories(example-warehouse PRIVATE examples)
    add_test(NAME example COMMAND example-warehouse)
endif ()
