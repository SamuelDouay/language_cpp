cmake_minimum_required(VERSION 3.20)
project(language_cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- Options ----------
option(ENABLE_COVERAGE "Enable code coverage (GCC/Clang only)" ON)

# ---------- Couverture ----------
if (ENABLE_COVERAGE)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)

        add_custom_target(coverage
                COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
                COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage_report/index.html
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating code coverage report with gcovr"
        )
    else ()
        message(WARNING "Coverage requested but compiler is not GCC or Clang.")
    endif ()
endif ()

# ---------- Warnings ----------
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Werror)

# ---------- Catch2 ----------
add_subdirectory(externals/Catch2)

#  ---------- Macro pour les jeux ----------
function(create_executable TARGET_NAME SOURCE_DIR)
    file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/src/*.cpp)
    file(GLOB_RECURSE HEADERS ${SOURCE_DIR}/include/*.hpp)

    add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})
    target_include_directories(${TARGET_NAME} PRIVATE ${SOURCE_DIR}/include)
    target_link_libraries(${TARGET_NAME} PRIVATE project_warnings)
    target_link_libraries(${TARGET_NAME} PRIVATE stdc++exp)

    message(STATUS "${TARGET_NAME} compilé avec : ${SOURCES}")
endfunction()

# ---------- Macro pour les test ----------
function(create_test TARGET_NAME SOURCE_DIR)
    file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/src/*.cpp)

    set(MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_DIR}/src/main.cpp)
    list(REMOVE_ITEM SOURCES ${MAIN_FILE})

    add_library(${TARGET_NAME}_lib STATIC ${SOURCES})
    target_include_directories(${TARGET_NAME}_lib PUBLIC ${SOURCE_DIR}/include)
    target_link_libraries(${TARGET_NAME}_lib PUBLIC project_warnings)
    target_link_libraries(${TARGET_NAME} PRIVATE stdc++exp)

    set(LIBRARIES_FOR_TESTS ${LIBRARIES_FOR_TESTS} ${TARGET_NAME}_lib PARENT_SCOPE)

    message(STATUS "${TARGET_NAME}_lib créée avec : ${SOURCES}")
endfunction()

# ---------- Création des exécutables (jeux) ----------
create_executable(minesweeper minesweeper)
create_executable(sudoku sudoku)

# ---------- Création des bibliothèques pour les tests ----------
create_test(minesweeper minesweeper)
create_test(sudoku sudoku)

# ---------- Tests et call tests\CMakeLists.txt  ----------
enable_testing()
add_subdirectory(tests)