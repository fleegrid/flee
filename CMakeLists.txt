project(flee)
cmake_minimum_required(VERSION 3.2)

# configs
include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wextra -Wpedantic -O3")

# libflee
set(FLEE_LIB_SRCS
  src/tun.c
  src/crypto.c
  src/common.c
  src/client.c
)

add_library(flee-static STATIC ${FLEE_LIB_SRCS})
add_library(flee-shared SHARED ${FLEE_LIB_SRCS})

set_target_properties(flee-static PROPERTIES OUTPUT_NAME "flee")
set_target_properties(flee-shared PROPERTIES OUTPUT_NAME "flee")

target_link_libraries(
  flee-static

  sodium
  event_core
  event_extra
)

target_link_libraries(
  flee-shared

  sodium
  event_core
  event_extra
)

if(UNIX)
  target_link_libraries(flee-static event_pthreads pthread)
  target_link_libraries(flee-shared event_pthreads pthread)
endif()

# flee
SET(FLEE_CLI_SRCS
  src/main.c
)

add_executable(flee-cli ${FLEE_CLI_SRCS})

set_target_properties(flee-cli PROPERTIES OUTPUT_NAME "flee")

target_link_libraries(
  flee-cli

  flee-static
  sodium
  event_core
  event_extra
)

if(UNIX)
  target_link_libraries(flee-cli event_pthreads pthread)
endif()
