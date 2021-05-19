
#include <inttypes.h>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "ros2_ass1/srv/random_position.hpp"
#include <iostream>

using namespace std;
using RandomPositionSRV = ros2_ass1::srv::RandomPosition;
rclcpp::Node::SharedPtr g_node = nullptr;


using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;



namespace ros2_ass1
{
class Server_rdm_position : public rclcpp::Node
{
public:
  Server_rdm_position(const rclcpp::NodeOptions & options)
  : Node("Server_rdm_position", options)
  {
    //when the server will be called, then the handle_service void will be executed in order to genereate a response
    service_ = this->create_service<RandomPositionSRV>("random_position_service", std::bind(&Server_rdm_position::handle_service, this, _1, _2, _3));
   }
   

private:

double randMToN(double M, double N)
{     return M + (rand() / ( RAND_MAX / (N-M) ) ) ; }

void handle_service(
  const std::shared_ptr<rmw_request_id_t> request_header,
  const std::shared_ptr<RandomPositionSRV::Request> request,
  const std::shared_ptr<RandomPositionSRV::Response> response)
{
  (void)request_header;


  //here the rensponse will be generated
  RCLCPP_INFO(this->get_logger(), "Service request");
  response->x= randMToN(request->x_min, request->x_max);
  response->y= randMToN(request->y_min, request->y_max);
  response->theta = randMToN(-3.14, 3.14);


}

  rclcpp::Service<RandomPositionSRV>::SharedPtr service_;
};
}


RCLCPP_COMPONENTS_REGISTER_NODE(ros2_ass1::Server_rdm_position) 
