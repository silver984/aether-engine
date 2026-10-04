cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(aether-engine VERSION 1.0 LANGUAGES C CXX)

add_executable(app)

set(AETHER_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(AETHER_OUTPUT_DIR ${AETHER_BINARY_DIR}/engine)

set_target_properties(
  app PROPERTIES
  OUTPUT_NAME "Aether Engine"
  RUNTIME_OUTPUT_DIRECTORY ${AETHER_OUTPUT_DIR}
)

target_include_directories(app PRIVATE include)

file(GLOB AETHER_SOURCE CONFIGURE_DEPENDS src/*.c*)
file(GLOB AETHER_APPLICATION_SOURCE CONFIGURE_DEPENDS src/applications/*.c*)
file(GLOB AETHER_NODE_COMPONENTS_SOURCE CONFIGURE_DEPENDS src/node/components/*.c*)

target_sources(
  app PRIVATE
  src/node/node.cc
  ${AETHER_SOURCE}
  ${AETHER_APPLICATION_SOURCE}
  ${AETHER_NODE_COMPONENTS_SOURCE}
)

add_subdirectory(vendor)

# todo: make functions so this file is cleaner and things are modular

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME raylib
  GITHUB_REPOSITORY raysan5/raylib
  GIT_TAG 6.0
  GIT_SHALLOW TRUE
)

target_compile_definitions(
  raylib PRIVATE
  SUPPORT_MODULE_RAUDIO=0
  SUPPORT_TRACELOG=0
  SUPPORT_SCREEN_CAPTURE=0
  SUPPORT_CAMERA_SYSTEM=0
  SUPPORT_GESTURES_SYSTEM=0
  SUPPORT_RPRAND_GENERATOR=0
  SUPPORT_WINMM_HIGHRES_TIMER=0
  SUPPORT_CLIPBOARD_IMAGE=0
  SUPPORT_FILEFORMAT_OBJ=0
  SUPPORT_FILEFORMAT_MTL=0
  SUPPORT_FILEFORMAT_IQM=0
  SUPPORT_FILEFORMAT_GLTF=0
  SUPPORT_FILEFORMAT_VOX=0
  SUPPORT_FILEFORMAT_M3D=0
  SUPPORT_MESH_GENERATION=0
)

CPMAddPackage(
  NAME imgui
  GITHUB_REPOSITORY ocornut/imgui
  GIT_TAG v1.92.9-docking
  GIT_SHALLOW TRUE
  DOWNLOAD_ONLY TRUE
)

add_library(
  imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

CPMAddPackage(
  NAME rlimgui
  GITHUB_REPOSITORY raylib-extras/rlImGui
  GIT_TAG 1550009359ad975927f7f0e4a3f47e3f27123ea9
  GIT_SHALLOW TRUE
  DOWNLOAD_ONLY TRUE
)

add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
target_link_libraries(
  rlimgui PUBLIC
  raylib
  imgui
)

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  GIT_SHALLOW TRUE
)

target_link_libraries(app PRIVATE rlimgui)

add_subdirectory(modules)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set_target_properties(app PROPERTIES WIN32_EXECUTABLE TRUE)
  if(MSVC)
    target_link_options(app PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_LINKER MATCHES "lld-link|link.exe")
      target_link_options(app PRIVATE -Wl,/subsystem:windows -Wl,/entry:mainCRTStartup)
    else()
      target_link_options(app PRIVATE -mwindows)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(app PRIVATE -mwindows)
  endif()
  target_compile_definitions(app PRIVATE _WINDOWS)
endif()

find_package(Python COMPONENTS Interpreter REQUIRED)

add_custom_command(
  TARGET app POST_BUILD
  COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/resource_packer.py ${CMAKE_SOURCE_DIR}/resources ${AETHER_OUTPUT_DIR}/aether
  VERBATIM
)

if(IS_DIRECTORY ${AETHER_OUTPUT_DIR}/licenses)
  add_custom_command(
    TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${AETHER_OUTPUT_DIR}/licenses
    VERBATIM
  )
endif()

add_custom_command(
  TARGET app POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/licenses ${AETHER_OUTPUT_DIR}/licenses
  VERBATIM
)

add_custom_command(
  TARGET app POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/LICENSE.md ${AETHER_OUTPUT_DIR}/LICENSE.md
  VERBATIM
)

add_custom_command(
  TARGET app POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/THIRD-PARTY-NOTICES.md ${AETHER_OUTPUT_DIR}/THIRD-PARTY-NOTICES.md
  VERBATIM
)

