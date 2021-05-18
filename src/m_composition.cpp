#include <memory>

#include "ros2_ass1/robot_fsm.hpp"
#include "ros2_ass1/position_server.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  rclcpp::init(argc, argv);
  rclcpp::executors::SingleThreadedExecutor exec;
  rclcpp::NodeOptions options;

  auto robot_fsm = std::make_shared<ros2_ass1::Server_rdm_position>(options);
  exec.add_node(robot_fsm);
  auto position_server = std::make_shared<ros2_ass1::Server>(options);
  exec.add_node(position_server);

  exec.spin();

  rclcpp::shutdown();

  return 0;
}
