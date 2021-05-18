import rospy
import time
from rt2_ass1_ros1.srv import Command
def main():
    rospy.init_node('user_interface')
    ui_client = rospy.ServiceProxy('/user_interface', Command)
    #time.sleep(10)
    rate = rospy.Rate(20)
    x=0
    while not rospy.is_shutdown() and x!=-1:
        x = int(input("\nPress 0 to stop the robot\nPress 1 to start the robot\nChoice:"))
        if (x == 1):
            ui_client("start")
        elif (x==0):
            ui_client("stop")
            
if __name__ == '__main__':
    main()
