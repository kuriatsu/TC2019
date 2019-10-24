#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>


class WhiteLineStopper
{
private:
    ros::Subscriber sub_joy;
    ros::Subscriber sub_ndt_pose;
    ros::Subscriber sub_twist;
    ros::Publisher pub_twist;


    bool go_frag;
    bool intrusion_frag;

    float min_side;
    float max_side;
    float min_line;
    float max_line;

public:
    WhiteLineStopper();

private:
    void joyCallback(const sensor_msgs::Joy &in_msg);
    void twistCallback(const geometry_msgs::TwistStamped &in_msg);
    // void timerCallback(const ros::TimerEvent&);
    void poseCallback(const geometry_msgs::PoseStamped &in_pose);
};


WhiteLineStopper::WhiteLineStopper(): go_frag(true), intrusion_frag(false)
{
    ros::NodeHandle n;
    sub_joy = n.subscribe("/joy", 1, &WhiteLineStopper::joyCallback, this);
    sub_twist = n.subscribe("/twist_cmd_safe", 1, &WhiteLineStopper::twistCallback, this);
    sub_twist = n.subscribe("/ndt_pose", 1, &WhiteLineStopper::poseCallback, this);
    pub_twist = n.advertise<geometry_msgs::Twist>("/twist_cmd_helper", 1);

    min_side = 51.4;
    max_side = 54.2;
    min_line = -22.5;
    max_line = -21.9;

    // ros::Duration(1).sleep();
    // timer = n.createTimer(ros::Duration(pub_rate), &WhiteLineStopper::timerCallback, this);
}


void WhiteLineStopper::twistCallback(const geometry_msgs::TwistStamped &in_msg)
{
    geometry_msgs::Twist in_twist;
    geometry_msgs::Twist out_twist;

    in_twist.linear.x = in_msg.twist.linear.x;
    in_twist.angular.z = in_msg.twist.angular.z;

    if (!go_frag)
    {
        out_twist.linear.x = 0.0;
        out_twist.angular.z = 0.0;
    }
    else
    {
        out_twist = in_twist;
    }

    pub_twist.publish(out_twist);

}


void WhiteLineStopper::poseCallback(const geometry_msgs::PoseStamped &in_msg)
{
    // geometry_msgs::PoseStamped in_msg;

    // in_pose = in_msg.pose.position;
    if (min_side < in_msg.pose.position.x && in_msg.pose.position.x < max_side)
    {
        if (min_line < in_msg.pose.position.y && in_msg.pose.position.y < max_line)
        {
            if (!intrusion_frag)
            {
                intrusion_frag = true;
                ROS_INFO("intrusion");
                go_frag = false;
            }
        }
    }
    else
    {
        intrusion_frag = false;
    }
}


void WhiteLineStopper::joyCallback(const sensor_msgs::Joy &in_msg)
{
    if(go_frag && in_msg.buttons[3])
    {
        go_frag == true;
    }
}


// void WhiteLineStopper::timerCallback(const ros::TimerEvent&)
// {
// }


int main(int argc, char **argv)
{
    ros::init(argc, argv, "white_line_stopper_node");
    WhiteLineStopper white_line_stopper;

    ros::spin();
    return (0);
}
