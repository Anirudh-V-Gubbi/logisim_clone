cmake_minimum_required(VERSION 3.26)
project(Logisim_clone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIGURATION>)

option(ENABLE_LOGGING "Enables logging debug information onto the console" ON)

# Set the level of the logger
if(ENABLE_LOGGING)
    set(LOGGER_LEVEL "0" CACHE STRING "Set the log level: \n0: Disabled\n1: INFO\n2: WARN\n3: ERR\n4: FUNC")
endif()

# Set default configuration to be Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

# Resolve library dependencies
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Create the executable for the project
add_executable(Logisim_clone "Logisim(clone)/main.cpp")

# Configure include search paths
target_include_directories(Logisim_clone PRIVATE "include/" "Logisim(clone)/")

# Add all the source files to the target
file(GLOB_RECURSE HEADER_FILES "Logisim(clone)/*")
target_sources(Logisim_clone PRIVATE ${HEADER_FILES})

# Add MACROS
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Logisim_clone PUBLIC DEBUG)
    if(ENABLE_LOGGING)
    target_compile_definitions(Logisim_clone PUBLIC ENABLE_LOGGER)
    target_compile_definitions(Logisim_clone PUBLIC LOG_LEVEL=${LOGGER_LEVEL})
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(Logisim_clone PUBLIC RELEASE)
endif()
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC
                            RESOURCE_PATH="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/")

# Xcode target properties
set_target_properties(Logisim_clone PROPERTIES 
                      XCODE_GENERATE_SCHEME TRUE
                      XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(Logisim_clone glfw GLEW::glew)

# Copy resources into the final output directory
add_custom_command(TARGET Logisim_clone POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   "\"${CMAKE_SOURCE_DIR}/resources/\""
                   "\"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/\"")
