set(AETHER_SAMPLES_DIR ${AETHER_BINARY_DIR}/samples)
option(AETHER_BUILD_ALL_MODULE_SAMPLES "Build all module samples" OFF)

add_subdirectory(log)
add_subdirectory(math)
add_subdirectory(ref)
add_subdirectory(util)

target_link_libraries(
  app PRIVATE
  aether-engine::log
  aether-engine::math
  aether-engine::ref
  aether-engine::util
)