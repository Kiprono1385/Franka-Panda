Running the Simulation
Start Gazebo and spawn controllers:

Bash
ros2 launch panda_moveit_config panda_gazebo.launch.py
In a new terminal, launch MoveIt and RViz:

Bash
cd ~/ros2_ws
source install/setup.bash
ros2 launch panda_moveit_config moveit_rviz.launch.py