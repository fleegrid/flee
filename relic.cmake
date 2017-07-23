include(ExternalProject)

# ${ARCH}
execute_process( COMMAND uname -sm COMMAND tr " " "-" COMMAND tr "A-Z" "a-z" COMMAND tr -d '\n' OUTPUT_VARIABLE ARCH)

# relic paths
set(RELIC_ROOT_DIR ${PROJECT_SOURCE_DIR}/deps)
set(RELIC_PREFIX ${RELIC_ROOT_DIR})

set(RELIC_SRC_DIR ${RELIC_ROOT_DIR}/src)
set(RELIC_INCLUDE_DIR ${RELIC_ROOT_DIR}/include)
set(RELIC_LIB_DIR ${RELIC_ROOT_DIR}/lib)

include_directories(${RELIC_INCLUDE_DIR})

macro(relic_static_library TARGET SOURCE LIBNAME)
  add_library(${TARGET}-static STATIC IMPORTED)
  add_dependencies(${TARGET}-static ${SOURCE})
  set_target_properties(${TARGET}-static PROPERTIES IMPORTED_LOCATION ${RELIC_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBNAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
endmacro(relic_static_library)

macro(relic_shared_library TARGET SOURCE LIBNAME)
  add_library(${TARGET}-shared SHARED IMPORTED)
  add_dependencies(${TARGET}-shared ${SOURCE})
  set_target_properties(${TARGET}-shared PROPERTIES IMPORTED_LOCATION ${RELIC_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBNAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
endmacro(relic_shared_library)
