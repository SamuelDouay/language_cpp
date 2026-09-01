# Declare target
add_executable(dynamic_tests)
set_target_properties(dynamic_tests PROPERTIES FOLDER tests)

# Get sources and includes
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Add sources
target_sources(
  dynamic_tests PRIVATE
  ${sources} ${includes}
)

# Add includes
target_include_directories(
  dynamic_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(
  dynamic_tests PRIVATE
  dynamic
  Catch2::Catch2WithMain
)

# Build in C++23
target_compile_features(dynamic_tests PRIVATE cxx_std_23)

# Add test
add_test(NAME dynamic COMMAND $<TARGET_FILE:dynamic_tests>)

# Generate format target
generate_format_target(dynamic_tests)
