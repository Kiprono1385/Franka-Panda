from launch import LaunchDescription
from launch_ros.actions import Node

from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():

    # ---------------------------------------------------------
    # Load the same Panda MoveIt configuration
    # ---------------------------------------------------------

    moveit_config = (
        MoveItConfigsBuilder(
            "panda",
            package_name="panda_moveit_config"
        )
        .robot_description(
            file_path="config/panda.urdf.xacro"
        )
        .to_moveit_configs()
    )

    # ---------------------------------------------------------
    # Start custom C++ movement node
    # ---------------------------------------------------------

    move_panda_node = Node(
        package="custom_panda",
        executable="move_panda",
        name="panda_move_node",
        output="screen",

        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            moveit_config.joint_limits,

            {
                "use_sim_time": True
            }
        ]
    )

    return LaunchDescription([
        move_panda_node
    ])