#----------------------------------------------------------------------
# Build rules

add_library(biojet)

target_sources(biojet
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include
  FILES
  ../include/biojet/blocking_queue.hpp
  ../include/biojet/result.hpp
  ../include/biojet/serial_port.hpp
  ../include/biojet/status_code.hpp
  ../include/biojet/transport.hpp
  ../include/biojet/unique_handle.hpp
  PRIVATE
  $<$<PLATFORM_ID:Linux>:serial_port_unix.cpp>
  $<$<PLATFORM_ID:Linux>:serial_port_unix.hpp>
  serial_port.cpp
)

target_include_directories(biojet
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(biojet spdlog::spdlog)

#----------------------------------------------------------------------
# Install rules
install(TARGETS biojet FILE_SET HEADERS)
