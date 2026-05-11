import os
from launch import LaunchDescription
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder

def generate_launch_description():
    # 1. Load all MoveIt configurations (URDF, SRDF, Kinematics, Planners)
    moveit_config = (
        MoveItConfigsBuilder("panda", package_name="panda_moveit_config")
        .to_moveit_configs()
    )

    # 2. Planning Scene Monitor Parameters
    # This helps MoveIt stay synced with Gazebo's clock
    sim_time_param = {"use_sim_time": True}

    # 3. MoveGroup Node (The Brain)
    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            sim_time_param,
        ],
    )

    # 4. RViz Node (The GUI)
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
            sim_time_param,
        ],
    )

    # 5. Robot State Publisher (The Nervous System)
    # Essential so RViz knows where the links are
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="both",
        parameters=[
            moveit_config.robot_description,
            sim_time_param,
        ],
    )

    # 6. Static TF (World to Robot Base)
    # Replaces the 'virtual_joints' logic from the demo launch
    static_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_transform_publisher",
        output="log",
        arguments=["0.0", "0.0", "0.0", "0.0", "0.0", "0.0", "world", "panda_link0"],
        parameters=[sim_time_param],
    )

    return LaunchDescription([
        move_group_node,
        rviz_node,
        robot_state_publisher,
        static_tf,
    ])