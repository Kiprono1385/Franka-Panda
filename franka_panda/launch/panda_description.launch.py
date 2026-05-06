import os

from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():

    # Get the package directory
    share_dir = get_package_share_directory("franka_panda")

    # Path to the URDF file
    urdf_file = os.path.join(share_dir, "description", "panda.urdf")

    # Read the URDF content
    with open(urdf_file, "r", encoding="utf-8") as f:
        robot_description = f.read()

    # UPDATED: Path to the default.rviz file inside rviz_config folder
    rviz_config = os.path.join(share_dir, "rviz_config", "default.rviz")

    # Node: Robot State Publisher
    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="screen",
        parameters=[{"robot_description": robot_description}],
    )

    # Node: Joint State Publisher GUI
    joint_state_publisher_node = Node(
        package="joint_state_publisher_gui",
        executable="joint_state_publisher_gui",
        output="screen",
    )

    # Node: RViz2
    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        output="screen",
        # This will now use default.rviz if it exists
        arguments=["-d", rviz_config] if os.path.exists(rviz_config) else [],
    )

    return LaunchDescription([
        robot_state_publisher_node,
        joint_state_publisher_node,
        rviz_node,
    ])