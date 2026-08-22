# Récupère tous les .cpp de tests
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(FILTER TEST_SOURCES EXCLUDE REGEX "cmake-build-")

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests
        PRIVATE
        Catch2::Catch2WithMain
        project_warnings
        ${LIBRARIES_FOR_TESTS}
        stdc++exp
)

include(Catch)
catch_discover_tests(tests)