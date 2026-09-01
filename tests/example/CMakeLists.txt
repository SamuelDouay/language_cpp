# Declare target
add_executable(example_tests)
set_target_properties(example_tests PROPERTIES FOLDER tests)

# Get sources and includes
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Add sources
target_sources(
  example_tests PRIVATE
  ${sources} ${includes}
)

# Add includes
target_include_directories(
  example_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(
  example_tests PRIVATE
  Catch2::Catch2WithMain
)

# Build in C++23
target_compile_features(example_tests PRIVATE cxx_std_23)

# Add test
add_test(NAME example COMMAND $<TARGET_FILE:example_tests>)

# Generate format target
generate_format_target(example_tests)
