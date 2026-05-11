from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, SetEnvironmentVariable, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution
from ament_index_python.packages import get_package_share_directory
import os
import xacro

def generate_launch_description():
    # 1. Environment Safety for EliteBook Graphics (Prevents Exit Code -11)
    env_vars = [
        SetEnvironmentVariable('LIBGL_ALWAYS_SOFTWARE', '1'),
        SetEnvironmentVariable('IGN_GAZEBO_RESOURCE_PATH', 
            os.path.join(os.getenv('HOME'), 'franka_panda_ws/install/franka_panda/share'))
    ]

    share_dir = get_package_share_directory('panda_moveit_config')
    xacro_file = os.path.join(share_dir, 'config', 'panda.urdf.xacro')
    robot_description_config = xacro.process_file(xacro_file)

    # 2. Robot State Publisher
    rsp = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description_config.toxml(), 'use_sim_time': True}]
    )

    # 3. Ignition Gazebo Simulation
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([FindPackageShare('ros_gz_sim'), 'launch', 'gz_sim.launch.py'])
        ]),
        launch_arguments={'gz_args': '-r empty.sdf'}.items(),
    )

    # 4. Spawn Panda
    spawn_panda = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=['-name', 'panda', '-topic', 'robot_description', '-z', '0.0'],
        output='screen'
    )

    # 5. The Bridge (Maps Clock and Joint States)
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '/clock@rosgraph_msgs/msg/Clock[ignition.msgs.Clock',
            '/model/panda/tf@tf2_msgs/msg/TFMessage[ignition.msgs.Pose_V',
            '/world/empty/model/panda/joint_state@sensor_msgs/msg/JointState[ignition.msgs.Model'
        ],
        output='screen'
    )

    # 6. Controller Spawners (Delayed to ensure Gazebo is ready)
    jsb = Node(package="controller_manager", executable="spawner", arguments=["joint_state_broadcaster"])
    arm_controller = Node(package="controller_manager", executable="spawner", arguments=["panda_arm_controller"])
    hand_controller = Node(package="controller_manager", executable="spawner", arguments=["hand_controller"])

    # Wrap spawners in a 15-second timer
    delayed_spawners = TimerAction(
        period=5.0,
        actions=[jsb, arm_controller, hand_controller]
    )

    return LaunchDescription(env_vars + [
        rsp, 
        gz_sim, 
        spawn_panda, 
        bridge, 
        delayed_spawners
    ])