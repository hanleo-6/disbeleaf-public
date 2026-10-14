cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Disbeleaf LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# output directories: everything (exe + any dlls) goes into bin/, for every config,
# so the game's relative "../assets/..." paths work when run from bin/
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${CONFIG} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${OUTPUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${OUTPUT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${OUTPUT_DIR})
endforeach()

# copy assets
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${OUTPUT_DIR}/assets
    COMMENT "Copying assets")

# ---------------------------------------------------------------------------
# Dependencies
#   - glad, KHR, glm, stb_image: vendored headers in ../Dependencies/include
#   - Dear ImGui: vendored source in vendor/imgui
#   - GLFW, Assimp: use the system package if installed (e.g. Arch:
#     `pacman -S glfw assimp`, Ubuntu: `apt install libglfw3-dev libassimp-dev`),
#     otherwise download and build from source with FetchContent (e.g. Windows).
#     Pass -DDISBELEAF_FETCH_DEPS=ON to always build from source.
# ---------------------------------------------------------------------------
option(DISBELEAF_FETCH_DEPS "Always download GLFW and Assimp instead of using system packages" OFF)

find_package(OpenGL REQUIRED)
include(FetchContent)

if(NOT DISBELEAF_FETCH_DEPS)
    find_package(glfw3 3.3 CONFIG QUIET)
    find_package(assimp CONFIG QUIET)
endif()

if(NOT TARGET glfw)
    message(STATUS "GLFW: not found on system, fetching from source")
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        GIT_SHALLOW    TRUE)
    FetchContent_MakeAvailable(glfw)
else()
    message(STATUS "GLFW: using system package")
endif()

if(NOT TARGET assimp::assimp)
    message(STATUS "Assimp: not found on system, fetching from source")
    # only build what the game needs (glTF/GLB importer), statically, to keep build time down
    set(BUILD_SHARED_LIBS                      OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_TESTS                     OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS              OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_SAMPLES                   OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL                         OFF CACHE BOOL "" FORCE)
    set(ASSIMP_WARNINGS_AS_ERRORS              OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ZLIB                      ON  CACHE BOOL "" FORCE)
    set(ASSIMP_NO_EXPORT                       ON  CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT  OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_GLTF_IMPORTER             ON  CACHE BOOL "" FORCE)
    FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG        v6.0.2
        GIT_SHALLOW    TRUE)
    FetchContent_MakeAvailable(assimp)
    if(NOT TARGET assimp::assimp)
        add_library(assimp::assimp ALIAS assimp)
    endif()
else()
    message(STATUS "Assimp: using system package")
endif()

set(VENDORED_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../Dependencies/include)

add_subdirectory(vendor/imgui)
add_subdirectory(src)
