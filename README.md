# Franka Panda ROS 2 Simulation

This repository contains a ROS 2 project for simulating the **Franka Emika Panda robotic arm** using **MoveIt 2** and **Gazebo**.

The project is currently focused on configuring the Panda robot description, controllers, MoveIt 2, Gazebo simulation, and ROS-Gazebo communication.

## Features

* Franka Panda robot simulation
* ROS 2 integration
* MoveIt 2 configuration
* Gazebo simulation
* Robot State Publisher
* Joint State Broadcaster
* Panda arm controller
* Panda hand controller
* ROS-Gazebo bridge
* Gazebo resource path configuration
* NVIDIA GPU acceleration support
* Xacro-based robot description
* YAML-based controller configuration

## Project Structure

```text
franka_panda/
├── src/
│   ├── franka_panda/
│   │   └── ...
│   │
│   └── panda_moveit_config/
│       ├── config/
│       │   ├── panda.urdf.xacro
│       │   └── gazebo_controllers.yaml
│       │
│       └── launch/
│           └── panda_gazebo.launch.py
│
├── build/
├── install/
└── log/
```

## Requirements

The project requires:

* Ubuntu Linux
* ROS 2
* MoveIt 2
* Gazebo / Gazebo Sim
* `ros_gz_sim`
* `ros_gz_bridge`
* `controller_manager`
* `xacro`
* `robot_state_publisher`

Make sure your ROS 2 environment is sourced before building or running the project.

## Build the Workspace

Navigate to the workspace:

```bash
cd ~/ros2_ws/franka_panda
```

Source ROS 2:

```bash
source /opt/ros/$ROS_DISTRO/setup.bash
```

Build the workspace:

```bash
colcon build --symlink-install
```

After the build completes:

```bash
source install/setup.bash
```

## Launch the Panda Simulation

Run the Gazebo simulation using:

```bash
ros2 launch panda_moveit_config panda_gazebo.launch.py
```

The launch file performs the following tasks:

1. Loads the Panda robot description.
2. Starts `robot_state_publisher`.
3. Starts Gazebo.
4. Spawns the Panda robot.
5. Starts the ROS-Gazebo bridge.
6. Loads the controller configuration.
7. Starts the joint state broadcaster.
8. Starts the Panda arm controller.
9. Starts the Panda hand controller.

Controller startup is delayed to allow Gazebo enough time to initialize.

## Controllers

The project uses the following controllers:

### Joint State Broadcaster

```text
joint_state_broadcaster
```

Publishes the current joint states of the robot.

### Panda Arm Controller

```text
panda_arm_controller
```

Controls the Panda arm joints.

### Hand Controller

```text
hand_controller
```

Controls the Panda gripper/hand.

Controller parameters are stored in:

```text
panda_moveit_config/config/gazebo_controllers.yaml
```

## MoveIt 2

The project uses MoveIt 2 for robot motion planning and manipulation.

The Panda robot description and MoveIt configuration are located inside:

```text
panda_moveit_config/
```

The robot description is generated from:

```text
panda_moveit_config/config/panda.urdf.xacro
```

## Gazebo

The simulation uses Gazebo to provide the physics and visualization environment.

The launch file starts an empty Gazebo world and then spawns the Panda robot:

```text
empty.sdf
```

The Panda robot is spawned using its ROS 2 `robot_description`.

## ROS-Gazebo Bridge

The project uses `ros_gz_bridge` to communicate between ROS 2 and Gazebo.

The bridge currently handles:

* Simulation clock
* TF
* Joint states

This allows ROS 2 nodes and MoveIt 2 to interact with the simulated robot.

## NVIDIA GPU Support

The launch file includes environment variables for NVIDIA GPU acceleration:

```bash
LIBGL_ALWAYS_SOFTWARE=0
__NV_PRIME_RENDER_OFFLOAD=1
__GLX_VENDOR_LIBRARY_NAME=nvidia
```

These settings are intended to allow Gazebo to use the NVIDIA discrete GPU on supported systems.

If Gazebo behaves unexpectedly on a system without NVIDIA graphics, these settings may need to be adjusted.

## Gazebo Resource Paths

The launch file dynamically configures:

```text
IGN_GAZEBO_RESOURCE_PATH
GZ_SIM_RESOURCE_PATH
```

This allows Gazebo to locate the robot models and other simulation resources.

## Useful Commands

Check available ROS 2 packages:

```bash
ros2 pkg list
```

Check available launch files:

```bash
ros2 launch panda_moveit_config
```

Check active nodes:

```bash
ros2 node list
```

Check active topics:

```bash
ros2 topic list
```

Check joint states:

```bash
ros2 topic echo /joint_states
```

Check controllers:

```bash
ros2 control list_controllers
```

## Clean and Rebuild

If you encounter build or configuration problems, you can clean the workspace:

```bash
cd ~/ros2_ws/franka_panda

rm -rf build install log

colcon build --symlink-install

source install/setup.bash
```

## Git Workflow

This project is developed locally and pushed to GitHub for version control.

After making changes:

```bash
git status
```

Add the changes:

```bash
git add .
```

Commit:

```bash
git commit -m "describe your changes"
```

Push:

```bash
git push origin main
```

Before starting new work, it is recommended to synchronize with the remote repository:

```bash
git pull --rebase origin main
```

## Project Status

🚧 **Work in Progress**

The project is actively being developed. Configuration, controllers, simulation, and MoveIt 2 integration may change as development continues.

## Author

**Kiprono**

GitHub:

```text
https://github.com/Kiprono1385/Franka-Panda
```
