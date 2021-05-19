#include "ros/ros.h"
#include "rt2_ass1_std/Command.h"
#include "rt2_ass1_std/Position.h"
#include "rt2_ass1_std/RandomPosition.h"

bool start = false;

bool user_interface(rt2_ass1_std::Command::Request &req, rt2_ass1_std::Command::Response &res){
    if (req.command == "start"){
    	start = true;
    }
    else {
    	start = false;
    }
    return true;
}


int main(int argc, char **argv)
{
   ros::init(argc, argv, "state_machine");
   ros::NodeHandle n;
   ros::ServiceServer service= n.advertiseService("/commandservice", user_interface);
   ros::ServiceClient client_rp = n.serviceClient<rt2_ass1_std::RandomPosition>("/random_position_service");
   ros::ServiceClient client_p = n.serviceClient<rt2_ass1_std::Position>("/go_to_point");
   
   rt2_ass1_std::RandomPosition rp;
   rp.request.x_max = 5.0;
   rp.request.x_min = -5.0;
   rp.request.y_max = 5.0;
   rp.request.y_min = -5.0;
   rt2_ass1_std::Position p;
   
   while(ros::ok()){
   	ros::spinOnce();
   	if (start){
   		client_rp.call(rp);
   		p.request.x = rp.response.x;
   		p.request.y = rp.response.y;
   		p.request.theta = rp.response.theta;
   		std::cout << "\nGoing to the position: x= " << p.request.x << " y= " <<p.request.y << " theta = " <<p.request.theta << std::endl;
   		client_p.call(p);
   		std::cout << "Position reached" << std::endl;
   	}
   }
   return 0;
}
