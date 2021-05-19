
#include <inttypes.h>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include <iostream>

#include "ros2_ass1/srv/command.hpp"
#include "ros2_ass1/srv/random_position.hpp"
#include "ros2_ass1/srv/position.hpp"
#include "rclcpp_components/register_node_macro.hpp"
using namespace std;
using RandomPositionSRV = ros2_ass1::srv::RandomPosition;
using CommandSRV = ros2_ass1::srv::Command;
using PositionSRV = ros2_ass1::srv::Position;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
rclcpp::Node::SharedPtr g_node = nullptr;

bool need_to_send=false;
bool response_from_rdm_position=false;
bool loose_one_cycle=false;
bool start=false;
float X_target=0;
float Y_target=0;
float Theta_target=0;

namespace ros2_ass1
{
  class Client_go_to_point : public rclcpp::Node
  {
  public:
    
    Client_go_to_point()
    : Node("go_to_point_client")
    {


      //the system will try to connect to the go_to_point server every 1 second, and will print a message in case the service has not appear

      client_ = this->create_client<PositionSRV>("/go_to_point");
      while (!client_->wait_for_service(std::chrono::seconds(1))){
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(this->get_logger(), "client interrupted while waiting for service to appear.");
        return;
      }
      RCLCPP_INFO(this->get_logger(), "waiting for service go_to_point to appear...");
      }
      


    this->request_ = std::make_shared<PositionSRV::Request>();
    this->response_ = std::make_shared<PositionSRV::Response>();
    }
    
    void call_server()
    {


    //this function will be called from the timer_callback() of the Server_ros2 class. This void will contact the go_to_point service in order
    //to comunicate if it has to start or stop the robot. Thanks to the "spin_until_future_complete" it will wait until a response is generated

    auto result_future = client_->async_send_request(request_);
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result_future) != rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_ERROR(this->get_logger(), "service call failed :(");
    }
    this->response_=result_future.get();
    }
    


    std::shared_ptr<PositionSRV::Request> request_;
    std::shared_ptr<PositionSRV::Response> response_;
    
  private:
    rclcpp::Client<PositionSRV>::SharedPtr client_;  
  };



class Server_ros2 : public rclcpp::Node
{
public:
  Server_ros2(const rclcpp::NodeOptions & options)
  : Node("Server_ros2", options)
  {

    //A timer_callback is created. Every 100 milliseconds it will be executed. In this void the main code of the system is situated.
    //If a request from the user_interface arrived, then the handle_service void is executed.
    //How does this all works? The user_interface request "start", then "need_to_send" becomes true and a new random position is generated
    //by requesting it. Here there is no "spin_until_future_complete" so a the "response_received_callback" will be executed when a response
    //will be acquired. At that moment "response_from_rdm_position" will be set to true, and so the go_to_point server will be called.

    service_ = this->create_service<CommandSRV>("/commandservice", std::bind(&Server_ros2::handle_service, this, _1, _2, _3));
    timer_ =this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&Server_ros2::timer_callback, this));
    RP_client=this->create_client<RandomPositionSRV>("/random_position_service");



  }

private:

  void handle_service(
  const std::shared_ptr<rmw_request_id_t> request_header,
  const std::shared_ptr<CommandSRV::Request> request,
  const std::shared_ptr<CommandSRV::Response> response)
  {
  
  
  (void)request_header;
   
  RCLCPP_INFO(this->get_logger(), "Service request");          
  if (request->command == "start"){

    //If the system is already running, then it is not necessary to ask to send another goal.
    //For this reason "need_to_send" is set to true only if the system is stopped("start==false")

    if(!start){
      start=true;
      need_to_send=true;
    }
    }
    else if (request->command == "stop"){
      start=false;
      need_to_send=false;
    }

  response->ok=true;
  
  }

  void timer_callback()
	{
        //this below lines have been implemented due to give priority to the "/commandservice" server. Indeed it will loose a cycle of the timer_callback
        //and so the system will check if the server has requested any messages before calling the "timer_callback" again.
        if (loose_one_cycle)
					{
						loose_one_cycle=false;
					}
        else{
      
          auto node_go_to_point = std::make_shared<Client_go_to_point>();
          auto rdm_position= std::make_shared<RandomPositionSRV::Request>();
              
          if (need_to_send){
            //By setting it to false, it will call the server just one time
            need_to_send=false;

            rdm_position->x_max=5.0;
            rdm_position->x_min=-5.0;
            rdm_position->y_max=5.0;
            rdm_position->y_min=-5.0;

            //Here is defined the callback which will be call when the "random_position_service" will generate a response
            using ServiceResponseFuture =rclcpp::Client<RandomPositionSRV>::SharedFuture;
            auto response_received_callback= [this] (ServiceResponseFuture future){
              X_target=future.get()->x;
              Y_target=future.get()->y;
              Theta_target=future.get()->theta;
              response_from_rdm_position=true;
            };



            auto future_result= RP_client->async_send_request(rdm_position, response_received_callback);
          
          }
          if(response_from_rdm_position ){

            response_from_rdm_position=false;

            //A request to the go_to_point will be generated.

            node_go_to_point->request_->x=X_target;
            node_go_to_point->request_->y=Y_target;
            node_go_to_point->request_->theta=Theta_target;
            RCLCPP_INFO(this->get_logger(), "Going to position: x:%f y:%f theta:%f",node_go_to_point->request_->x,node_go_to_point->request_->y,node_go_to_point->request_->theta);
            node_go_to_point->call_server();
            
            //need_to_send is set to true in order to start going to the next position
            need_to_send=true;
            loose_one_cycle=true;
          }
              
        }		
	}



  rclcpp::Service<CommandSRV>::SharedPtr service_;
	rclcpp::TimerBase::SharedPtr timer_;
	rclcpp::Client<RandomPositionSRV>::SharedPtr RP_client;
};


}

RCLCPP_COMPONENTS_REGISTER_NODE(ros2_ass1::Server_ros2) 
