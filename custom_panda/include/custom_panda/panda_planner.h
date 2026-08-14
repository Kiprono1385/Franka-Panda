#ifndef __PANDA_PLANNER_H__
#define __PANDA_PLANNER_H__

#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/msg/display_robot_state.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>

namespace panda_planner
{
    class PandaPlanner
    {
    public:

        PandaPlanner(
            const rclcpp::Node::SharedPtr& node,
            const std::string& group_name);

        PandaPlanner(
            const std::string& group_name);

        ~PandaPlanner() {};

        bool planJointTarget(
            const std::vector<double>& joint_target);

        bool planPoseTarget(
            const geometry_msgs::msg::Pose& pose_target);

        bool planPoseTargets(
            const std::vector<geometry_msgs::msg::Pose>& pose_target_vector);

        bool planCartesianPath(
            const std::vector<geometry_msgs::msg::Pose>& pose_target_vector);

        bool executePath(
            bool wait = true);

    private:

        void init(
            const std::string& group_name);

        rclcpp::Node::SharedPtr node_;

        std::shared_ptr<
            moveit::planning_interface::MoveGroupInterface>
            move_group_;

        moveit::planning_interface::MoveGroupInterface::Plan panda_plan_;

        moveit_msgs::msg::RobotTrajectory trajectory_;

        bool is_trajectory_;
    };
}

#endif // __PANDA_PLANNER_H__