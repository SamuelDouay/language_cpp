# Declare target
add_library(dynamic INTERFACE)

# Get sources and includes
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Add custom sources property for format and tidy
set_target_properties(dynamic PROPERTIES CUSTOM_SOURCES "${includes}")

# Add includes
target_include_directories(
  dynamic INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Build in C++23
target_compile_features(dynamic INTERFACE cxx_std_23)

# Generate format target
generate_format_target(dynamic)

# Generate tidy target
generate_tidy_target(dynamic)
