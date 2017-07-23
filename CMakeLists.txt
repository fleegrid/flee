project(flee)
cmake_minimum_required(VERSION 3.2)

# utils
include(vendor.cmake)

# configs
include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wextra -Wpedantic -O3")
set(CMAKE_DEBUG_POSTFIX _debug)

# libsodium
externalproject_add(
  libsodium
  PREFIX ${VENDOR_PREFIX}
  DOWNLOAD_DIR ${VENDOR_DOWNLOAD_DIR}
  URL https://github.com/jedisct1/libsodium/releases/download/1.0.13/libsodium-1.0.13.tar.gz
  URL_MD5 f38aac160a4bd05f06f743863e54e499
  CONFIGURE_COMMAND ${VENDOR_SRC_DIR}/libsodium/configure --prefix=${VENDOR_PREFIX}
  BUILD_COMMAND make
)

vendor_static_library(sodium libsodium sodium)
vendor_shared_library(sodium libsodium sodium)

# libevent
externalproject_add(
  libevent
  PREFIX ${VENDOR_PREFIX}
  DOWNLOAD_DIR ${VENDOR_DOWNLOAD_DIR}
  URL https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz
  URL_MD5 f3eeaed018542963b7d2416ef1135ecc
  CONFIGURE_COMMAND ${VENDOR_SRC_DIR}/libevent/configure --prefix=${VENDOR_PREFIX} --disable-openssl
  BUILD_COMMAND make
)

vendor_static_library(event-core libevent event_core)
vendor_shared_library(event-core libevent event_core)
vendor_static_library(event-extra libevent event_extra)
vendor_shared_library(event-extra libevent event_extra)

if(UNIX)
  vendor_static_library(event-pthread libevent event_pthreads)
  vendor_shared_library(event-pthread libevent event_pthreads)
endif()

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

  sodium-static
  event-core-static
  event-extra-static
)

target_link_libraries(
  flee-shared

  sodium-shared
  event-core-shared
  event-extra-shared
)

if(UNIX)
  target_link_libraries(flee-static event-pthread-static)
  target_link_libraries(flee-static pthread)
  target_link_libraries(flee-shared event-pthread-shared)
  target_link_libraries(flee-shared pthread)
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
  sodium-static
  event-core-static
  event-extra-static
)

if(UNIX)
  target_link_libraries(flee-cli event-pthread-static)
  target_link_libraries(flee-cli pthread)
endif()
