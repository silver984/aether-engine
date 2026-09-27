add_executable(math-sample src/main.cc)
set_target_properties(
  math-sample PROPERTIES
  OUTPUT_NAME "math"
  RUNTIME_OUTPUT_DIRECTORY ${AETHER_SAMPLES_DIR}
)
target_link_libraries(
  math-sample PRIVATE
  aether-engine::log
  aether-engine::math
)