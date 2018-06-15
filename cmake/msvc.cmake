set(CMAKE_INCLUDE_PREFIX e:/usr/x32/local/)

SET(PASSOA_COMPILER_MSVC TRUE)
add_definitions(-DPLAT_MSVC)

include_directories(${CMAKE_INCLUDE_PREFIX}/include)
LINK_DIRECTORIES (${CMAKE_INCLUDE_PREFIX}/lib)