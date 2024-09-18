#pragma once

#include"main.h"

#define PI 3.141592653

class PnPClass {
public:
	PnPClass() {};

	void PnP(vector<RotatedRect> armors);
public:
	//小装甲版四个角点的世界坐标
	std::vector<cv::Point3d> World_Coordinate =
	{
		cv::Point3f(-0.0675, 0.0625, 0),
		cv::Point3f(-0.0675, -0.0625, 0),
		cv::Point3f(0.0675, -0.0625, 0),
		cv::Point3f(0.0675, 0.0625, 0) };

	//����ڲξ���
	cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<
		901.5319174629633, 0, 382.9579217426170,
		0, 913.6835015051211, 260.0260075805787,
		0, 0, 1);

	//�������ϵ��
	cv::Mat dist_coeffs = (cv::Mat_<double>(5, 1) <<
	-0.110418682209790, 3.115239441613367, 0,
		0, -13.483354475192714);

	//��ת����
	cv::Mat rotation_vector = cv::Mat::zeros(3, 1, CV_64FC1);
	//ƽ������
	cv::Mat translation_vector = cv::Mat::zeros(3, 1, CV_64FC1);

public:
	float pitch=0.0,yaw=0.0,distance=0.0;//������,�ຽ��
	float x, y, z;
	float vx, vy;
};

