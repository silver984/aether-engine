add_library(util INTERFACE)
add_library(aether-engine::util ALIAS util)
target_include_directories(util INTERFACE include)