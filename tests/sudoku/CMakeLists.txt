# Declare target
add_executable(sudoku_tests)
set_target_properties(sudoku_tests PROPERTIES FOLDER tests)

# Get sources and includes
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Add sources
target_sources(
  sudoku_tests PRIVATE
  ${sources} ${includes}
)

# Pull in sudoku's sources, excluding its main.cpp
get_target_property(sudoku_sources sudoku SOURCES)
list(FILTER sudoku_sources EXCLUDE REGEX "main\\.cpp$")
target_sources(sudoku_tests PRIVATE ${sudoku_sources})

# Add includes
target_include_directories(
  sudoku_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  $<TARGET_PROPERTY:sudoku,INCLUDE_DIRECTORIES>
)

# Link dependencies
target_link_libraries(
  sudoku_tests PRIVATE
  Catch2::Catch2WithMain
)

# Build in C++23
target_compile_features(sudoku_tests PRIVATE cxx_std_23)

# Add test
add_test(NAME sudoku COMMAND $<TARGET_FILE:sudoku_tests>)

# Generate format target
generate_format_target(sudoku_tests)
