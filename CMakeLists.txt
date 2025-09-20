cmake_minimum_required(VERSION 3.12)
project(RGT_ParallelProcessor VERSION 1.0.0 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 컴파일러별 최적화 옵션
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W4")
    add_definitions(-D_WIN32_WINNT=0x0601)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -pthread")
endif()

# 헤더 파일 디렉토리
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# main 실행 파일
add_executable(main src/main.cpp)

# Windows에서 추가 라이브러리 링크
if(WIN32)
    target_link_libraries(main psapi)
endif()

# 스레드 라이브러리 링크
find_package(Threads REQUIRED)
target_link_libraries(main Threads::Threads)

# 디버그 정보 출력
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")