#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "ros/ros.h"
#define READIMAGE_ONLY
#ifndef READIMAGE_ONLY
#include <geometry_msgs/Twist.h>
#endif

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	ROS_WARN("*****START*****");
	ros::init(argc, argv, "trafficLaneTrack"); //初始化ROS节点
	ros::NodeHandle n;

	//Before the use of camera, you can test ur program with images first: imread()
	VideoCapture capture;
	capture.open(0); //打开zed相机，如果要打开笔记本上的摄像头，需要改为0
	waitKey(100);
	if (!capture.isOpened())
	{
		printf("摄像头没有正常打开，重新插拔工控机上当摄像头\n");
		return 0;
	}

#ifndef READIMAGE_ONLY
	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/smoother_cmd_vel", 5); //定义dashgo机器人的速度发布器
#endif
	Mat src_frame;
	while (ros::ok())
	{
		capture.read(src_frame);
		if (src_frame.empty())
		{
			break;
		}
		imshow("src", src_frame);
		// 此处为实验部分，请自行增加直方图均衡化的代码

#ifndef READIMAGE_ONLY
		//以下代码可设置机器人的速度值，从而控制机器人运动
		geometry_msgs::Twist cmd_red;
		cmd_red.linear.x = 0;
		cmd_red.linear.y = 0;
		cmd_red.linear.z = 0;
		cmd_red.angular.x = 0;
		cmd_red.angular.y = 0;
		cmd_red.angular.z = 0.2;
		pub.publish(cmd_red);
#endif
		ros::spinOnce();
		waitKey(5);
	}
	return 0;
}
