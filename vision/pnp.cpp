#include<pnp.h>

using namespace std;
using namespace cv;


void PnPClass::PnP(vector<RotatedRect> armors) {
	

	for (size_t i = 0; i < armors.size(); i++) {
		
		vector<Point2d> points = {
			Point2f(armors[i].center.x-armors[i].size.width/2,
					armors[i].center.y- armors[i].size.height/2),//左上角
			Point2f(armors[i].center.x+armors[i].size.width/2,
					armors[i].center.y- armors[i].size.height/2),//右上角
			Point2f(armors[i].center.x+armors[i].size.width/2,
					armors[i].center.y+armors[i].size.height/2),//右下角
			Point2f(armors[i].center.x-armors[i].size.width/2,
					armors[i].center.y+armors[i].size.height/2),//左下角
		};

		//pnp解算
		solvePnP(World_Coordinate, points, camera_matrix, dist_coeffs, \
			rotation_vector, translation_vector, 0, SOLVEPNP_ITERATIVE);
				
		//获取世界坐标
		x = (float)translation_vector.at<double>(0, 0);
		y = (float)translation_vector.at<double>(1, 0);
		z = (float)translation_vector.at<double>(2, 0);

		//获取pitch和yaw角度
		pitch = -asin((float)translation_vector.at<double>(0, 0)/
			(float)translation_vector.at<double>(2, 0)) / PI;

		yaw = asin((float)translation_vector.at<double>(1, 0)/
			(float)translation_vector.at<double>(2, 0)) / PI;

		//z轴即为目标距离
		distance = z;

	}
}

