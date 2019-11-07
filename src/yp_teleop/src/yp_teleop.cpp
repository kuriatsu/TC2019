#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>
#include <sound_play/sound_play.h>
#include <dynamic_reconfigure/server.h>
#include "yp_teleop/yp_teleopConfig.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"

#include <cmath>


class YpTeleop
{
    private :
    ros::Subscriber sub_joy;
    ros::Subscriber sub_twist;
    ros::Subscriber sub_closest_waypoint;
    ros::Subscriber sub_whiteline_stopper;
    ros::Subscriber sub_safe_follow;
    ros::Publisher pub_yp_cmd;
    sound_play::SoundClient sc;

    float vel_rate_whiteline_stopper;
    float vel_rate_safe_follow;
    geometry_msgs::Twist in_twist;
    geometry_msgs::Twist out_twist;
    int triger_frag;

    float max_twist_speed;
    float pub_rate;
    float dash = 0;
    float base_speed = 0.5;

    ros::Timer timer;
    bool rosbag_flag;
    bool auto_mode;
    int closest_waypoint;

    dynamic_reconfigure::Server<yp_teleop::yp_teleopConfig> server;
    dynamic_reconfigure::Server<yp_teleop::yp_teleopConfig>::CallbackType server_callback;

    public :
    YpTeleop();

    private :
    void joyCallback(const sensor_msgs::Joy &in_msg);
    void twistCallback(const geometry_msgs::TwistStamped &in_msg);
    void timerCallback(const ros::TimerEvent&);
    void dynamicCfgCallback(yp_teleop::yp_teleopConfig &config, uint32_t level);
    void checkSystemError(const int system_return);
    void whiteLineStopperCallback(const std_msgs::Float32 &vel_rate);
    void safeFollowCallback(const std_msgs::Float32 &vel_rate);
};


YpTeleop::YpTeleop(): auto_mode(false), triger_frag(0), rosbag_flag(0), vel_rate_safe_follow(1.0), vel_rate_whiteline_stopper(1.0)
{
    // initialize
    ros::NodeHandle n;
    out_twist.linear.x = 0.0;
    out_twist.angular.z = 0.0;

    // ros communication
    sub_joy = n.subscribe("/joy", 1, &YpTeleop::joyCallback, this);
    sub_twist = n.subscribe("/twist_cmd", 1, &YpTeleop::twistCallback, this);
    sub_whiteline_stopper = n.subscribe("/vel_rate_whiteline_stopper", 1, &YpTeleop::whiteLineStopperCallback, this);
    sub_whiteline_stopper = n.subscribe("/vel_rate_safe_follow", 1, &YpTeleop::safeFollowCallback, this);

    pub_yp_cmd = n.advertise<geometry_msgs::Twist>("/ypspur_ros/cmd_vel", 1);

    // for dynamic reconfigure
    server_callback = boost::bind(&YpTeleop::dynamicCfgCallback, this, _1, _2);
    server.setCallback(server_callback);

    // parametor from ros launch
    n.getParam("autonomous_mode", auto_mode);

    ros::Duration(1).sleep();
    timer = n.createTimer(ros::Duration(pub_rate), &YpTeleop::timerCallback, this);
}


// get params from dynamic reconfigure
void YpTeleop::dynamicCfgCallback(yp_teleop::yp_teleopConfig &config, uint32_t level)
{
    base_speed = config.base_speed;
    dash = config.dash;
    max_twist_speed = config.max_twist_speed;
    pub_rate = config.pub_rate;
}


void YpTeleop::timerCallback(const ros::TimerEvent&)
{
    float speed_change;
    float current_twist_speed, aim_twist_speed;

    // limit speed with threshold set at the dynamic reconfigure
    aim_twist_speed = in_twist.linear.x * vel_rate_whiteline_stopper * vel_rate_safe_follow;
    aim_twist_speed = (aim_twist_speed < max_twist_speed) ? aim_twist_speed : max_twist_speed;
    aim_twist_speed = (aim_twist_speed > -max_twist_speed) ? aim_twist_speed : -max_twist_speed;

    out_twist.linear.x = aim_twist_speed;
    out_twist.angular.z = in_twist.angular.z;
    pub_yp_cmd.publish(out_twist);
}


// save twist value from safe_follow to buffer
void YpTeleop::twistCallback(const geometry_msgs::TwistStamped &in_msg)
{
    std::cout << auto_mode << std::endl;
    if(auto_mode)
    {
        in_twist.linear.x = in_msg.twist.linear.x;
        in_twist.angular.z = in_msg.twist.angular.z;
    }
}

void YpTeleop::safeFollowCallback(const std_msgs::Float32 &vel_rate)
{
    vel_rate_safe_follow = vel_rate.data;
}

void YpTeleop::whiteLineStopperCallback(const std_msgs::Float32 &vel_rate)
{
    vel_rate_whiteline_stopper = vel_rate.data;
}
// save twist value from joystick to buffer
// start rosbag or any othe application
void YpTeleop::joyCallback(const sensor_msgs::Joy &in_msg)
{
    double sec_interval;
    int system_return;
    // if(in_msg.axes[5] != -0.0) triger_frag = 1;
    // if(in_msg.axes[2] != -0.0) triger_frag = 2;

    // if(triger_frag < 1)
    // {
    //     accel = 0.0;
    // }else
    // {
    //     accel = (1.0 - in_msg.axes[5]) * 0.5;
    // }

    // if(triger_frag < 2)
    // {
    //     brake = 0.0;
    // }else
    // {
    //     brake = (1.0 + in_msg.axes[2]) * 0.5;
    // }

    // ROS_INFO("accel %f", accel);
    // ROS_INFO("brake %f", brake);

    //A button [0]
    if (in_msg.buttons[0])
    {
        sc.playWave("/usr/share/sounds/robot_sounds/jump.wav");
    }
    // B button [1]

    // X button [2]
    if (in_msg.buttons[2])
    {
        if(auto_mode)
        {
            auto_mode = false;
            sc.playWave("/usr/share/sounds/robot_sounds/pipe.wav");
            ROS_INFO("manual mode\n");
        }else if(!auto_mode)
        {
            auto_mode = true;
            sc.playWave("/usr/share/sounds/robot_sounds/powerup.wav");
            ROS_INFO("autonomous mode\n");
        }
    }

    // Y button [3]
    if (in_msg.buttons[3])
    {
        sc.playWave("/usr/share/sounds/robot_sounds/coin.wav");
    }

    if(!auto_mode)
    {
        in_twist.linear.x = base_speed * in_msg.axes[4];
        in_twist.angular.z = 0.5 * in_msg.axes[0] * in_msg.axes[4];
    }

    // LB [4]
    if (in_msg.buttons[4])
    {
        sc.playWave("/usr/share/sounds/robot_sounds/airship_moves.wav");
        in_twist.linear.x += (in_twist.linear.x > 0.0) ? dash : -dash;
    }
    // RB [5]
    if (in_msg.buttons[5])
    {
        sc.playWave("/usr/share/sounds/robot_sounds/airship_moves.wav");
        in_twist.linear.x += (in_twist.linear.x > 0.0) ? dash : -dash;
    }

    // START [7]
    if (in_msg.buttons[7] && in_msg.axes[2] == -1.0 && in_msg.axes[5] == -1)
    {
        if(!rosbag_flag)
        {
            ROS_INFO("bag_record_on");
            sc.playWave("/usr/share/sounds/robot_sounds/new_world.wav");
            system_return = system("bash ~/Program/Ros/mad_mobile_ws/src/yp_teleop/src/bag_recorder.sh &");
            ros::Duration(2).sleep();
            system_return = system("bash ~/Program/Ros/mad_mobile_ws/src/yp_teleop/src/bag_checker.sh &");
            checkSystemError(system_return);
            rosbag_flag = true;
        }
        else if(rosbag_flag)
        {
            ROS_INFO("bag_record_off");
            sc.playWave("/usr/share/sounds/robot_sounds/break_brick_block.wav");
            system_return = system("bash ~/Program/Ros/mad_mobile_ws/src/yp_teleop/src/bag_stopper.sh &");
            checkSystemError(system_return);
            rosbag_flag = false;
        }
    }
    // BACK [6]
    // Logicoool [8]
    // left joy click [9]
    // right joy click [10]
    // ROS_INFO("joy_in_twist %f", in_twist.linear.x);
}


void YpTeleop::checkSystemError(const int system_return)
{
    ROS_ERROR("shell command was failed");
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "yp_teleop");
    YpTeleop yp_teleop;

    ros::spin();
    return (0);
}
