#!/bin/bash

today=`date +'%Y_%m_%d'`
time=`date +'%H_%M_%S'`
dir=/media/kuriatsu/Samsung_TC2019/TC2019_bag/$today

if [ -e $dir]; then
	echo "file found"
else
	mkdir -p $dir
fi

echo "record_start"
rosbag record \
/twist_raw \
/joy \
/ypspur_ros/cmd_vel \
/ypspur_ros/odom \
/scan \
/velodyne_packets \
/tf \
/d435_912/gyro/imu_info \
/d435_912/gyro/sample \
/d435_912/accel/imu_info \
/d435_912/depth/image_rect_raw \
/d435_912/accel/sample \
/d435_912/color/camera_info \
/d435_912/color/image_raw \
/d435_912/depth/color/points \
-O $dir/"$time.bag"
# /image_raw \
# /ndt_pose \
# /points_raw \
# /d435_902/gyro/imu_info \
# /d435_902/gyro/sample \
# /d435_902/color/camera_info \
# /d435_902/color/image_raw \
# /d435_902/depth/color/points \
# /d435_902/depth/image_rect_raw \
# /t265/fisheye1/camera_info \
# /t265/fisheye2/camera_info \
# /t265/fisheye1/image_raw \
# /t265/fisheye2/image_raw \
# /t265/odom/sample \
# /t265/gyro/imu_info \
# /t265/gyro/sample \
# /t265/accel/imu_info \
# /t265/accel/sample \
