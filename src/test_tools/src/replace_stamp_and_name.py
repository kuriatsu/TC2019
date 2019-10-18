#!/usr/bin/env python
import os.path                                                          
import rosbag                                                           
import rospy
def replace_messages(bag, duration, name):                  
    new = os.path.splitext(bag)[0] + '.mod.bag'                           
#    os.rename(bag, orig)                                                    
    inbag = rosbag.Bag(bag, 'r')                                           
    outbag = rosbag.Bag(new, 'w')        

    print "replace bag file" 
    for t, msg, ts in inbag.read_messages():                                
     if t == "/velodyne_points":
        msg.header.stamp=msg.header.stamp-rospy.Duration(duration)
        msg.header.frame_id=name
        outbag.write('/velodyne_points_zmp', msg, ts-rospy.Duration(duration))
    inbag.close()                                                           
    outbag.close()

if __name__ == '__main__':
  import sys
  if len(sys.argv) >= 2:
    replace_messages(sys.argv[1],float(sys.argv[2]),sys.argv[3])
  else:
    print "usage: %s bag1 [bag2 bag3 ...]" % sys.argv[0]
