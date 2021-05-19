# First Assignment of the Research Track 2 course (Robotics Engineering / JEMARO, Unige)

#How to run the code:
- First of all you need to have the main branch download in a ros1 folder.
- Then you will have to download this package in a ros2 workspace. "ros2_ass1" need to be in the /src folder
- You should compile everything and then compile you ros1_bridge
- In the first shell run source ros1 and then run:
```
roslaunch rt2_ass1_std sim_ros2.launch
```
- In a second shell source ros1 and ros2 and run the bridge:
```
ros2 run ros1_bridge dynamic_bridge
```
- In a third shell source ros2 and run:
```
ros2 launch ros2_ass1 ros2_ass1_launcher.py
```
- If everything has worked properly, you should see gazebo opened, and the user interface running in the first shell.
You should wait that the third shell complete to load the comonents before executing commands with the user interface.
From now on, you should be able to follow user_interface instruction to move the robot

# What is the architecture of the system?

It is exactly the same of the standard package, but "position_server.cpp" and "robot_fsm.cpp" are loaded as componentes in a ros2 workspace.
"robot_fsm.cpp" is the only file which has a small different structure. Has it is explained in the code, through a timer_callback() which is executed every 100 millieseconds all the main code is controlled. It will check if the boolean "need_to_send" has became true (due to a request from the user_interface), and will decide when a new random position has to be requested in order to send a new goal to the "go_to_point" server. 



