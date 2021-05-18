import rospy
import time
from rt2_ass1_std.srv import Command

def main():
    rospy.init_node('user_interface')
    ui_client = rospy.ServiceProxy('/commandservice', Command)
    time.sleep(5)
    rate = rospy.Rate(20)
    x = 0
    while not rospy.is_shutdown() and x!=-1:
        x = int(input("\nPress -1 to kill the user_interface\nPress 0 to stop the robot\nPress 1 to start the robot\nChoice:"))
        if (x == 1):
            ui_client("start")
        elif (x==0 or x==-1):
            print("Please wait, the robot is going to stop when the position will be reached")
            rospy.wait_for_service('/commandservice')
            try:
                ui_client("stop")
            except rospy.ServiceException as e:
                print("Service call failed")

if __name__ == '__main__':
    main()
