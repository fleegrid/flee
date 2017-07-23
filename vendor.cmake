include(ExternalProject)

# ${ARCH}
execute_process(COMMAND uname -sm COMMAND tr " " "-" COMMAND tr "A-Z" "a-z" COMMAND tr -d '\n' OUTPUT_VARIABLE ARCH)

# vendor paths
set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor/${ARCH})
set(VENDOR_DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/vendor/downloads)

set(VENDOR_PREFIX ${VENDOR_DIR})

set(VENDOR_SRC_DIR ${VENDOR_DIR}/src)
set(VENDOR_INCLUDE_DIR ${VENDOR_DIR}/include)
set(VENDOR_LIB_DIR ${VENDOR_DIR}/lib)

# include vendored headers
include_directories(${VENDOR_INCLUDE_DIR})

# messages
message(STATUS "Vendor Information")
message("arch: ${ARCH}")
message("vendor work directory: ${VENDOR_DIR}")
message("vendor download directory: ${VENDOR_DOWNLOAD_DIR}")

macro(vendor_static_library TARGET SOURCE LIBNAME)
  add_library(${TARGET}-static STATIC IMPORTED)
  add_dependencies(${TARGET}-static ${SOURCE})
  set_target_properties(${TARGET}-static PROPERTIES IMPORTED_LOCATION ${VENDOR_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBNAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
endmacro(vendor_static_library)

macro(vendor_shared_library TARGET SOURCE LIBNAME)
  add_library(${TARGET}-shared SHARED IMPORTED)
  add_dependencies(${TARGET}-shared ${SOURCE})
  set_target_properties(${TARGET}-shared PROPERTIES IMPORTED_LOCATION ${VENDOR_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBNAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
endmacro(vendor_shared_library)
