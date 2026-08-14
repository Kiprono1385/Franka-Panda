cmake_minimum_required(VERSION 3.8)
project(custom_panda)

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
# Executable
# ============================================================

add_executable(move_panda
  src/move_panda.cpp
)

# ============================================================
# ROS 2 Dependencies
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
# Install Targets & Directories
# ============================================================

install(
  TARGETS move_panda
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

# Install world files so Gazebo can locate them
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

ament_package()