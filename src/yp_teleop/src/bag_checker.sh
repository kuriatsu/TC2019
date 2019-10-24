#!/bin/bash
today=`date +'%Y_%m_%d'`
time=`date +'%H_%M_%S'`
dir=/media/kuriatsu/Samsung_T5/TC2019_bag/$today

if [ ! -e "$dir/$time.bag.active" ]; then
    /home/kuriatsu/Program/Ros/mad_mobile_ws/src/yp_teleop/src/qt
fi
