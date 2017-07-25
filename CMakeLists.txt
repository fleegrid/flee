project(flee)
cmake_minimum_required(VERSION 3.2)

# configs
include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

# libflee
set(
  FLEE_LIB_SRCS

  src/tun.c
  src/crypto.c
  src/common.c
)

add_library(flee-static STATIC ${FLEE_LIB_SRCS})
add_library(flee-shared SHARED ${FLEE_LIB_SRCS})

set_target_properties(flee-static PROPERTIES OUTPUT_NAME "flee")
set_target_properties(flee-shared PROPERTIES OUTPUT_NAME "flee")
set_target_properties(flee-static PROPERTIES C_STANDARD 99)
set_target_properties(flee-shared PROPERTIES C_STANDARD 99)

target_link_libraries(flee-static sodium)
target_link_libraries(flee-shared sodium)

# flee/fleed
add_executable(flee-cli src/flee.c)
add_executable(fled-cli src/fled.c)

set_target_properties(flee-cli PROPERTIES OUTPUT_NAME "flee")
set_target_properties(flee-cli PROPERTIES C_STANDARD 99)
set_target_properties(fled-cli PROPERTIES OUTPUT_NAME "fled")
set_target_properties(fled-cli PROPERTIES C_STANDARD 99)

target_link_libraries(
  flee-cli

  flee-static
  sodium
)
target_link_libraries(
  fled-cli

  flee-static
  sodium
  event_core
  event_extra
)

# platform specified links
if(UNIX)
  target_link_libraries(flee-cli pthread)
endif()

if(UNIX)
  target_link_libraries(fled-cli event_pthreads pthread)
  if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(fled-cli nftnl)
  endif()
endif()
