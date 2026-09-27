add_library(math STATIC)
add_library(aether-engine::math ALIAS math)

target_include_directories(math PUBLIC include)
target_sources(math PRIVATE src/mat3.cc)

option(AETHER_BUILD_MATH_SAMPLE "Build math sample" OFF)
if (AETHER_BUILD_ALL_MODULE_SAMPLES OR AETHER_BUILD_MATH_SAMPLE)
  add_subdirectory(sample)
endif()