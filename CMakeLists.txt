cmake_minimum_required(VERSION 3.20)
project(language_cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# ---------- Options ----------
option(ENABLE_COVERAGE "Enable code coverage (GCC/Clang only)" ON)

# ---------- Warnings ----------
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Werror)

# ---------- Coverage ----------
add_library(project_coverage INTERFACE)

if (ENABLE_COVERAGE)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(
                project_coverage
                INTERFACE
                --coverage
                -O0
                -g
        )

        target_link_options(
                project_coverage
                INTERFACE
                --coverage
        )
    else ()
        message(WARNING "Coverage requested but compiler is not GCC or Clang.")
    endif ()
endif ()

# ---------- Catch2 ----------
add_subdirectory(externals/Catch2)

# ---------- Macro pour les executables ----------
function(create_project TARGET_NAME SOURCE_DIR)

    # =========================================================
    # Executable
    # =========================================================

    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${SOURCE_DIR}/src/*.cpp)
    file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${SOURCE_DIR}/include/*.hpp)

    if (SOURCES)
        add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

        target_include_directories(${TARGET_NAME} PRIVATE ${SOURCE_DIR}/include)

        target_link_libraries(${TARGET_NAME} PRIVATE project_warnings)
        target_link_libraries(${TARGET_NAME} PRIVATE project_coverage)
        target_link_libraries(${TARGET_NAME} PRIVATE stdc++exp)
    endif ()

    # =========================================================
    # Library for tests
    # =========================================================

    set(TEST_SOURCES ${SOURCES})
    set(MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_DIR}/src/main.cpp)

    if (EXISTS ${MAIN_FILE})
        list(REMOVE_ITEM TEST_SOURCES ${MAIN_FILE})
    endif ()

    if (TEST_SOURCES)
        # Cas normal : il y a des .cpp a compiler (Grid.cpp, MinesweeperGrid.cpp...)
        add_library(${TARGET_NAME}_lib STATIC ${TEST_SOURCES})

        target_include_directories(${TARGET_NAME}_lib PUBLIC ${SOURCE_DIR}/include)
        target_link_libraries(${TARGET_NAME}_lib PUBLIC project_warnings)
        target_link_libraries(${TARGET_NAME}_lib PRIVATE project_coverage)
        target_link_libraries(${TARGET_NAME}_lib PRIVATE stdc++exp)
    else ()
        # Cas header-only (templates, comme MiniVector<T>) : pas de .cpp du tout
        add_library(${TARGET_NAME}_lib INTERFACE)

        target_include_directories(${TARGET_NAME}_lib INTERFACE ${SOURCE_DIR}/include)
        target_link_libraries(${TARGET_NAME}_lib INTERFACE project_warnings)
        target_link_libraries(${TARGET_NAME}_lib INTERFACE project_coverage)
        target_link_libraries(${TARGET_NAME}_lib INTERFACE stdc++exp)
    endif ()

    # =========================================================
    # Tests
    # =========================================================

    file(GLOB_RECURSE PROJECT_TESTS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tests/${TARGET_NAME}/*.cpp)

    if (PROJECT_TESTS)
        add_executable(${TARGET_NAME}_tests ${PROJECT_TESTS})

        target_link_libraries(${TARGET_NAME}_tests PRIVATE
                Catch2::Catch2WithMain
                project_warnings
                ${TARGET_NAME}_lib
                stdc++exp)

        include(Catch)

        catch_discover_tests(${TARGET_NAME}_tests)
    endif ()

endfunction()


# ---------- Création des exécutables ----------

create_project(sudoku sudoku)
create_project(minesweeper minesweeper)
create_project(dynamic dynamic)

# ---------- Tests ----------
enable_testing()