#----------------------------------------------------------------------
# Build rules

add_executable(unit_tests)

target_sources(unit_tests
  PRIVATE
  serial_port_unit_tests.cpp
  test_main.cpp
)

target_include_directories(unit_tests
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(unit_tests
  PRIVATE
  GTest::gtest
  GTest::gmock
  biojet
)

target_compile_options(unit_tests PRIVATE
  -Wno-global-constructors
)

#----------------------------------------------------------------------
# Test rules
include(GoogleTest)
gtest_discover_tests(unit_tests DISCOVERY_MODE PRE_TEST)

#----------------------------------------------------------------------