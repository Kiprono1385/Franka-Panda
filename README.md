# Franka Panda ROS 2 Simulation and Custom Motion Planning

This repository contains a ROS 2 project for simulating and controlling the **Franka Emika Panda** robotic arm using **MoveIt 2, Gazebo, RViz 2**, and a custom **C++ motion-planning node**.

The project demonstrates the workflow of:

* Loading the Panda robot description
* Simulating the Panda in Gazebo
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
* Table environment from Gazebo Fuel
* Panda robot
* ROS 2 controllers
* MoveIt 2
* RViz 2

The table is defined in:

```text
panda_moveit_config/worlds/panda_tableWorld.sdf
```

The world loads the table directly from Gazebo Fuel rather than maintaining a local copy of the table model.

Run:

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 launch custom_panda gazebo_and_moveit.launch.py
```

Wait until the Panda robot has spawned and MoveIt 2/RViz 2 are fully running.

## Gazebo Fuel Table

The simulation uses a table model hosted by Gazebo Fuel.

The world file contains a model include similar to:

```xml
<include>
  <uri>https://fuel.gazebosim.org/1.0/OpenRobotics/models/Table</uri>
  <name>work_table</name>
  <pose>0.7 0 0 0 0 0</pose>
</include>
```

This allows Gazebo to retrieve the table model when the world is launched.

An internet connection may therefore be required the first time the model is downloaded.

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

The C++ program will connect to the already-running MoveIt 2 system and command the Panda arm.

## Recommended Launch Sequence

### Terminal 1 — Simulation + MoveIt 2 + RViz 2

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 launch custom_panda gazebo_and_moveit.launch.py
```

Wait for Gazebo, MoveIt 2, controllers, and RViz 2 to start.

### Terminal 2 — Custom C++ Motion

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 launch custom_panda move_panda.launch.py
```

## Custom Motion Planning

The custom C++ node uses MoveIt 2 to plan and execute movements for the Panda arm.

### Planning Group

```text
panda_arm
```

### Motion Targets

The program uses predefined Cartesian targets for the Panda end effector.

The target poses are defined using:

* X position
* Y position
* Z position
* Quaternion orientation

The Panda is commanded to move between these predefined positions using MoveIt 2.

## Table Configuration

The table is defined in:

```text
panda_moveit_config/worlds/panda_tableWorld.sdf
```

The environment includes:

* Gazebo ground plane
* Gazebo sun
* Gazebo Fuel table
* Panda robot

The table position and orientation are controlled directly from the SDF world file.

For example:

```xml
<pose>0.7 0 0 0 0 0</pose>
```

represents:

```text
X = 0.7 m
Y = 0.0 m
Z = 0.0 m
Roll = 0
Pitch = 0
Yaw = 0
```

The exact table placement can be adjusted in the world file to match the Panda workspace.

## Rebuild After Code Changes

Whenever `move_panda.cpp` or package files are modified:

```bash
cd ~/ros2_ws
source /opt/ros/$ROS_DISTRO/setup.bash
colcon build --packages-select custom_panda --symlink-install
source install/setup.bash
```

If you modify `panda_moveit_config`:

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

### Gazebo cannot find the table

Make sure the computer has internet access when launching the world for the first time.

You can also test the world independently:

```bash
gz sim ~/ros2_ws/install/panda_moveit_config/share/panda_moveit_config/worlds/panda_tableWorld.sdf
```

### MoveIt does not execute the motion

Check that the controllers are running:

```bash
ros2 control list_controllers
```

The Panda arm controller should be active before starting the custom motion node.

### RViz does not display the Panda

Check that the robot description and joint states are being published:

```bash
ros2 topic list
```

and:

```bash
ros2 topic echo /joint_states
```

## Author

**Kiprono**

GitHub: https://github.com/Kiprono1385/Franka-Panda

---

## Project Goal

The goal of this project is to demonstrate practical experience with **ROS 2, Gazebo simulation, MoveIt 2, robot modeling, controllers, RViz 2, and C++ robotic motion planning** using the Franka Emika Panda robotic arm.
