cmake_minimum_required(VERSION 3.21)

project(comparison_between_various_coroutines)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O2")


include(FetchContent)
FetchContent_Declare(
    googleTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
)

FetchContent_Declare(
    googleBenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG master
)

FetchContent_Declare(
    libco
    GIT_REPOSITORY https://github.com/Tencent/libco.git
    GIT_TAG master
)

FetchContent_MakeAvailable(googleTest)
FetchContent_MakeAvailable(googleBenchmark)
FetchContent_MakeAvailable(libco)

set(common_lib 
    benchmark 
    pthread
)


add_executable(boost_fcontext boost_fcontext/benchmark.cpp)
target_link_libraries(boost_fcontext boost_context ${common_lib})

add_executable(boost_ucontext boost_ucontext/benchmark.cpp)
target_link_libraries(boost_ucontext ${common_lib})

add_executable(c++20_stackless c++20_stackless/benchmark.cpp)
target_link_libraries(c++20_stackless ${common_lib})

set(CMAKE_CXX_STANDARD 17)

# Make sure `link_directory` is before call to add_executable
# if you install libco globally, you do not need to specify the link directory manually
# but I choose to move the libcolib.a to the libco/ because it is single and small
# it depends on you
link_directories(libco)
add_executable(libco libco/benchmark.cpp)
target_link_libraries(libco colib dl ${common_lib})


