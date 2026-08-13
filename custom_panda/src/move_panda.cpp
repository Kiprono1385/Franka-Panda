#include <chrono>
#include <memory>
#include <thread>

#include <rclcpp/rclcpp.hpp>

#include <geometry_msgs/msg/pose.hpp>

#include <moveit/move_group_interface/move_group_interface.h>

using namespace std::chrono_literals;


int main(int argc, char * argv[])
{
    // =========================================================
    // Initialize ROS 2
    // =========================================================

    rclcpp::init(argc, argv);

    auto node = rclcpp::Node::make_shared("panda_move_node");

    // =========================================================
    // ROS 2 executor
    // =========================================================

    rclcpp::executors::SingleThreadedExecutor executor;

    executor.add_node(node);

    std::thread spinner(
        [&executor]()
        {
            executor.spin();
        }
    );

    // =========================================================
    // MoveIt planning group
    // =========================================================

    static const std::string PLANNING_GROUP = "panda_arm";

    moveit::planning_interface::MoveGroupInterface move_group(
        node,
        PLANNING_GROUP
    );

    // =========================================================
    // MoveIt configuration
    // =========================================================

    move_group.setPlanningTime(5.0);

    move_group.setNumPlanningAttempts(10);

    move_group.setMaxVelocityScalingFactor(0.3);

    move_group.setMaxAccelerationScalingFactor(0.3);

    // =========================================================
    // Print information
    // =========================================================

    RCLCPP_INFO(
        node->get_logger(),
        "======================================"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "Panda MoveIt Motion Program"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "======================================"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "Planning frame: %s",
        move_group.getPlanningFrame().c_str()
    );

    RCLCPP_INFO(
        node->get_logger(),
        "End effector link: %s",
        move_group.getEndEffectorLink().c_str()
    );

    RCLCPP_INFO(
        node->get_logger(),
        "Planning group: %s",
        PLANNING_GROUP.c_str()
    );

    // =========================================================
    // POINT 1
    //
    // Measured from:
    // panda_link0 -> panda_hand
    //
    // Position:
    // X = 0.280 m
    // Y = -0.454 m
    // Z = 0.125 m
    //
    // Orientation:
    // Quaternion [1.0, 0.0, 0.0, 0.0]
    // RPY approximately [-180, 0, 0] degrees
    // =========================================================

    geometry_msgs::msg::Pose point_1;

    point_1.position.x = 0.280;
    point_1.position.y = -0.454;
    point_1.position.z = 0.125;

    point_1.orientation.x = 1.0;
    point_1.orientation.y = 0.0;
    point_1.orientation.z = 0.0;
    point_1.orientation.w = 0.0;

    // =========================================================
    // Move to Point 1
    // =========================================================

    RCLCPP_INFO(
        node->get_logger(),
        "======================================"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "POINT 1"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "X: %.3f m",
        point_1.position.x
    );

    RCLCPP_INFO(
        node->get_logger(),
        "Y: %.3f m",
        point_1.position.y
    );

    RCLCPP_INFO(
        node->get_logger(),
        "Z: %.3f m",
        point_1.position.z
    );

    RCLCPP_INFO(
        node->get_logger(),
        "======================================"
    );

    // Clear any previous target
    move_group.clearPoseTargets();

    // Set Point 1
    move_group.setPoseTarget(point_1);

    // =========================================================
    // Plan to Point 1
    // =========================================================

    moveit::planning_interface::MoveGroupInterface::Plan plan_1;

    bool success_1 =
        (move_group.plan(plan_1) ==
         moveit::core::MoveItErrorCode::SUCCESS);

    // =========================================================
    // Execute Point 1
    // =========================================================

    if (success_1)
    {
        RCLCPP_INFO(
            node->get_logger(),
            "Planning to Point 1 successful."
        );

        auto result = move_group.execute(plan_1);

        if (result == moveit::core::MoveItErrorCode::SUCCESS)
        {
            RCLCPP_INFO(
                node->get_logger(),
                "Successfully reached Point 1."
            );
        }
        else
        {
            RCLCPP_ERROR(
                node->get_logger(),
                "Failed to execute trajectory to Point 1."
            );

            move_group.clearPoseTargets();

            rclcpp::shutdown();
            spinner.join();

            return 1;
        }
    }
    else
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Planning to Point 1 failed."
        );

        move_group.clearPoseTargets();

        rclcpp::shutdown();
        spinner.join();

        return 1;
    }

    // =========================================================
    // Wait at Point 1
    // =========================================================

    RCLCPP_INFO(
        node->get_logger(),
        "Waiting at Point 1..."
    );

    rclcpp::sleep_for(2s);

    // =========================================================
    // POINT 2
    //
    // Measured from:
    // panda_link0 -> panda_hand
    //
    // Position:
    // X = 0.269 m
    // Y = 0.665 m
    // Z = 0.211 m
    //
    // Orientation:
    // Quaternion [1.0, 0.0, 0.0, 0.0]
    // RPY approximately [-180, 0, 0] degrees
    // =========================================================

    geometry_msgs::msg::Pose point_2;

    point_2.position.x = 0.269;
    point_2.position.y = 0.665;
    point_2.position.z = 0.211;

    point_2.orientation.x = 1.0;
    point_2.orientation.y = 0.0;
    point_2.orientation.z = 0.0;
    point_2.orientation.w = 0.0;

    // =========================================================
    // Move to Point 2
    // =========================================================

    RCLCPP_INFO(
        node->get_logger(),
        "======================================"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "POINT 2"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "X: %.3f m",
        point_2.position.x
    );

    RCLCPP_INFO(
        node->get_logger(),
        "Y: %.3f m",
        point_2.position.y
    );

    RCLCPP_INFO(
        node->get_logger(),
        "Z: %.3f m",
        point_2.position.z
    );

    RCLCPP_INFO(
        node->get_logger(),
        "======================================"
    );

    // Clear Point 1
    move_group.clearPoseTargets();

    // Set Point 2
    move_group.setPoseTarget(point_2);

    // =========================================================
    // Plan to Point 2
    // =========================================================

    moveit::planning_interface::MoveGroupInterface::Plan plan_2;

    bool success_2 =
        (move_group.plan(plan_2) ==
         moveit::core::MoveItErrorCode::SUCCESS);

    // =========================================================
    // Execute Point 2
    // =========================================================

    if (success_2)
    {
        RCLCPP_INFO(
            node->get_logger(),
            "Planning to Point 2 successful."
        );

        auto result = move_group.execute(plan_2);

        if (result == moveit::core::MoveItErrorCode::SUCCESS)
        {
            RCLCPP_INFO(
                node->get_logger(),
                "Successfully reached Point 2."
            );
        }
        else
        {
            RCLCPP_ERROR(
                node->get_logger(),
                "Failed to execute trajectory to Point 2."
            );
        }
    }
    else
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Planning to Point 2 failed."
        );
    }

    // =========================================================
    // Wait at Point 2
    // =========================================================

    RCLCPP_INFO(
        node->get_logger(),
        "Waiting at Point 2..."
    );

    rclcpp::sleep_for(2s);

    // =========================================================
    // Cleanup
    // =========================================================

    move_group.clearPoseTargets();

    RCLCPP_INFO(
        node->get_logger(),
        "======================================"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "Panda movement program finished."
    );

    RCLCPP_INFO(
        node->get_logger(),
        "======================================"
    );

    rclcpp::shutdown();

    spinner.join();

    return 0;
}