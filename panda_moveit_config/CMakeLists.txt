cmake_minimum_required(VERSION 3.22)
project(panda_moveit_config)

find_package(ament_cmake REQUIRED)

ament_package()

# =========================
# Install launch files
# =========================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/launch")
  install(
    DIRECTORY launch
    DESTINATION share/${PROJECT_NAME}
    PATTERN "setup_assistant.launch" EXCLUDE
  )
endif()

# =========================
# Install configuration files
# =========================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config")
  install(
    DIRECTORY config
    DESTINATION share/${PROJECT_NAME}
  )
endif()





# =========================
# Install MoveIt Setup Assistant configuration
# =========================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.setup_assistant")
  install(
    FILES .setup_assistant
    DESTINATION share/${PROJECT_NAME}
  )
endif()