cmake_minimum_required(VERSION 3.2)


if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(bulk_server VERSION 0.0.1)
endif ()

# Load settings for an external project.
find_package (GTest REQUIRED)
find_package (Threads REQUIRED)
find_package (Boost COMPONENTS system REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


include_directories (
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Copy a file to another location and modify its contents.
configure_file(version.h.in version.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
file (GLOB MAIN_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        )
file (GLOB MAIN_TEST
        ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
        )
file (GLOB MAIN_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*
        )


add_library(bulk_server_lib src/lib.cpp
        src/interpreter.cpp
        src/commandstorage.cpp
        src/lib.cpp
        src/logger.cpp
        src/libdata.cpp)
add_executable(bulk_server
        src/bulk_server.cpp)
# Add an executable to the project using the specified source files.
add_executable(gtest
        test/gtest.cpp
        src/interpreter.cpp
        src/commandstorage.cpp
        src/lib.cpp
        src/libdata.cpp
        src/logger.cpp)


# Set propertiess to a target.
set_target_properties(bulk_server bulk_server_lib gtest PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# Add include directories to a target.
# CMAKE_BINARY_DIR - The path to the top level of the build tree.
target_include_directories(bulk_server PRIVATE "${CMAKE_BINARY_DIR}" ${Boost_INCLUDE_DIRS})
target_include_directories(gtest PRIVATE "${CMAKE_BINARY_DIR}" ${Boost_INCLUDE_DIRS})

# Add include directories to the build.
include_directories(${GTEST_INCLUDE_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set_target_properties(gtest PROPERTIES
    INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(bulk_server PRIVATE bulk_server_lib pthread ${Boost_LIBRARIES})
target_link_libraries(gtest ${GTEST_LIBRARIES} bulk_server_lib pthread ${GTEST_MAIN_LIBRARIES} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# Add compile options to a target.
if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
    target_compile_options(bulk_server_lib PRIVATE
        /W4
    )
    target_compile_options(gtest PRIVATE
        /W4
    )
else ()    
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(bulk_server_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(gtest PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



# Load and run CMake code from a file or module.
install(TARGETS bulk_server RUNTIME DESTINATION bin)

# Set a CMake, cache or environment variable to a given value.
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT martynovich@protei.ru)

include(CPack)

# Enable testing for current directory and below.
enable_testing()

# Add a test to the project to be run by ctest(1).
add_test(gtest gtest)

