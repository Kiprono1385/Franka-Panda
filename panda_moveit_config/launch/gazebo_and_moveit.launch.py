import os
import yaml
import xacro
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, SetEnvironmentVariable, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution
from ament_index_python.packages import get_package_share_directory
from moveit_configs_utils import MoveItConfigsBuilder

def load_yaml(package_name, file_path):
    package_path = get_package_share_directory(package_name)
    absolute_file_path = os.path.join(package_path, file_path)
    try:
        with open(absolute_file_path, "r") as file:
            return yaml.safe_load(file)
    except EnvironmentError:
        return None

def generate_launch_description():
    # -------------------------------------------------------------
    # 1. Environment & Paths Setup
    # -------------------------------------------------------------
    franka_panda_share = os.path.dirname(get_package_share_directory('franka_panda'))
    share_dir = get_package_share_directory('panda_moveit_config')

    env_vars = [
        # Disable Software Rendering (Allows hardware GPU acceleration)
        SetEnvironmentVariable('LIBGL_ALWAYS_SOFTWARE', '0'),
        
        # Force OpenGL rendering offload to your NVIDIA Discrete GPU
        SetEnvironmentVariable('__NV_PRIME_RENDER_OFFLOAD', '1'),
        SetEnvironmentVariable('__GLX_VENDOR_LIBRARY_NAME', 'nvidia'),

        # Gazebo Resource Paths (Resolves model:// URIs dynamically)
        SetEnvironmentVariable('IGN_GAZEBO_RESOURCE_PATH', 
            [franka_panda_share, ':', os.environ.get('IGN_GAZEBO_RESOURCE_PATH', '')]),
        SetEnvironmentVariable('GZ_SIM_RESOURCE_PATH', 
            [franka_panda_share, ':', os.environ.get('GZ_SIM_RESOURCE_PATH', '')]),
    ]

    # -------------------------------------------------------------
    # 2. Robot Description & MoveIt Configs
    # -------------------------------------------------------------
    xacro_file = os.path.join(share_dir, 'config', 'panda.urdf.xacro')
    robot_description_config = xacro.process_file(xacro_file)

    moveit_config = (
        MoveItConfigsBuilder("panda", package_name="panda_moveit_config")
        .robot_description(file_path="config/panda.urdf.xacro")
        .to_moveit_configs()
    )

    controllers_yaml = load_yaml("panda_moveit_config", "config/moveit_controllers.yaml")

    move_group_params = moveit_config.to_dict()
    move_group_params.update(controllers_yaml)
    move_group_params.update({
        "use_sim_time": True,
        "trajectory_execution.allowed_start_tolerance": 0.05,
        "trajectory_execution.allowed_execution_duration_scaling": 1.2,
        "trajectory_execution.allowed_goal_duration_margin": 0.5,
    })

    # -------------------------------------------------------------
    # 3. Core Simulation & ROS 2 Nodes
    # -------------------------------------------------------------
    rsp = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description_config.toxml(), 'use_sim_time': True}]
    )

    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([FindPackageShare('ros_gz_sim'), 'launch', 'gz_sim.launch.py'])
        ]),
        launch_arguments={'gz_args': '-r empty.sdf'}.items(),
    )

    spawn_panda = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=['-name', 'panda', '-topic', 'robot_description', '-z', '0.0'],
        output='screen'
    )

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

    static_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_transform_publisher",
        output="log",
        arguments=["0.0", "0.0", "0.0", "0.0", "0.0", "0.0", "world", "panda_link0"],
        parameters=[{"use_sim_time": True}],
    )

    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[move_group_params],
    )

    # -------------------------------------------------------------
    # 4. Controllers & Spawners (Delayed)
    # -------------------------------------------------------------
    jsb = Node(package="controller_manager", executable="spawner", arguments=["joint_state_broadcaster"])
    arm_controller = Node(package="controller_manager", executable="spawner", arguments=["panda_arm_controller"])
    hand_controller = Node(package="controller_manager", executable="spawner", arguments=["hand_controller"])

    delayed_spawners = TimerAction(
        period=5.0,
        actions=[jsb, arm_controller, hand_controller]
    )

    # -------------------------------------------------------------
    # 5. RViz2 Node (Delayed to ensure full stack readiness)
    # -------------------------------------------------------------
    rviz_config_path = os.path.join(moveit_config.package_path, "config", "moveit.rviz")
    
    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="log",
        arguments=["-d", rviz_config_path],
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            moveit_config.planning_pipelines,
            moveit_config.joint_limits,
            {"use_sim_time": True},
        ],
    )

    delayed_rviz = TimerAction(
        period=8.0,
        actions=[rviz_node]
    )

    # -------------------------------------------------------------
    # 6. Return Launch Description
    # -------------------------------------------------------------
    return LaunchDescription(env_vars + [
        rsp, 
        gz_sim, 
        spawn_panda, 
        bridge, 
        static_tf,
        move_group_node,
        delayed_spawners,
        delayed_rviz
    ])