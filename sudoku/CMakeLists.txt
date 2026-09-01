# Declare target
add_executable(sudoku)

# Get sources and includes
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# Add sources
target_sources(
  sudoku PRIVATE
  ${sources} ${includes}
)

# Add includes
target_include_directories(
  sudoku PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Build in C++23
target_compile_features(sudoku PRIVATE cxx_std_23)

# Add warning flags
target_add_cxx_warning_flags(sudoku)

# Generate format target
generate_format_target(sudoku)

# Generate tidy target
generate_tidy_target(sudoku)
