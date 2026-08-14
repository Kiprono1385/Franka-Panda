#include "custom_panda/panda_planner.h"

#include <geometry_msgs/msg/pose.hpp>
#include <moveit_msgs/msg/robot_trajectory.hpp>

namespace panda_planner
{

PandaPlanner::PandaPlanner(
    const rclcpp::Node::SharedPtr& node,
    const std::string& group_name)
    : node_(node),
      is_trajectory_(false)
{
    init(group_name);
}


PandaPlanner::PandaPlanner(
    const std::string& group_name)
    : node_(rclcpp::Node::make_shared("panda_planner")),
      is_trajectory_(false)
{
    init(group_name);
}


void PandaPlanner::init(
    const std::string& group_name)
{
    move_group_ =
        std::make_shared<
            moveit::planning_interface::MoveGroupInterface>(
            node_,
            group_name);

    move_group_->setMaxVelocityScalingFactor(0.3);
    move_group_->setMaxAccelerationScalingFactor(0.3);

    is_trajectory_ = false;
}


bool PandaPlanner::planJointTarget(
    const std::vector<double>& joint_target)
{
    move_group_->setJointValueTarget(joint_target);

    auto result =
        move_group_->plan(panda_plan_);

    if (result != moveit::core::MoveItErrorCode::SUCCESS)
    {
        RCLCPP_ERROR(
            node_->get_logger(),
            "Failed to plan joint target.");

        is_trajectory_ = false;

        return false;
    }

    is_trajectory_ = true;

    return true;
}


bool PandaPlanner::planPoseTarget(
    const geometry_msgs::msg::Pose& pose_target)
{
    move_group_->setPoseTarget(pose_target);

    auto result =
        move_group_->plan(panda_plan_);

    if (result != moveit::core::MoveItErrorCode::SUCCESS)
    {
        RCLCPP_ERROR(
            node_->get_logger(),
            "Failed to plan pose target.");

        is_trajectory_ = false;

        return false;
    }

    is_trajectory_ = true;

    return true;
}


bool PandaPlanner::planPoseTargets(
    const std::vector<geometry_msgs::msg::Pose>& pose_target_vector)
{
    move_group_->setPoseTargets(pose_target_vector);

    auto result =
        move_group_->plan(panda_plan_);

    if (result != moveit::core::MoveItErrorCode::SUCCESS)
    {
        RCLCPP_ERROR(
            node_->get_logger(),
            "Failed to plan pose targets.");

        is_trajectory_ = false;

        return false;
    }

    is_trajectory_ = true;

    return true;
}


bool PandaPlanner::planCartesianPath(
    const std::vector<geometry_msgs::msg::Pose>& pose_target_vector)
{
    constexpr double eef_step = 0.01;
    constexpr double jump_threshold = 0.0;

    double fraction =
        move_group_->computeCartesianPath(
            pose_target_vector,
            eef_step,
            jump_threshold,
            trajectory_);

    RCLCPP_INFO(
        node_->get_logger(),
        "Cartesian path: %.2f%% achieved",
        fraction * 100.0);

    if (fraction < 0.9)
    {
        RCLCPP_ERROR(
            node_->get_logger(),
            "Cartesian path planning failed.");

        is_trajectory_ = false;

        return false;
    }

    panda_plan_.trajectory_ = trajectory_;

    is_trajectory_ = true;

    return true;
}


bool PandaPlanner::executePath(
    bool wait)
{
    if (!is_trajectory_)
    {
        RCLCPP_ERROR(
            node_->get_logger(),
            "No planned path available to execute.");

        return false;
    }

    moveit::core::MoveItErrorCode result;

    if (wait)
    {
        result = move_group_->execute(panda_plan_);
    }
    else
    {
        result = move_group_->asyncExecute(panda_plan_);
    }

    if (result != moveit::core::MoveItErrorCode::SUCCESS)
    {
        RCLCPP_ERROR(
            node_->get_logger(),
            "Failed to execute planned path.");

        is_trajectory_ = false;

        return false;
    }

    if (wait)
    {
        move_group_->stop();
    }

    is_trajectory_ = false;

    return true;
}

}  // namespace panda_planner