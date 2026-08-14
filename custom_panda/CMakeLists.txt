cmake_minimum_required(VERSION 3.8)

project(custom_panda)


# ============================================================
# Compiler options
# ============================================================

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# ============================================================
# Dependencies
# ============================================================

find_package(ament_cmake REQUIRED)

find_package(rclcpp REQUIRED)

find_package(moveit_ros_planning_interface REQUIRED)

find_package(moveit_msgs REQUIRED)

find_package(geometry_msgs REQUIRED)

find_package(shape_msgs REQUIRED)

find_package(linkattacher_msgs REQUIRED)


# ============================================================
# Include directory
# ============================================================

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# ============================================================
# Executable
# ============================================================

add_executable(move_panda
  src/move_panda.cpp
  src/panda_planner.cpp
)


# ============================================================
# ROS 2 dependencies
# ============================================================

ament_target_dependencies(move_panda
  rclcpp
  moveit_ros_planning_interface
  moveit_msgs
  geometry_msgs
  shape_msgs
  linkattacher_msgs
)


# ============================================================
# Install executable
# ============================================================

install(
  TARGETS move_panda
  DESTINATION lib/${PROJECT_NAME}
)


# ============================================================
# Install headers
# ============================================================

install(
  DIRECTORY include/
  DESTINATION include
)


# ============================================================
# Install launch files
# ============================================================

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)


# ============================================================
# Install world files
# ============================================================

install(
  DIRECTORY worlds
  DESTINATION share/${PROJECT_NAME}
)


# ============================================================
# Testing
# ============================================================

if(BUILD_TESTING)

  find_package(ament_lint_auto REQUIRED)

  set(ament_cmake_copyright_FOUND TRUE)

  set(ament_cmake_cpplint_FOUND TRUE)

  ament_lint_auto_find_test_dependencies()

endif()


# ============================================================
# Package
# ============================================================

ament_package()