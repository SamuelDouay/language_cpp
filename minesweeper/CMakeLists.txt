# Declare target
add_executable(minesweeper)

# Get sources and includes
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# Add sources
target_sources(
  minesweeper PRIVATE
  ${sources} ${includes}
)

# Add includes
target_include_directories(
  minesweeper PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Build in C++23
target_compile_features(minesweeper PRIVATE cxx_std_23)

# Add warning flags
target_add_cxx_warning_flags(minesweeper)

# Generate format target
generate_format_target(minesweeper)

# Generate tidy target
generate_tidy_target(minesweeper)
