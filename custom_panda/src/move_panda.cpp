#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>

#include <geometry_msgs/msg/pose.hpp>

#include <string>


int main(int argc, char **argv)
{
    // ============================================================
    // ROS 2 INITIALIZATION
    // ============================================================

    rclcpp::init(argc, argv);

    rclcpp::NodeOptions node_options;
    node_options.automatically_declare_parameters_from_overrides(true);

    auto node = rclcpp::Node::make_shared(
        "panda_move_node",
        node_options
    );


    // ============================================================
    // MOVEIT ARM
    // ============================================================

    moveit::planning_interface::MoveGroupInterface arm(
        node,
        "panda_arm"
    );


    // ============================================================
    // MOTION SETTINGS
    // ============================================================

    arm.setPlanningTime(5.0);

    arm.setNumPlanningAttempts(10);

    arm.setMaxVelocityScalingFactor(0.3);

    arm.setMaxAccelerationScalingFactor(0.3);

    arm.setGoalPositionTolerance(0.005);

    arm.setGoalOrientationTolerance(0.01);


    // ============================================================
    // INFORMATION
    // ============================================================

    RCLCPP_INFO(
        node->get_logger(),
        "Planning frame: %s",
        arm.getPlanningFrame().c_str()
    );

    RCLCPP_INFO(
        node->get_logger(),
        "End effector: %s",
        arm.getEndEffectorLink().c_str()
    );


    // ============================================================
    // COMMON END-EFFECTOR ORIENTATION
    // ============================================================

    geometry_msgs::msg::Pose pose;

    pose.orientation.x = 1.0;
    pose.orientation.y = 0.0;
    pose.orientation.z = 0.0;
    pose.orientation.w = 0.0;


    // ============================================================
    // PLAN OBJECT
    // ============================================================

    moveit::planning_interface::MoveGroupInterface::Plan plan;


    // ============================================================
    // POINT 1
    // FORWARD APPROACH
    // ============================================================

    pose.position.x = 0.40;
    pose.position.y = 0.00;
    pose.position.z = 0.30;

    RCLCPP_INFO(
        node->get_logger(),
        "========================================"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "POINT 1 - Forward Approach"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "x = %.3f  y = %.3f  z = %.3f",
        pose.position.x,
        pose.position.y,
        pose.position.z
    );


    arm.setPoseTarget(pose);


    if (
        arm.plan(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to plan POINT 1."
        );

        rclcpp::shutdown();

        return 1;
    }


    if (
        arm.execute(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to execute POINT 1."
        );

        rclcpp::shutdown();

        return 1;
    }


    // ============================================================
    // POINT 2
    // LOWER FORWARD
    // ============================================================

    pose.position.x = 0.40;
    pose.position.y = 0.00;
    pose.position.z = 0.20;

    RCLCPP_INFO(
        node->get_logger(),
        "POINT 2 - Lower Forward"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "x = %.3f  y = %.3f  z = %.3f",
        pose.position.x,
        pose.position.y,
        pose.position.z
    );


    arm.setPoseTarget(pose);


    if (
        arm.plan(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to plan POINT 2."
        );

        rclcpp::shutdown();

        return 1;
    }


    if (
        arm.execute(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to execute POINT 2."
        );

        rclcpp::shutdown();

        return 1;
    }


    // ============================================================
    // POINT 3
    // LIFT
    // ============================================================

    pose.position.x = 0.40;
    pose.position.y = 0.00;
    pose.position.z = 0.30;

    RCLCPP_INFO(
        node->get_logger(),
        "POINT 3 - Lift"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "x = %.3f  y = %.3f  z = %.3f",
        pose.position.x,
        pose.position.y,
        pose.position.z
    );


    arm.setPoseTarget(pose);


    if (
        arm.plan(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to plan POINT 3."
        );

        rclcpp::shutdown();

        return 1;
    }


    if (
        arm.execute(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to execute POINT 3."
        );

        rclcpp::shutdown();

        return 1;
    }


    // ============================================================
    // POINT 4
    // MOVE FORWARD TO SECOND POSITION
    // ============================================================

    pose.position.x = 0.40;
    pose.position.y = 0.30;
    pose.position.z = 0.30;

    RCLCPP_INFO(
        node->get_logger(),
        "POINT 4 - Move Forward"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "x = %.3f  y = %.3f  z = %.3f",
        pose.position.x,
        pose.position.y,
        pose.position.z
    );


    arm.setPoseTarget(pose);


    if (
        arm.plan(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to plan POINT 4."
        );

        rclcpp::shutdown();

        return 1;
    }


    if (
        arm.execute(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to execute POINT 4."
        );

        rclcpp::shutdown();

        return 1;
    }


    // ============================================================
    // POINT 5
    // LOWER AT SECOND POSITION
    // ============================================================

    pose.position.x = 0.40;
    pose.position.y = 0.30;
    pose.position.z = 0.20;

    RCLCPP_INFO(
        node->get_logger(),
        "POINT 5 - Lower"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "x = %.3f  y = %.3f  z = %.3f",
        pose.position.x,
        pose.position.y,
        pose.position.z
    );


    arm.setPoseTarget(pose);


    if (
        arm.plan(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to plan POINT 5."
        );

        rclcpp::shutdown();

        return 1;
    }


    if (
        arm.execute(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to execute POINT 5."
        );

        rclcpp::shutdown();

        return 1;
    }


    // ============================================================
    // POINT 6
    // FINAL CLEAR
    // ============================================================

    pose.position.x = 0.40;
    pose.position.y = 0.30;
    pose.position.z = 0.30;

    RCLCPP_INFO(
        node->get_logger(),
        "POINT 6 - Final Clear"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "x = %.3f  y = %.3f  z = %.3f",
        pose.position.x,
        pose.position.y,
        pose.position.z
    );


    arm.setPoseTarget(pose);


    if (
        arm.plan(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to plan POINT 6."
        );

        rclcpp::shutdown();

        return 1;
    }


    if (
        arm.execute(plan) !=
        moveit::core::MoveItErrorCode::SUCCESS
    )
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Failed to execute POINT 6."
        );

        rclcpp::shutdown();

        return 1;
    }


    // ============================================================
    // COMPLETE
    // ============================================================

    RCLCPP_INFO(
        node->get_logger(),
        "========================================"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "PANDA MOTION SEQUENCE COMPLETE"
    );

    RCLCPP_INFO(
        node->get_logger(),
        "========================================"
    );


    rclcpp::shutdown();

    return 0;
}