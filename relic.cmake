include(ExternalProject)

set(RELIC_ROOT_DIR ${PROJECT_SOURCE_DIR}/deps)

set(RELIC_SRC_DIR ${RELIC_ROOT_DIR}/src)
set(RELIC_INCLUDE_DIR ${RELIC_ROOT_DIR}/include)

set(RELIC_PREFIX ${RELIC_ROOT_DIR})

include_directories(${RELIC_INCLUDE_DIR})

macro(relic_add PROJ URL MD5 CONFIG_COMMAND)
  externalproject_add(
    ${PROJ}
    PREFIX ${PROJECT_SOURCE_DIR}/deps
    URL ${URL}
    URL_MD5 ${MD5}
    BUILD_COMMAND make
    CONFIGURE_COMMAND ${CONFIG_COMMAND}
  )
endmacro(relic_add)

macro(relic_static_library TARGET SOURCE LIBNAME)
  add_library(${TARGET}-static STATIC IMPORTED)
  add_dependencies(${TARGET}-static ${SOURCE})
  set_target_properties(${TARGET}-static PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/deps/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBNAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
endmacro(relic_static_library)

macro(relic_shared_library TARGET SOURCE LIBNAME)
  add_library(${TARGET}-shared SHARED IMPORTED)
  add_dependencies(${TARGET}-shared ${SOURCE})
  set_target_properties(${TARGET}-shared PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/deps/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBNAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
endmacro(relic_shared_library)
