import os
import yaml
from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder
from ament_index_python.packages import get_package_share_directory

def load_yaml(package_name, file_path):
    package_path = get_package_share_directory(package_name)
    absolute_file_path = os.path.join(package_path, file_path)
    try:
        with open(absolute_file_path, "r") as file:
            return yaml.safe_load(file)
    except EnvironmentError:
        return None

def generate_launch_description():
    pkg_share = get_package_share_directory("panda_moveit_config")

    # 1. Build MoveIt configurations normally
    moveit_config = (
        MoveItConfigsBuilder("panda", package_name="panda_moveit_config")
        .robot_description(file_path="config/panda.urdf.xacro")
        .to_moveit_configs()
    )

    # 2. Explicitly load the YAML dictionaries into memory
    controllers_yaml = load_yaml("panda_moveit_config", "config/moveit_controllers.yaml")

    # 3. Combine everything into the move_group parameters dictionary
    move_group_params = moveit_config.to_dict()
    move_group_params.update(controllers_yaml)  # <--- Loads the actual parameters, not just a path string!
    move_group_params.update({
        "use_sim_time": True,
        "trajectory_execution.allowed_start_tolerance": 0.05,
        "trajectory_execution.allowed_execution_duration_scaling": 1.2,
        "trajectory_execution.allowed_goal_duration_margin": 0.5,
    })

    # 4. MoveGroup Node
    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[move_group_params],
    )

    # 5. RViz2 Node
    rviz_config_path = os.path.join(
        moveit_config.package_path, "config", "moveit.rviz"
    )
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

    # 6. Static TF
    static_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_transform_publisher",
        output="log",
        arguments=["0.0", "0.0", "0.0", "0.0", "0.0", "0.0", "world", "panda_link0"],
        parameters=[{"use_sim_time": True}],
    )

    return LaunchDescription([
        move_group_node,
        rviz_node,
        static_tf,
    ])