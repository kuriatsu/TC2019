#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <fstream>
#include <tf/transform_listener.h>
class WhiteLineStopper
{
private:
    ros::Subscriber sub_joy;
    ros::Subscriber sub_ndt_pose;
    ros::Subscriber sub_twist;
    ros::Publisher pub_twist;
    ros::Publisher pub_marker;
    ros::Publisher pub_corner;

    bool go_frag;
    bool intrusion_frag;
    ros::Timer timer;

    visualization_msgs::MarkerArray marker_array;
    std::vector<geometry_msgs::Pose> whiteline_list;
    std::vector<std::vector<geometry_msgs::Point>> whiteline_corners_list;
    geometry_msgs::PointStamped whiteline_corner_point1;
    geometry_msgs::PointStamped whiteline_corner_point2;
    geometry_msgs::PointStamped whiteline_corner_point3;
    geometry_msgs::PointStamped whiteline_corner_point4;

    float whiteline_length;
    float whiteline_width;

    float vehicle_length;
    float vehicle_width;
    float decelerate_range;

public:
    WhiteLineStopper();

private:
    void joyCallback(const sensor_msgs::Joy &in_msg);
    void twistCallback(const geometry_msgs::TwistStamped &in_msg);
    void timerCallback(const ros::TimerEvent&);
    void poseCallback(const geometry_msgs::PoseStamped &ego_pose);
    void readFile(const std::string &file_name);
    void createWhiteLine();
    double quatToRpy(const geometry_msgs::Quaternion &quat);
    void createEgoMarker(const geometry_msgs::Pose &ego_pose);
};


WhiteLineStopper::WhiteLineStopper(): go_frag(true), intrusion_frag(false)
{
    ros::NodeHandle n;

    sub_joy = n.subscribe("/joy", 1, &WhiteLineStopper::joyCallback, this);
    sub_twist = n.subscribe("/twist_cmd_safe", 1, &WhiteLineStopper::twistCallback, this);
    sub_ndt_pose = n.subscribe("/ndt_pose", 1, &WhiteLineStopper::poseCallback, this);
    pub_corner = n.advertise<geometry_msgs::PointStamped>("/corner_check", 5);
    pub_twist = n.advertise<geometry_msgs::Twist>("/twist_cmd_helper", 1);
    pub_marker = n.advertise<visualization_msgs::MarkerArray>("/whiteline_marker", 1);

    whiteline_length = 2.0;
    whiteline_width = 0.5;
    vehicle_length = 0.8;
    vehicle_width = 0.6;
    decelerate_range = 3.0;

    readFile("/home/kuriatsu/Program/Ros/mad_mobile_ws/white_line_list.csv");
    createWhiteLine();

    ros::Duration(1).sleep();
    timer = n.createTimer(ros::Duration(0.1), &WhiteLineStopper::timerCallback, this);
}


void WhiteLineStopper::readFile(const std::string &file_name)
{
    std::string line;
    std::ifstream ifs(file_name);
    geometry_msgs::Pose whiteline_pose;
    std::vector<geometry_msgs::Point> whiteline_corners;

    double whiteline_yaw;

    if (!ifs == 2)
    {
        ROS_ERROR("Cannot Open File !");
        return;
    }

    // skip first line
    std::getline(ifs, line);
    //read csv file
    while(std::getline(ifs, line))
    {
        std::istringstream stream(line);
        std::string value;
        std::vector<std::string> result;

        while(std::getline(stream, value, ','))
        {
            result.push_back(value);
        }

        whiteline_pose.position.x = std::stof(result.at(1));
        whiteline_pose.position.y = std::stof(result.at(2));
        whiteline_pose.position.z = std::stof(result.at(3));
        whiteline_pose.orientation.x = std::stof(result.at(4));
        whiteline_pose.orientation.y = std::stof(result.at(5));
        whiteline_pose.orientation.z = std::stof(result.at(6));
        whiteline_pose.orientation.w = std::stof(result.at(7));

        whiteline_list.push_back(whiteline_pose);


        whiteline_yaw = quatToRpy(whiteline_pose.orientation);

        whiteline_corner_point1.point.x = whiteline_pose.position.x + (whiteline_width*0.5)*cos(whiteline_yaw)-(whiteline_length*0.5)*sin(whiteline_yaw);
        whiteline_corner_point1.point.y = whiteline_pose.position.y + (whiteline_width*0.5)*sin(whiteline_yaw)+(whiteline_length*0.5)*cos(whiteline_yaw);
        whiteline_corner_point1.point.z = 0.0;
        whiteline_corners.push_back(whiteline_corner_point1.point);

        whiteline_corner_point2.point.x = whiteline_pose.position.x + (-whiteline_width*0.5)*cos(whiteline_yaw)-(whiteline_length*0.5)*sin(whiteline_yaw);
        whiteline_corner_point2.point.y = whiteline_pose.position.y + (-whiteline_width*0.5)*sin(whiteline_yaw)+(whiteline_length*0.5)*cos(whiteline_yaw);
        whiteline_corner_point2.point.z = 0.0;
        whiteline_corners.push_back(whiteline_corner_point2.point);

        whiteline_corner_point3.point.x = whiteline_pose.position.x + (-whiteline_width*0.5)*cos(whiteline_yaw)-(-whiteline_length*0.5)*sin(whiteline_yaw);
        whiteline_corner_point3.point.y = whiteline_pose.position.y + (-whiteline_width*0.5)*sin(whiteline_yaw)+(-whiteline_length*0.5)*cos(whiteline_yaw);
        whiteline_corner_point3.point.z = 0.0;
        whiteline_corners.push_back(whiteline_corner_point3.point);

        whiteline_corner_point4.point.x = whiteline_pose.position.x + (whiteline_width*0.5)*cos(whiteline_yaw)-(-whiteline_length*0.5)*sin(whiteline_yaw);
        whiteline_corner_point4.point.y = whiteline_pose.position.y + (whiteline_width*0.5)*sin(whiteline_yaw)+(-whiteline_length*0.5)*cos(whiteline_yaw);
        whiteline_corner_point4.point.z = 0.0;
        whiteline_corners.push_back(whiteline_corner_point4.point);

        whiteline_corners_list.push_back(whiteline_corners);
        ROS_INFO_STREAM(whiteline_corner_point1);
    }
}


double WhiteLineStopper::quatToRpy(const geometry_msgs::Quaternion &quat)
{
    tf::Quaternion tf_quat;
    double roll, pitch, yaw;
    quaternionMsgToTF(quat, tf_quat);
    tf::Matrix3x3(tf_quat).getRPY(roll, pitch, yaw);
    return yaw;
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
    // for(auto itr=whiteline_list.begin(); itr != whiteline_list.end(); ++itr)
    // {
    //     if(abs(in_msg.pose.position.x - itr->position.x) < vehicle_decelerate_width && abs(in_msg.pose.position.y - itr->position.y) < vehicle_decelerate_width)
    //     {
    //
    //     }
    // }
}


void WhiteLineStopper::joyCallback(const sensor_msgs::Joy &in_msg)
{
    if(go_frag && in_msg.buttons[3])
    {
        go_frag == true;
    }
}


void WhiteLineStopper::createWhiteLine()
{
    visualization_msgs::Marker marker;

    for(auto itr=whiteline_list.begin(); itr != whiteline_list.end(); ++itr)
    {
        marker.header.frame_id = "map";
        marker.header.stamp = ros::Time(0);
        marker.id = std::distance(whiteline_list.begin(), itr);
        marker.ns = "";
        marker.action = visualization_msgs::Marker::ADD;
        marker.type = visualization_msgs::Marker::CUBE;
        marker.pose = *itr;
        marker.scale.x = whiteline_width;
        marker.scale.y = whiteline_length;
        marker.scale.z = 0.1;
        marker.color.r = 1.0;
        marker.color.g = 1.0;
        marker.color.b = 1.0;
        marker.color.a = 0.5;
        // marker.lifetime = ros::Duration();
        marker_array.markers.push_back(marker);

        marker.type = visualization_msgs::Marker::CYLINDER;
        marker.id += std::distance(whiteline_list.begin(), whiteline_list.end());
        marker.scale.x = decelerate_range;
        marker.scale.y = decelerate_range;
        marker.scale.z = 0.1;
        marker.color.r = 0.5;
        marker.color.g = 0.5;
        marker.color.b = 0;
        // decelerate_range_marker.lifetime = ros::Duration();
        marker_array.markers.push_back(marker);

    }
}


void WhiteLineStopper::createEgoMarker(const geometry_msgs::Pose &ego_pose)
{
    visualization_msgs::Marker ego_vehicle_marker;
    ego_vehicle_marker.header.frame_id = "map";
    ego_vehicle_marker.header.stamp = ros::Time(0);
    ego_vehicle_marker.ns = "";
    ego_vehicle_marker.action = visualization_msgs::Marker::ADD;
    ego_vehicle_marker.type = visualization_msgs::Marker::CUBE;
    ego_vehicle_marker.id = 0;
    ego_vehicle_marker.pose = ego_pose;
    ego_vehicle_marker.pose.position.x += vehicle_length / 2;
    ego_vehicle_marker.scale.x = vehicle_length;
    ego_vehicle_marker.scale.y = vehicle_width;
    ego_vehicle_marker.scale.z = 1.2;
    ego_vehicle_marker.color.r = ego_vehicle_marker.id * (1.0 / whiteline_list.size());
    ego_vehicle_marker.color.g = 1.0 - ego_vehicle_marker.color.r;
    ego_vehicle_marker.color.b = 1.0;
    ego_vehicle_marker.color.a = 1.0;
    ego_vehicle_marker.lifetime = ros::Duration();
    marker_array.markers.push_back(ego_vehicle_marker);


}


void WhiteLineStopper::timerCallback(const ros::TimerEvent&)
{
    whiteline_corner_point1.header.frame_id = "/map";
    whiteline_corner_point1.header.stamp = ros::Time(0);
    whiteline_corner_point2.header.frame_id = "/map";
    whiteline_corner_point2.header.stamp = ros::Time(0);
    whiteline_corner_point3.header.frame_id = "/map";
    whiteline_corner_point3.header.stamp = ros::Time(0);
    whiteline_corner_point4.header.frame_id = "/map";
    whiteline_corner_point4.header.stamp = ros::Time(0);
    pub_marker.publish(marker_array);
    pub_corner.publish(whiteline_corner_point1);
    pub_corner.publish(whiteline_corner_point3);
    pub_corner.publish(whiteline_corner_point2);
    pub_corner.publish(whiteline_corner_point4);


}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "whiteline_stopper_node");
    WhiteLineStopper whiteline_stopper;

    ros::spin();
    return (0);
}
