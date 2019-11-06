#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PointStamped.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <fstream>
#include <tf/transform_listener.h>
#include <math.h>
#include <std_msgs/Float32.h>

struct MarkerData
{
    int id;
    geometry_msgs::Pose pose;
    std::vector<geometry_msgs::Point> corner_array;
};

class WhiteLineStopper
{
private:
    ros::Subscriber sub_joy;
    ros::Subscriber sub_ndt_pose;
    ros::Publisher pub_marker;
    ros::Publisher pub_corner;
    ros::Publisher pub_vel_rate;

    ros::Timer timer;
    int passed_whiteline_id;
    int intrusion_whiteline_id;
    int intrusion_decelerate_id;

    visualization_msgs::MarkerArray marker_array;

    std::vector<MarkerData> whiteline_data_array;
    geometry_msgs::Pose ego_vehicle_pose;

    float whiteline_length;
    float whiteline_width;
    float whiteline_deceleration;

    float vehicle_length;
    float vehicle_width;

public:
    WhiteLineStopper();

private:
    void joyCallback(const sensor_msgs::Joy &in_msg);
    void timerCallback(const ros::TimerEvent&);
    void poseCallback(const geometry_msgs::PoseStamped &ego_pose);
    void readFile(const std::string &file_name);
    void createWhiteLine();
    double quatToRpy(const geometry_msgs::Quaternion &quat);
    std::vector<geometry_msgs::Point> getBoxCorner(const geometry_msgs::Pose pose, const float width, const float length);
    void createEgoMarker(const geometry_msgs::Pose &ego_pose);
    void checkIntrusion();
    void pubStampedPoint(const std::vector<geometry_msgs::Point> &point_array);
};


WhiteLineStopper::WhiteLineStopper(): passed_whiteline_id(0), intrusion_whiteline_id(0), intrusion_decelerate_id(0)
{
    ros::NodeHandle n;

    sub_joy = n.subscribe("/joy", 1, &WhiteLineStopper::joyCallback, this);
    sub_ndt_pose = n.subscribe("/ndt_pose", 1, &WhiteLineStopper::poseCallback, this);
    pub_corner = n.advertise<geometry_msgs::PointStamped>("/corner_points", 10);
    pub_marker = n.advertise<visualization_msgs::MarkerArray>("/tc_helper_marker", 5);
    pub_vel_rate = n.advertise<std_msgs::Float32>("/vel_rate_whiteline_stopper", 1);

    whiteline_length = 2.0;
    whiteline_width = 1.5;
    vehicle_length = 0.8;
    vehicle_width = 0.6;
    whiteline_deceleration = 3.0;

    // ego_vehicle_data.corner_array.resize(1);

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

        whiteline_data_array.push_back({std::stoi(result.at(0)), whiteline_pose, getBoxCorner(whiteline_pose, whiteline_width, whiteline_length)});
        // ROS_INFO_STREAM(whiteline_data_array);
    }
}


std::vector<geometry_msgs::Point> WhiteLineStopper::getBoxCorner(const geometry_msgs::Pose pose, const float width, const float length)
{
    geometry_msgs::Point corner;
    std::vector<geometry_msgs::Point> corner_array;
    double yaw;

    yaw = quatToRpy(pose.orientation);

    corner.x = pose.position.x + (width * 0.5) * cos(yaw) - (length * 0.5) * sin(yaw);
    corner.y = pose.position.y + (width * 0.5) * sin(yaw) + (length * 0.5) * cos(yaw);
    corner.z = 0.0;
    corner_array.push_back(corner);

    corner.x = pose.position.x + (width * 0.5) * cos(yaw) - (-length * 0.5) * sin(yaw);
    corner.y = pose.position.y + (width * 0.5) * sin(yaw) + (-length * 0.5) * cos(yaw);
    corner_array.push_back(corner);

    corner.x = pose.position.x + (-width * 0.5) * cos(yaw) - (-length * 0.5) * sin(yaw);
    corner.y = pose.position.y + (-width * 0.5) * sin(yaw) + (-length * 0.5) * cos(yaw);
    corner_array.push_back(corner);

    corner.x = pose.position.x + (-width * 0.5) * cos(yaw) - (length * 0.5) * sin(yaw);
    corner.y = pose.position.y + (-width * 0.5) * sin(yaw) + (length * 0.5) * cos(yaw);
    corner_array.push_back(corner);

    return corner_array;
}


double WhiteLineStopper::quatToRpy(const geometry_msgs::Quaternion &quat)
{
    tf::Quaternion tf_quat;
    double roll, pitch, yaw;
    quaternionMsgToTF(quat, tf_quat);
    tf::Matrix3x3(tf_quat).getRPY(roll, pitch, yaw);
    return yaw;
}


void WhiteLineStopper::poseCallback(const geometry_msgs::PoseStamped &ego_pose)
{
    // std::vector<geometry_msgs::Point> vehicle_point

    createEgoMarker(ego_pose.pose);
    // ego_vehicle_data.id = 0;
    ego_vehicle_pose = ego_pose.pose;
    // ego_vehicle_data.corner_array = getBoxCorner(ego_pose.pose, vehicle_width, vehicle_length);
    // ego_vehicle_data.corner_array[0].x = {ego_pose.pose.position.x + vehicle_length/2, ego_pose.pose.position.y, ego_pose.pose.position.z};
    // ego_vehicle_data.corner_array[0].x = ego_pose.pose.position.x + vehicle_length/2;
    // ego_vehicle_data.corner_array[0].y = ego_pose.pose.position.y;
    // ego_vehicle_data.corner_array[0].z = ego_pose.pose.position.z;
}


void WhiteLineStopper::joyCallback(const sensor_msgs::Joy &in_msg)
{
    if (in_msg.buttons[3])
    {
        passed_whiteline_id = intrusion_whiteline_id;
    }
}


void WhiteLineStopper::timerCallback(const ros::TimerEvent&)
{
    float vel_rate = 1.0;
    checkIntrusion();

    pub_marker.publish(marker_array);
    std::cout << intrusion_decelerate_id << "," << intrusion_whiteline_id << "," << passed_whiteline_id << std::endl;
    if (intrusion_decelerate_id != passed_whiteline_id) vel_rate = 0.5;
    if (intrusion_whiteline_id != passed_whiteline_id) vel_rate = 0.0;
    pub_vel_rate.publish(vel_rate);

    for (auto whiteline_itr = whiteline_data_array.begin(); whiteline_itr != whiteline_data_array.end(); ++whiteline_itr)
    {
        pubStampedPoint(whiteline_itr->corner_array);
    }
    // pubStampedPoint(ego_vehicle_pose);


}


void WhiteLineStopper::pubStampedPoint(const std::vector<geometry_msgs::Point> &point_array)
{
    geometry_msgs::PointStamped point_stamped;

    for (auto itr = point_array.begin(); itr != point_array.end(); ++itr)
    {
        point_stamped.point = *itr;
        point_stamped.header.stamp = ros::Time(0);
        point_stamped.header.frame_id = "/map";
        pub_corner.publish(point_stamped);
    }

}


void WhiteLineStopper::checkIntrusion()
{
    bool frag = false;
    float outer_product[4], distance;
    int close_whiteline_id;

    for (auto itr=whiteline_data_array.begin(); itr!=whiteline_data_array.end(); ++itr)
    {
        distance = sqrt(pow(itr->pose.position.x - ego_vehicle_pose.position.x, 2) \
                        + pow(itr->pose.position.y - ego_vehicle_pose.position.y, 2));

        close_whiteline_id = itr->id;
        std::cout << "close_whiteline_id:" << close_whiteline_id << "distance:" << distance << std::endl;
        if (distance < whiteline_deceleration)
        // if (distance < whiteline_deceleration && close_whiteline_id != passed_whiteline_id)
        {
            intrusion_decelerate_id = close_whiteline_id;

            for (int i=0; i<4; i++)
            {
                outer_product[i] = (itr->corner_array[(i+1)%4].x - itr->corner_array[i].x) \
                                * (ego_vehicle_pose.position.y - itr->corner_array[i].y) \
                                - (ego_vehicle_pose.position.x - itr->corner_array[i].x) \
                                * (itr->corner_array[(i+1)%4].y - itr->corner_array[i].y);


            }
            std::cout << outer_product << '\n';
            if (outer_product[0] < 0 && outer_product[1] < 0 && outer_product[2] < 0 && outer_product[3] < 0)
            {
                intrusion_whiteline_id = close_whiteline_id;
                // return;
            }
        }
    }
}


void WhiteLineStopper::createWhiteLine()
{
    visualization_msgs::Marker marker;

    for (auto itr=whiteline_data_array.begin(); itr != whiteline_data_array.end(); ++itr)
    {
        marker.header.frame_id = "map";
        marker.header.stamp = ros::Time(0);
        marker.id = itr->id;
        marker.ns = "";
        marker.action = visualization_msgs::Marker::ADD;
        marker.type = visualization_msgs::Marker::CUBE;
        marker.pose = itr->pose;
        marker.scale.x = whiteline_width;
        marker.scale.y = whiteline_length;
        marker.scale.z = 0.1;
        marker.color.r = 1.0;
        marker.color.g = 1.0;
        marker.color.b = 1.0;
        marker.color.a = 0.5;
        marker.lifetime = ros::Duration();
        marker_array.markers.push_back(marker);
        ROS_INFO_STREAM(marker.pose);
        marker.type = visualization_msgs::Marker::CYLINDER;
        marker.id += std::distance(whiteline_data_array.begin(), whiteline_data_array.end());
        marker.scale.x = whiteline_deceleration*2;
        marker.scale.y = whiteline_deceleration*2;
        marker.scale.z = 0.1;
        marker.color.r = 0.5;
        marker.color.g = 0.5;
        marker.color.b = 0;
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
    // ego_vehicle_marker.pose.position.x += vehicle_length / 2;
    ego_vehicle_marker.scale.x = vehicle_length;
    ego_vehicle_marker.scale.y = vehicle_width;
    ego_vehicle_marker.scale.z = 1.2;
    ego_vehicle_marker.color.r = 0.0;
    ego_vehicle_marker.color.g = 0.5;
    ego_vehicle_marker.color.b = 0.5;
    ego_vehicle_marker.color.a = 0.5;
    ego_vehicle_marker.lifetime = ros::Duration();
    marker_array.markers.push_back(ego_vehicle_marker);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "whiteline_stopper_node");
    WhiteLineStopper whiteline_stopper;

    ros::spin();
    return 0;
}
