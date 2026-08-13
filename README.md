# Franka Panda ROS 2 Simulation and Custom Motion Planning

This repository contains a ROS 2 project for simulating and controlling the **Franka Emika Panda robotic arm** using **MoveIt 2**, **Gazebo**, **RViz 2**, and a custom C++ motion-planning node.

The project demonstrates the workflow of:

* Loading the Panda robot description
* Simulating the robot in Gazebo
* Configuring ROS 2 controllers
* Running MoveIt 2
* Visualizing the robot in RViz 2
* Planning and executing robot movements using C++
* Moving the Panda between predefined Cartesian poses

---

## Features

* Franka Emika Panda robot simulation
* ROS 2 integration
* MoveIt 2 motion planning
* Gazebo simulation
* RViz 2 visualization
* Robot State Publisher
* Joint State Broadcaster
* Panda arm controller
* Panda hand controller
* ROS-Gazebo communication
* Xacro-based robot description
* YAML-based controller configuration
* Custom C++ MoveIt 2 motion-planning node
* Custom launch file for robot movement
* Cartesian position targets
* Quaternion-based end-effector orientation
* NVIDIA GPU acceleration support

---

# Project Structure

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
│   └── launch/
│       ├── gazebo_and_moveit.launch.py
│       └── ...
│
├── custom_panda/
│   ├── src/
│   │   └── move_panda.cpp
│   │
│   ├── launch/
│   │   └── move_panda.launch.py
│   │
│   ├── CMakeLists.txt
│   └── package.xml
│
└── README.md
```

---

# Requirements

The project requires:

* Ubuntu Linux
* ROS 2
* MoveIt 2
* Gazebo / Gazebo Sim
* RViz 2
* `ros_gz_sim`
* `ros_gz_bridge`
* `controller_manager`
* `xacro`
* `robot_state_publisher`
* `rosdep`

Make sure the ROS 2 environment is sourced before building or running the project.

---

# Clone the Repository

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

---

# Install Dependencies

Initialize `rosdep` if it has not already been initialized:

```bash
sudo rosdep init
```

If `rosdep` has already been initialized, this command may report that it is already initialized. Continue with the next step.

Update the rosdep database:

```bash
rosdep update
```

Install the required dependencies:

```bash
rosdep install --from-paths src --ignore-src -r -y
```

---

# Build the Workspace

Navigate to the workspace:

```bash
cd ~/ros2_ws
```

Source ROS 2:

```bash
source /opt/ros/$ROS_DISTRO/setup.bash
```

Build the workspace:

```bash
colcon build --symlink-install
```

After the build:

```bash
source install/setup.bash
```

---

# Launch the Complete Panda Simulation

The main launch file starts the complete Panda simulation environment, including:

* Gazebo
* Panda robot
* Controllers
* MoveIt 2
* RViz 2
* Robot description
* ROS-Gazebo interfaces
* Required TF configuration

Run:

```bash
cd ~/ros2_ws
source install/setup.bash

ros2 launch panda_moveit_config gazebo_and_moveit.launch.py
```

Wait until the Panda robot has spawned and MoveIt 2/RViz 2 are running.

The launch file is responsible for starting:

1. Panda robot description
2. Robot State Publisher
3. Gazebo
4. Panda robot spawning
5. ROS-Gazebo bridge
6. Joint State Broadcaster
7. Panda arm controller
8. Panda hand controller
9. MoveIt 2 `move_group`
10. RViz 2
11. Required TF configuration

---

# Run the Custom Panda Motion Program

The custom movement program is implemented in:

```text
custom_panda/src/move_panda.cpp
```

The program uses MoveIt 2's:

```cpp
MoveGroupInterface
```

to plan and execute movement of the Panda arm.

The custom launch file is:

```text
custom_panda/launch/move_panda.launch.py
```

## Important

The custom movement launch file **does not start Gazebo, MoveIt 2, or RViz 2 again**.

The main simulation must already be running.

Open a second terminal:

```bash
cd ~/ros2_ws
source install/setup.bash
```

Then run:

```bash
ros2 launch custom_panda move_panda.launch.py
```

The C++ program will connect to the already-running MoveIt 2 system and command the Panda arm.

---

# Recommended Launch Sequence

The recommended workflow uses **two terminals**.

## Terminal 1 — Simulation + MoveIt 2 + RViz 2

```bash
cd ~/ros2_ws
source install/setup.bash

ros2 launch panda_moveit_config gazebo_and_moveit.launch.py
```

Wait for Gazebo, MoveIt 2, RViz 2, the Panda robot, and controllers to initialize.

---

## Terminal 2 — Custom C++ Motion

```bash
cd ~/ros2_ws
source install/setup.bash

ros2 launch custom_panda move_panda.launch.py
```

The Panda will then execute the motion defined in:

```text
custom_panda/src/move_panda.cpp
```

The overall workflow is:

```text
Terminal 1
│
└── gazebo_and_moveit.launch.py
    │
    ├── Gazebo
    ├── Panda
    ├── Controllers
    ├── MoveIt 2
    └── RViz 2
             │
             ▼
Terminal 2
│
└── move_panda.launch.py
    │
    └── move_panda.cpp
        │
        ├── Point 1
        └── Point 2
```

---

# Custom Motion Planning

The custom C++ node uses the MoveIt 2 planning group:

```cpp
static const std::string PLANNING_GROUP = "panda_arm";
```

The program also obtains information about the planning frame and end-effector link from the running MoveIt configuration.

Typical output includes:

```text
Planning frame: panda_link0
End effector link: panda_hand
Planning group: panda_arm
```

---

# Defined Robot Positions

The current program uses two measured end-effector poses.

The poses were obtained from the running simulation using:

```bash
ros2 run tf2_ros tf2_echo panda_link0 panda_hand
```

This provides the transform from the Panda base:

```text
panda_link0
```

to the end effector:

```text
panda_hand
```

---

## Point 1

The first measured position is:

```text
X =  0.280 m
Y = -0.454 m
Z =  0.125 m
```

The orientation is represented using a quaternion in `(x, y, z, w)` order:

```text
[1.0, 0.0, 0.0, 0.0]
```

Approximately:

```text
Roll  = -180°
Pitch =    0°
Yaw   =    0°
```

The C++ representation is:

```cpp
geometry_msgs::msg::Pose point_1;

point_1.position.x = 0.280;
point_1.position.y = -0.454;
point_1.position.z = 0.125;

point_1.orientation.x = 1.0;
point_1.orientation.y = 0.0;
point_1.orientation.z = 0.0;
point_1.orientation.w = 0.0;
```

---

# Point 2

The second measured position is:

```text
X = 0.269 m
Y = 0.665 m
Z = 0.211 m
```

The orientation is:

```text
Quaternion (x, y, z, w)

[1.0, 0.0, 0.0, 0.0]
```

Approximately:

```text
Roll  = -180°
Pitch =    0°
Yaw   =    0°
```

The C++ representation is:

```cpp
geometry_msgs::msg::Pose point_2;

point_2.position.x = 0.269;
point_2.position.y = 0.665;
point_2.position.z = 0.211;

point_2.orientation.x = 1.0;
point_2.orientation.y = 0.0;
point_2.orientation.z = 0.0;
point_2.orientation.w = 0.0;
```

---

# Motion Between the Two Points

The current program moves between the two measured Cartesian poses:

```text
Point 1
(0.280, -0.454, 0.125)
       │
       │
       ▼
Point 2
(0.269,  0.665, 0.211)
```

The approximate Cartesian displacement is:

```text
ΔX = 0.269 - 0.280
   = -0.011 m

ΔY = 0.665 - (-0.454)
   = +1.119 m

ΔZ = 0.211 - 0.125
   = +0.086 m
```

Therefore, the largest movement is along the **Y axis**.

The same end-effector orientation is maintained at both target poses.

---

# MoveIt Planning

The program uses:

```cpp
move_group.setPoseTarget(point_1);
```

to set the first target.

It then uses:

```cpp
move_group.setPoseTarget(point_2);
```

to set the second target.

MoveIt calculates a collision-free joint trajectory to each target.

The current planning parameters are:

```cpp
move_group.setPlanningTime(5.0);

move_group.setNumPlanningAttempts(10);

move_group.setMaxVelocityScalingFactor(0.3);

move_group.setMaxAccelerationScalingFactor(0.3);
```

The velocity and acceleration scaling factors are set to `0.3` to provide relatively conservative motion during simulation.

---

# Custom Movement Source Code

The main C++ movement program is located at:

```text
custom_panda/src/move_panda.cpp
```

The program:

1. Initializes ROS 2.
2. Creates a MoveIt `MoveGroupInterface`.
3. Selects the `panda_arm` planning group.
4. Reads the MoveIt planning frame and end-effector link.
5. Defines Point 1.
6. Plans a trajectory to Point 1.
7. Executes the trajectory.
8. Waits for two seconds.
9. Defines Point 2.
10. Plans a trajectory to Point 2.
11. Executes the trajectory.
12. Finishes and shuts down ROS 2.

---

# Custom Launch File

The custom launch file is:

```text
custom_panda/launch/move_panda.launch.py
```

Its purpose is to start the custom movement node and provide the required Panda MoveIt configuration.

It **does not** start:

* Another Gazebo instance
* Another `move_group`
* Another RViz 2 instance

This prevents duplicate simulation and MoveIt nodes.

---

# Controllers

The project uses the following controllers.

## Joint State Broadcaster

```text
joint_state_broadcaster
```

Publishes the current joint states of the Panda.

## Panda Arm Controller

```text
panda_arm_controller
```

Controls the Panda arm joints.

## Hand Controller

```text
hand_controller
```

Controls the Panda gripper/hand.

Controller parameters are configured in:

```text
panda_moveit_config/config/gazebo_controllers.yaml
```

---

# MoveIt 2 Configuration

The MoveIt 2 configuration is located inside:

```text
panda_moveit_config/
```

The Panda robot description is generated using:

```text
panda_moveit_config/config/panda.urdf.xacro
```

The MoveIt configuration contains information about:

* Robot description
* SRDF
* Planning groups
* End effectors
* Kinematics
* Joint limits
* Controllers
* Planning pipelines

---

# Gazebo

Gazebo provides the simulated environment and physics for the Panda.

The Panda robot is spawned from its ROS 2 robot description.

The simulation provides the environment in which the Panda controllers and MoveIt 2 operate.

---

# ROS-Gazebo Bridge

The project uses:

```text
ros_gz_bridge
```

to exchange information between ROS 2 and Gazebo.

The bridge handles information such as:

* Simulation clock
* TF
* Joint states

This allows MoveIt 2 and other ROS 2 nodes to communicate with the simulated robot.

---

# NVIDIA GPU Support

The simulation launch configuration includes NVIDIA GPU acceleration settings:

```bash
LIBGL_ALWAYS_SOFTWARE=0
__NV_PRIME_RENDER_OFFLOAD=1
__GLX_VENDOR_LIBRARY_NAME=nvidia
```

These settings are useful on systems with supported NVIDIA discrete graphics.

If Gazebo has graphical problems on another computer, these environment variables may need to be modified.

---

# Gazebo Resource Paths

The launch configuration dynamically sets:

```text
IGN_GAZEBO_RESOURCE_PATH
```

and:

```text
GZ_SIM_RESOURCE_PATH
```

This allows Gazebo to locate the Panda models and other required simulation resources.

---

# Useful ROS 2 Commands

## List packages

```bash
ros2 pkg list
```

## Check running nodes

```bash
ros2 node list
```

## Check available topics

```bash
ros2 topic list
```

## Check joint states

```bash
ros2 topic echo /joint_states
```

## Check controllers

```bash
ros2 control list_controllers
```

## Inspect TF

Check the transform from the Panda base to the end effector:

```bash
ros2 run tf2_ros tf2_echo panda_link0 panda_hand
```

Example:

```text
Translation: [0.269, 0.665, 0.211]

Rotation:
Quaternion (xyzw) [1.000, 0.000, 0.000, 0.000]
```

---

# Rebuild After Code Changes

Whenever `move_panda.cpp` is modified:

```bash
cd ~/ros2_ws

source /opt/ros/$ROS_DISTRO/setup.bash

colcon build --packages-select custom_panda --symlink-install

source install/setup.bash
```

Then make sure the main simulation is running:

```bash
ros2 launch panda_moveit_config gazebo_and_moveit.launch.py
```

In another terminal:

```bash
cd ~/ros2_ws
source install/setup.bash

ros2 launch custom_panda move_panda.launch.py
```

---

# Clean and Rebuild

If you encounter build or configuration problems:

```bash
cd ~/ros2_ws

rm -rf build install log

source /opt/ros/$ROS_DISTRO/setup.bash

colcon build --symlink-install

source install/setup.bash
```

---

# Troubleshooting

## MoveIt Cannot Find `robot_description_semantic`

If the custom movement node reports:

```text
Could not find parameter robot_description_semantic
```

make sure the main Panda MoveIt launch is already running:

```bash
ros2 launch panda_moveit_config gazebo_and_moveit.launch.py
```

Then, from another terminal:

```bash
ros2 launch custom_panda move_panda.launch.py
```

The custom launch file also loads the Panda MoveIt configuration required by the C++ node.

---

## Check Whether MoveIt Is Running

Run:

```bash
ros2 node list
```

You should see:

```text
/move_group
```

You can also inspect the available ROS 2 topics:

```bash
ros2 topic list
```

for MoveIt-related topics and actions.

---

## Check the Panda End Effector

Run:

```bash
ros2 run tf2_ros tf2_echo panda_link0 panda_hand
```

This can be used to determine the current Cartesian position and orientation of the Panda hand.

---

# Current Development Status

🚧 **Work in Progress**

The project currently includes:

* Panda Gazebo simulation
* Panda controllers
* MoveIt 2 integration
* RViz 2 visualization
* Custom C++ motion planning
* Custom movement launch file
* Two measured Cartesian target poses

The next stage of development can include:

* Cartesian path planning
* Pick-and-place motion
* Gripper control
* Multiple target positions
* Collision-aware manipulation
* Object interaction
* Automated motion sequences

---

# Author

**Kiprono**

GitHub:

```text
https://github.com/Kiprono1385/Franka-Panda
```
