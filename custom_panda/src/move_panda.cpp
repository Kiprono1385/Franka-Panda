#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <moveit_msgs/msg/attached_collision_object.hpp>
#include <moveit_msgs/msg/planning_scene.hpp>
#include <moveit_msgs/msg/object_color.hpp>

// --- SERVICE HEADER ---
#include "linkattacher_msgs/srv/attach_link.hpp"
#include "linkattacher_msgs/srv/detach_link.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto const node = rclcpp::Node::make_shared("panda_safe_picker");

    // Spin single-threaded executor in background thread for async operations
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    std::future<void> async_executor = std::async(std::launch::async, [&executor]() { executor.spin(); });

    // --- MOVEIT 2 INTERFACES ---
    moveit::planning_interface::MoveGroupInterface arm_group(node, "panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group(node, "hand");
    moveit::planning_interface::PlanningSceneInterface psi;

    // Configure planner performance scaling factors
    arm_group.setMaxVelocityScalingFactor(0.3);
    arm_group.setMaxAccelerationScalingFactor(0.3);

    // --- SERVICE CLIENTS ---
    auto attach_client = node->create_client<linkattacher_msgs::srv::AttachLink>("/ATTACHLINK");
    auto detach_client = node->create_client<linkattacher_msgs::srv::DetachLink>("/DETACHLINK");

    // Orthogonal downward orientation quaternion for the alternative side pair
    geometry_msgs::msg::Quaternion downward_orientation;
    downward_orientation.x = 0.38268;
    downward_orientation.y = 0.92388;
    downward_orientation.z = 0.0;
    downward_orientation.w = 0.0;

    // 1. Setup the "Target Cube" (Red)
    auto const target_cube = [] {
        moveit_msgs::msg::CollisionObject obj;
        obj.header.frame_id = "panda_link0";
        obj.id = "target_cube";
        shape_msgs::msg::SolidPrimitive primitive;
        primitive.type = shape_msgs::msg::SolidPrimitive::BOX;
        primitive.dimensions = {0.05, 0.05, 0.05};
        geometry_msgs::msg::Pose pose;
        pose.orientation.w = 1.0; 
        pose.position.x = 0.40; pose.position.y = 0.0; pose.position.z = 0.025; 
        obj.primitives.push_back(primitive);
        obj.primitive_poses.push_back(pose);
        obj.operation = moveit_msgs::msg::CollisionObject::ADD;
        return obj;
    }();

    // 2. Setup the "Table Surface" (Brown)
    auto const table_surface = [] {
        moveit_msgs::msg::CollisionObject obj;
        obj.header.frame_id = "panda_link0";
        obj.id = "table_surface";
        shape_msgs::msg::SolidPrimitive primitive;
        primitive.type = shape_msgs::msg::SolidPrimitive::BOX;
        primitive.dimensions = {0.8, 1.5, 0.03}; 
        geometry_msgs::msg::Pose pose;
        pose.orientation.z = 0;
        pose.orientation.w = 0;
        pose.position.x = 0.245; pose.position.y = 0.0; pose.position.z = -0.015; 
        obj.primitives.push_back(primitive);
        obj.primitive_poses.push_back(pose);
        obj.operation = moveit_msgs::msg::CollisionObject::ADD;
        return obj;
    }();

    psi.applyCollisionObject(target_cube);
    psi.applyCollisionObject(table_surface);

    // 3. Apply Colors
    moveit_msgs::msg::PlanningScene planning_scene;
    planning_scene.is_diff = true;
    moveit_msgs::msg::ObjectColor cube_color;
    cube_color.id = "target_cube";
    cube_color.color.r = 1.0; cube_color.color.g = 0.0; cube_color.color.b = 0.0; cube_color.color.a = 1.0;
    moveit_msgs::msg::ObjectColor table_color;
    table_color.id = "table_surface";
    table_color.color.r = 0.58; table_color.color.g = 0.29; table_color.color.b = 0.0; table_color.color.a = 0.8;
    planning_scene.object_colors.push_back(cube_color);
    planning_scene.object_colors.push_back(table_color);
    psi.applyPlanningScene(planning_scene);

    // Franka Hand Joint Targets
    std::vector<double> gripper_open = {0.04, 0.04};
    std::vector<double> gripper_close = {0.00, 0.00};

    // 4. STAGE: Approach
    geometry_msgs::msg::Pose approach_pose;
    approach_pose.orientation = downward_orientation;
    approach_pose.position.x = 0.40; approach_pose.position.y = 0.0; approach_pose.position.z = 0.20;
    
    RCLCPP_INFO(node->get_logger(), "Executing STAGE: Approach");
    arm_group.setPoseTarget(approach_pose);
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    if (arm_group.plan(plan) != moveit::core::MoveItErrorCode::SUCCESS) {
        RCLCPP_ERROR(node->get_logger(), "Failed to plan Approach pose!");
        return 1;
    }
    arm_group.execute(plan);

    // STAGE: Cartesian Lowering
    geometry_msgs::msg::Pose pick_pose = approach_pose;
    pick_pose.position.z = 0.065; 
    
    std::vector<geometry_msgs::msg::Pose> cartesian_waypoints;
    cartesian_waypoints.push_back(pick_pose);

    RCLCPP_INFO(node->get_logger(), "STAGE: Cartesian Lowering");
    constexpr double jump_threshold = 0.0;
    constexpr double eef_step = 0.01;
    moveit_msgs::msg::RobotTrajectory trajectory;
    double fraction = arm_group.computeCartesianPath(cartesian_waypoints, eef_step, jump_threshold, trajectory);
    RCLCPP_INFO(node->get_logger(), "Visualizing Cartesian Lowering path (%.2f%% achieved)", fraction * 100.0);
    
    if (fraction < 0.9) {
        RCLCPP_ERROR(node->get_logger(), "Cartesian path failed to compute fully!");
        return 1;
    }
    arm_group.execute(trajectory);

    // --- STAGE: Enabling 'Ghost' mode (Attached Collision Object) ---
    moveit_msgs::msg::AttachedCollisionObject allow_touch;
    allow_touch.link_name = "panda_hand"; 
    allow_touch.object = target_cube;
    allow_touch.object.operation = moveit_msgs::msg::CollisionObject::ADD;
    allow_touch.touch_links = {"panda_hand", "panda_leftfinger", "panda_rightfinger", "panda_link8"};
    psi.applyAttachedCollisionObject(allow_touch);

    // STAGE: Grasp (Close Gripper)
    RCLCPP_INFO(node->get_logger(), "STAGE: Grasp - Sending close command");
    hand_group.setJointValueTarget(gripper_close);
    if (hand_group.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS) {
        hand_group.execute(plan);
        
        // --- ATTACH LINK IN GAZEBO ---
        auto request = std::make_shared<linkattacher_msgs::srv::AttachLink::Request>();
        request->model1_name = "panda";
        request->link1_name = "panda_hand";
        request->model2_name = "target_cube";
        request->link2_name = "link";

        if (!attach_client->wait_for_service(std::chrono::seconds(5))) {
            RCLCPP_ERROR(node->get_logger(), "Service /ATTACHLINK not available!");
        } else {
            auto result = attach_client->async_send_request(request);
        }
    }

    // 5. STAGE: Cartesian Lift
    geometry_msgs::msg::Pose above_pick_pose = approach_pose;
    std::vector<geometry_msgs::msg::Pose> lift_waypoints = {above_pick_pose};

    RCLCPP_INFO(node->get_logger(), "STAGE: Cartesian Lift");
    fraction = arm_group.computeCartesianPath(lift_waypoints, eef_step, jump_threshold, trajectory);
    if (fraction > 0.9) {
        arm_group.execute(trajectory);
    }

    // 6. STAGE: Above place position — free-space transit
    geometry_msgs::msg::Pose above_place_pose;
    above_place_pose.orientation = downward_orientation;
    above_place_pose.position.x = 0.25; above_place_pose.position.y = 0.30; above_place_pose.position.z = 0.20;

    RCLCPP_INFO(node->get_logger(), "STAGE: Move to above place pose (free-space)");
    arm_group.setPoseTarget(above_place_pose);
    if (arm_group.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS) {
        arm_group.execute(plan);
    }

    // 7. STAGE: Place position
    geometry_msgs::msg::Pose place_pose = above_place_pose;
    place_pose.position.z = 0.065;
    std::vector<geometry_msgs::msg::Pose> final_drop_waypoints = {place_pose};

    RCLCPP_INFO(node->get_logger(), "STAGE: Cartesian Place");
    fraction = arm_group.computeCartesianPath(final_drop_waypoints, eef_step, jump_threshold, trajectory);
    if (fraction > 0.9) {
        arm_group.execute(trajectory);
    }

    // STAGE: Opening of gripper and Detaching
    RCLCPP_INFO(node->get_logger(), "STAGE: Opening Gripper and Detaching Cube");
    hand_group.setJointValueTarget(gripper_open);
    if (hand_group.plan(plan) == moveit::core::MoveItErrorCode::SUCCESS) {
        hand_group.execute(plan);
        
        auto detach_request = std::make_shared<linkattacher_msgs::srv::DetachLink::Request>();
        detach_request->model1_name = "panda";
        detach_request->link1_name = "panda_hand";
        detach_request->model2_name = "target_cube";
        detach_request->link2_name = "link";

        if (!detach_client->wait_for_service(std::chrono::seconds(10))) {
            RCLCPP_ERROR(node->get_logger(), "Service /DETACHLINK still not available!");
        } else {
            auto result = detach_client->async_send_request(detach_request);
            
            moveit_msgs::msg::AttachedCollisionObject detach_object;
            detach_object.object.id = "target_cube";
            detach_object.link_name = "panda_hand";
            detach_object.object.operation = moveit_msgs::msg::CollisionObject::REMOVE;
            psi.applyAttachedCollisionObject(detach_object);
            RCLCPP_INFO(node->get_logger(), "Gazebo: Cube successfully detached.");
        }
    }

    // Final Stage: Clear move back up
    geometry_msgs::msg::Pose clear_pose = above_place_pose;
    std::vector<geometry_msgs::msg::Pose> clear_waypoints = {clear_pose};

    RCLCPP_INFO(node->get_logger(), "STAGE: Final clearing move");
    fraction = arm_group.computeCartesianPath(clear_waypoints, eef_step, jump_threshold, trajectory);
    if (fraction > 0.9) {
        arm_group.execute(trajectory);
    }

    RCLCPP_INFO(node->get_logger(), "Pick and Place Sequence Complete!");
    rclcpp::shutdown();
    async_executor.wait();
    return 0;
}