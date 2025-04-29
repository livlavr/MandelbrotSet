cmake_minimum_required(VERSION 3.11)

set(CMAKE_C_STANDARD   17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED   True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ENV{CC}  "gcc")
set(ENV{CXX} "g++")

message(STATUS "C standard ${CMAKE_C_STANDARD}")
message(STATUS "C++ standard ${CMAKE_CXX_STANDARD}")

set(DEBUG_FLAGS -ggdb3 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wno-format-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wno-switch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wno-unused-parameter -Wstack-protector -fsized-deallocation -fstack-protector -fstrict-overflow -fPIC -fno-omit-frame-pointer -fPIE -Werror=vla -Wno-write-strings)
set(RELEASE_FLAGS -O3)

project(MandelbrotSet)

add_subdirectory(${CMAKE_SOURCE_DIR}/Custom-asserts)

add_executable(${PROJECT_NAME}
            src/main.cpp
            src/Mandelbrot.cpp
            src/App.cpp
            src/Tests.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    inc
    ${CMAKE_SOURCE_DIR}/Custom-asserts
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>
)

message(STATUS "CXX Debug Flags: ${DEBUG_FLAGS}")
message(STATUS "CXX Release Flags: ${RELEASE_FLAGS}")

# Ask powerful Maksonchik about it
if(DEFINED ENV{UNROLL_LEVEL})
    set(UNROLL_LEVEL $ENV{UNROLL_LEVEL})
else()
    set(UNROLL_LEVEL 16)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNROLL_LEVEL=${UNROLL_LEVEL}
)

find_package(SFML 3 COMPONENTS Graphics REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    CustomAsserts
    SFML::Graphics
)

add_custom_target(clear
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning build directory: ${CMAKE_BINARY_DIR}"
)
