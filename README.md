# Franka Panda ROS 2 Simulation and Custom Motion Planning

This repository contains a ROS 2 project for simulating and controlling the **Franka Emika Panda** robotic arm using **MoveIt 2, Gazebo, RViz 2**, and a custom **C++ motion-planning node**.

The project demonstrates the workflow of:

* Loading the Panda robot description
* Simulating the Panda robot in Gazebo
* Using a table environment from **Gazebo Fuel**
* Configuring ROS 2 controllers
* Running MoveIt 2
* Visualizing the robot in RViz 2
* Planning and executing robot movements using C++
* Moving the Panda between predefined Cartesian poses

## Features

* Franka Emika Panda robot simulation
* ROS 2 integration
* MoveIt 2 motion planning
* Gazebo simulation
* Gazebo Fuel table model
* RViz 2 visualization
* Robot State Publisher
* Joint State Broadcaster
* Panda arm controller
* Panda hand controller
* ROS-Gazebo communication
* Xacro-based robot description
* YAML-based controller configuration
* Custom C++ MoveIt 2 motion-planning node
* Custom launch files for simulation and robot movement
* Cartesian position targets
* Quaternion-based end-effector orientation
* NVIDIA GPU acceleration support

## Project Structure

```text
franka_panda/
│
├── panda_moveit_config/
│   ├── config/
│   │   ├── panda.urdf.xacro
│   │   ├── gazebo_controllers.yaml
│   │   ├── moveit_controllers.yaml
│   │   └── ...
│   │
│   ├── launch/
│   │   └── ...
│   │
│   ├── worlds/
│   │   └── panda_tableWorld.sdf
│   │
│   ├── CMakeLists.txt
│   └── package.xml
│
├── custom_panda/
│   ├── launch/
│   │   ├── gazebo_and_moveit.launch.py
│   │   └── move_panda.launch.py
│   │
│   ├── src/
│   │   └── move_panda.cpp
│   │
│   ├── CMakeLists.txt
│   └── package.xml
│
└── README.md
```

## Requirements

The project requires:

* Ubuntu Linux
* ROS 2
* MoveIt 2
* Gazebo Sim
* RViz 2
* `ros_gz_sim`
* `ros_gz_bridge`
* `controller_manager`
* `xacro`
* `robot_state_publisher`
* `rosdep`

Make sure the ROS 2 environment is sourced before building or running the project.

## Clone the Repository

Create or navigate to your ROS 2 workspace:

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
```

Clone the repository:

```bash
git clone https://github.com/Kiprono1385/Franka-Panda.git
```

Navigate to the workspace:

```bash
cd ~/ros2_ws
```

## Install Dependencies

Initialize `rosdep` if it has not already been initialized:

```bash
sudo rosdep init
```

Update the rosdep database:

```bash
rosdep update
```

Install the required dependencies:

```bash
rosdep install --from-paths src --ignore-src -r -y
```

## Build the Workspace

Source ROS 2:

```bash
source /opt/ros/$ROS_DISTRO/setup.bash
```

Build the workspace:

```bash
cd ~/ros2_ws
colcon build --symlink-install
```

After the build:

```bash
source install/setup.bash
```

## Launch the Complete Panda Simulation

The main simulation launch file is:

```text
custom_panda/launch/gazebo_and_moveit.launch.py
```

It starts the complete Panda simulation environment, including:

* Gazebo
* Ground plane
* Sun
* Gazebo Fuel table
* Panda robot
* ROS 2 controllers
* MoveIt 2
* RViz 2

The world file is:

```text
panda_moveit_config/worlds/panda_tableWorld.sdf
```

Launch the simulation:

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 launch custom_panda gazebo_and_moveit.launch.py
```

Wait until Gazebo, the Panda robot, controllers, MoveIt 2, and RViz 2 have fully started.

## Gazebo Fuel Table

The simulation uses a table model hosted by **Gazebo Fuel** rather than a locally created table model.

The table is included directly in:

```text
panda_moveit_config/worlds/panda_tableWorld.sdf
```

using:

```xml
<include>
  <uri>https://fuel.gazebosim.org/1.0/OpenRobotics/models/Table</uri>
  <name>work_table</name>
  <pose>0.245 0 0 0 0 1.5708</pose>
</include>
```

### Table Pose

The table pose is:

```text
X     = 0.245 m
Y     = 0.0 m
Z     = 0.0 m
Roll  = 0 rad
Pitch = 0 rad
Yaw   = 1.5708 rad
```

The yaw angle corresponds to approximately:

```text
90 degrees
```

The 90-degree rotation aligns the table correctly with the Panda robot workspace.

Because the table is hosted on Gazebo Fuel, an internet connection may be required when Gazebo retrieves the model.

## Run the Custom Panda Motion Program

The custom movement program is implemented in:

```text
custom_panda/src/move_panda.cpp
```

The custom launch file is:

```text
custom_panda/launch/move_panda.launch.py
```

### Important Note

The custom movement launch file does **not** start Gazebo, MoveIt 2, or RViz 2 again.

The main simulation must already be running.

Open a second terminal:

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 launch custom_panda move_panda.launch.py
```

The C++ program connects to the already-running MoveIt 2 system and commands the Panda arm.

## Recommended Launch Sequence

### Terminal 1 — Simulation + MoveIt 2 + RViz 2

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 launch custom_panda gazebo_and_moveit.launch.py
```

Wait for Gazebo, controllers, MoveIt 2, and RViz 2 to start.

### Terminal 2 — Custom C++ Motion

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 launch custom_panda move_panda.launch.py
```

The Panda will then execute the predefined motion sequence.

## Custom Motion Planning

The custom C++ node uses **MoveIt 2** to plan and execute movements for the Panda arm.

### Planning Group

```text
panda_arm
```

### Motion Targets

The program uses predefined Cartesian target poses for the Panda end effector.

Each target consists of:

* X position
* Y position
* Z position
* Quaternion orientation

MoveIt 2 calculates a collision-aware trajectory from the current robot configuration to each target pose and sends the resulting trajectory to the Panda arm controller.

## Robot Description

The Panda robot description is defined using Xacro.

The main robot description is located in:

```text
panda_moveit_config/config/panda.urdf.xacro
```

The Xacro description provides the robot's:

* Links
* Joints
* Visual geometry
* Collision geometry
* Inertial properties
* Joint limits
* Gazebo-related configuration

The robot description is processed and published to ROS 2 using the appropriate robot description and state publisher nodes.

## Controllers

The simulation uses ROS 2 control components to connect MoveIt 2 with the simulated Panda.

The project includes configuration for:

* Joint State Broadcaster
* Panda arm trajectory controller
* Panda hand controller

The controller configuration is stored in the package configuration directory.

Controllers can be inspected using:

```bash
ros2 control list_controllers
```

The Panda arm controller should be active before running the custom motion node.

## RViz 2

RViz 2 is used to visualize:

* Panda robot model
* Robot joint states
* TF frames
* MoveIt 2 planning scene
* Planned trajectories
* Executed robot motion

RViz 2 provides a visual interface for monitoring the robot and the motion-planning process.

## Gazebo Simulation

Gazebo provides the physics simulation environment for the Panda.

The simulation contains:

* Ground plane
* Sun/light source
* Gazebo Fuel table
* Franka Panda robot

The table is positioned at:

```text
X = 0.245 m
Y = 0.0 m
Z = 0.0 m
```

and rotated:

```text
Yaw = 1.5708 rad ≈ 90°
```

This configuration places the table in the intended orientation relative to the Panda workspace.

## Rebuild After Code Changes

Whenever `move_panda.cpp` or other `custom_panda` package files are modified:

```bash
cd ~/ros2_ws
source /opt/ros/$ROS_DISTRO/setup.bash
colcon build --packages-select custom_panda --symlink-install
source install/setup.bash
```

If files in `panda_moveit_config` are modified:

```bash
cd ~/ros2_ws
source /opt/ros/$ROS_DISTRO/setup.bash
colcon build --packages-select panda_moveit_config --symlink-install
source install/setup.bash
```

## Clean and Rebuild

If you encounter build or configuration problems:

```bash
cd ~/ros2_ws
rm -rf build install log
source /opt/ros/$ROS_DISTRO/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## Troubleshooting

### Gazebo Cannot Find the Table

The table is loaded from Gazebo Fuel.

Make sure the computer has an active internet connection when launching the simulation.

If the Fuel model has already been downloaded and cached, Gazebo may be able to load it without downloading it again.

### MoveIt Does Not Execute the Motion

Check the active controllers:

```bash
ros2 control list_controllers
```

Make sure the Panda arm trajectory controller is active.

Also verify that MoveIt 2 is running:

```bash
ros2 node list
```

### RViz Does Not Display the Panda

Check that the robot description and joint states are being published:

```bash
ros2 topic list
```

Check the joint states:

```bash
ros2 topic echo /joint_states
```

### Check the TF Tree

If the robot appears incorrectly positioned in RViz, inspect the TF tree:

```bash
ros2 run tf2_tools view_frames
```

This can help identify missing or incorrect transforms.

## Project Goal

The goal of this project is to demonstrate practical experience with:

* ROS 2
* Gazebo simulation
* MoveIt 2
* RViz 2
* Robot modeling
* ROS 2 control
* Motion planning
* Cartesian pose control
* C++
* Robotic arm simulation

The project focuses on using a simulated **Franka Emika Panda** to plan and execute predefined robotic arm movements in a realistic table-based environment.

## Author

**Kiprono**

GitHub: https://github.com/Kiprono1385/Franka-Panda
