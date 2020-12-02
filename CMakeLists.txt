cmake_minimum_required(VERSION 3.5)

set (APPLICATION_NAME print_ip)

project(${APPLICATION_NAME} VERSION $ENV{TRAVIS_BUILD_NUMBER})

configure_file(version.h.in version.h)

add_executable(${APPLICATION_NAME} main.cpp)

set_target_properties(${APPLICATION_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(${APPLICATION_NAME}
    PRIVATE "${CMAKE_BINARY_DIR}"
)

FIND_PACKAGE(Doxygen)
if (DOXYGEN_FOUND)
    message(STATUS "Doxygen is found.")

    set(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR})
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)
    set(DOXYGEN_RECURSIVE NO)
    set(DOXYGEN_PROJECT_NAME ${CMAKE_PROJECT_NAME})
    set(DOXYGEN_PROJECT_NUMBER $ENV{TRAVIS_BUILD_NUMBER})
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_HAVE_DOT YES)

    configure_file(doxygen.conf.in doxygen.conf)
    add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf)
else ()
    message(STATUS "Doxygen is not found.")
endif()

if (MSVC)
    target_compile_options(${APPLICATION_NAME} PRIVATE
        /W4
    )
else ()
    target_compile_options(${APPLICATION_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror -O0 -g3
    )
endif()

install(TARGETS ${APPLICATION_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT sacrartorium@example.com)

include(CPack)
