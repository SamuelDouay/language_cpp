# Declare target
add_executable(minesweeper_tests)
set_target_properties(minesweeper_tests PROPERTIES FOLDER tests)

# Get sources and includes
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Add sources
target_sources(
  minesweeper_tests PRIVATE
  ${sources} ${includes}
)

# Pull in minesweeper's sources, excluding its main.cpp
get_target_property(minesweeper_sources minesweeper SOURCES)
list(FILTER minesweeper_sources EXCLUDE REGEX "main\\.cpp$")
target_sources(minesweeper_tests PRIVATE ${minesweeper_sources})

# Add includes
target_include_directories(
  minesweeper_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  $<TARGET_PROPERTY:minesweeper,INCLUDE_DIRECTORIES>
)

# Link dependencies
target_link_libraries(
  minesweeper_tests PRIVATE
  Catch2::Catch2WithMain
)

# Build in C++23
target_compile_features(minesweeper_tests PRIVATE cxx_std_23)

# Add test
add_test(NAME minesweeper COMMAND $<TARGET_FILE:minesweeper_tests>)

# Generate format target
generate_format_target(minesweeper_tests)
