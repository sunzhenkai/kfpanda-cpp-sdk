cmake_minimum_required(VERSION 3.20)
project(kfpanda)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-Wall)
include(GNUInstallDirs) # CMAKE_INSTALL_INCLUDEDIR / CMAKE_INSTALL_LIBDIR / ...

if(BUILD_TESTING)
  # set testing install prefix
  set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build/install/)
endif()

# 3rd packages
find_package(fmt CONFIG REQUIRED)
find_package(unofficial-brpc CONFIG REQUIRED)

set(THIRD_LIBRARIES fmt::fmt unofficial::brpc::brpc-static)

include_directories(.)
file(GLOB_RECURSE LIB_HREADERS kfpanda/*.h)
file(GLOB_RECURSE LIB_SRCS kfpanda/*.cc)
add_library(${PROJECT_NAME} ${LIB_HREADERS} ${LIB_SRCS})
target_link_libraries(${PROJECT_NAME} ${THIRD_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

# install files
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(
  DIRECTORY kfpanda
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  FILES_MATCHING
  PATTERN "*.h")
install(
  EXPORT ${PROJECT_NAME}
  DESTINATION share/${PROJECT_NAME}
  FILE ${PROJECT_NAME}Config.cmake
  NAMESPACE ${PROJECT_NAME}::)
