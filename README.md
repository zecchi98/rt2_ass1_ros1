### Research_track_2 assignment_1:

This Readme is related to the third branch, reguarding implementi vrep with ros.

### How to run the package:
- This package need to be inside your workspace/src. Then you need to execute "catkin_make"

- open a new shell;source ros1; roslaunch pioneer_ctrl sim_small.launch

- open a new shell;source ros1; execute "./coppeliaSim.sh" from the folder in which CoppeliaSim is installed; Then you should open the scene that I have created which is "pioneerROS.ttt", and it's located in the main folder of the package. Then you have to run the scene.


### How to use it:
If you correctly run the package you should see two shells working. In the first shell 4 nodes will run, "position_service.cpp", "user_interface.py", "state_machine.cpp", "go_to_point_action.py"; And you will see the output of all of them.
As the user interface will suggest you, if you press 1 the robot will start moving. By pressing 0 at any time, the robot will stop. By pressing -1 the user_interface will be killed, but if you want to close everything you should press Ctrl+C.


### What is the architecture of the system?
- "user_interface" comunicate to the server in "state_machine" if the system need to be stopped or started.
- "state_machine" has a server which wait messages from the "user_interface"; It can communicate to the server in "position_service" in order to elaborate a new random goal; It can communicate to the "go_to_point_action" action in order to create a new goal or delete the current goal.
- "go_to_point_action" has an action through which it will decide if the robot need to be stopped or run. It will comunicate to vrep with a subscriber and a publisher in order to read the position of the robot and to comunicate the new values of velocities.


