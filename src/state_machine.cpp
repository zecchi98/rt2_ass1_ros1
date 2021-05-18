#include "ros/ros.h"
#include "rt2_ass1_ros1/Command.h"
#include "rt2_ass1_ros1/Position.h"
#include "rt2_ass1_ros1/RandomPosition.h"
#include "rt2_ass1_ros1/go_to_pointAction.h"
#include <actionlib/client/simple_action_client.h>
bool send_goal = false;
bool stop_goal=false;
bool start=false;
bool user_interface(rt2_ass1_ros1::Command::Request &req, rt2_ass1_ros1::Command::Response &res){
    if (req.command == "start"){
      // if the user interface has sent such request, then "send_goal" and "start" bool are set to true.
      start = true;
      send_goal=true;
      stop_goal=false;
    }
    if (req.command == "stop"){
      start=false;
      send_goal=false;
      stop_goal=true;
    }
    return true;
}


int main(int argc, char **argv)
{
   ros::init(argc, argv, "state_machine");
   ros::NodeHandle n;
   ros::ServiceServer service= n.advertiseService("/user_interface", user_interface);
   ros::ServiceClient client_rp = n.serviceClient<rt2_ass1_ros1::RandomPosition>("/position_server");
   actionlib::SimpleActionClient<rt2_ass1_ros1::go_to_pointAction> ac("/go_to_point_action", true);

   while(!ac.waitForServer(ros::Duration(1.0))){
     ROS_INFO("Waiting for the go_to_point action server to come up");
   }

   rt2_ass1_ros1::go_to_pointGoal goal;

   rt2_ass1_ros1::RandomPosition rp;
   rp.request.x_max = 5.0;
   rp.request.x_min = -5.0;
   rp.request.y_max = 5.0;
   rp.request.y_min = -5.0;
   while(ros::ok()){
    
    ros::spinOnce();
    if (start && send_goal){
      //a new goal will be asked to the go_to_point_action
      send_goal=false;

      //a new random position will be generated
   		client_rp.call(rp);

      //new position values will be saved and sent to the action
      goal.target_position.x= rp.response.x;
      goal.target_position.y = rp.response.y;
      goal.theta = rp.response.theta;
      std::cout << "\nGoing to the position: x= " << goal.target_position.x << " y= " <<goal.target_position.y << " theta = " <<goal.theta << std::endl;
      ac.sendGoal(goal);
    }
    if(start){

      //if the robot is moving and the action consider the goal to be succeded, then an output will be print

      if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
        ROS_INFO("Hooray, target reached!");
        send_goal=true;
      }
    }
    if(stop_goal){
      //the goal action is deleted if the user interface has sent "stop" in its messages
      stop_goal=false;
      ac.cancelGoal();

    }
   }
   return 0;
}
