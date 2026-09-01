# Declare target
add_executable(excercice)

# Get sources and includes
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

# Add sources
target_sources(
  excercice PRIVATE
  ${sources} ${includes}
)

# Add includes
target_include_directories(
  excercice PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Build in C++23
target_compile_features(excercice PRIVATE cxx_std_23)

# Add warning flags
target_add_cxx_warning_flags(excercice)

# Generate format target
generate_format_target(excercice)

# Generate tidy target
generate_tidy_target(excercice)
