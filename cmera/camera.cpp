#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
using namespace cv;
 
int main(int argc, char **argv)
{
    // 定义棋盘格的尺寸，这里假设棋盘格是 8x6
    int boardWidth = 7;
    int boardHeight = 7;
    // 定义棋盘格每个方格的大小，单位是米
    float squareSize = 0.001f;
    // 创建棋盘格尺寸的 cv::Size 对象
    cv::Size boardSize(boardWidth, boardHeight);
 
    // 存储棋盘格角点对应的 3D 点
    vector<vector<cv::Point3f>> objectPoints;
    // 存储图像中检测到的棋盘格角点的 2D 点
    vector<vector<cv::Point2f>> imagePoints;
    // 临时存储当前图像中检测到的棋盘格角点
    vector<cv::Point2f> corners;
 
    // 声明图像的 cv::Mat 对象
    cv::Mat image, gray;
    // 创建一个窗口来显示图像
    cv::namedWindow("image", cv::WINDOW_NORMAL);
    // 存储图像文件名的 vector
    vector<cv::String> filNames;
    // 使用 glob 函数获取指定格式的图像文件名
    glob("/home/zcsl/cmera/chessboard_image_*.jpg", filNames);
 
    // 遍历所有图像文件
    for(size_t i = 0; i < filNames.size(); i++)
    {
        // 读取当前图像文件
        image = cv::imread(filNames[i], cv::IMREAD_COLOR);
        // 将图像从 BGR 颜色空间转换为灰度空间
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);
 
        // 使用 findChessboardCorners 函数检测棋盘格角点
        // 第二个参数是棋盘格的尺寸
        // 第三个参数是输出的角点集
        // 第四个参数是检测棋盘格时使用的参数
        bool found = cv::findChessboardCorners(image, boardSize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FAST_CHECK);
        
        // 如果角点被成功检测到
        if(found)
        {
            // 使用 cornerSubPix 对角点位置进行精确化处理
            cv::cornerSubPix(gray, corners, cv::Size(11,11), cv::Size(-1,-1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
            // 在图像上绘制棋盘格角点
            cv::drawChessboardCorners(image, boardSize, corners, found);
            // 显示处理后的图像
            cv::imshow("image",image);
            // 等待用户按键操作
            cv::waitKey(0);
 
            // 为每个 3D 点生成棋盘格角点的 vector
            vector<cv::Point3f> objectCorners;
            // 填充 objectCorners，每个棋盘格角点的 Z 坐标为 0.0f
            for(int j = 0; j < boardHeight; j++)
            {
                for(int k = 0; k < boardWidth; k++)
                {
                    objectCorners.push_back(cv::Point3f(k * squareSize, j * squareSize, 0.0f));
                }
            }
            // 将 objectCorners 添加到 objectPoints 中
            objectPoints.push_back(objectCorners);
            // 将检测到的 2D 角点 corners 添加到 imagePoints 中
            imagePoints.push_back(corners);
        }
    }
 
    // 声明相机内参矩阵和畸变系数向量
    cv::Mat cameraMatrix, distCoeffs;
    // 声明存储每个图像的旋转向量和平移向量的 vector
    vector<cv::Mat> rvecs, tvecs;
    // 使用 calibrateCamera 函数计算相机的内参和畸变系数
    // objectPoints 和 imagePoints 是之前存储的 3D 和 2D 角点
    // image.size() 是图像的尺寸
    // rvecs 和 tvecs 存储每个图像的旋转和平移向量
    cv::calibrateCamera(objectPoints, imagePoints, image.size(), cameraMatrix, distCoeffs, rvecs, tvecs);
    // 输出相机内参矩阵
    cout<<"camera matrix:"<<endl<<cameraMatrix<<endl;
    // 输出畸变系数
    cout<<"Distortion coefficients:"<<endl<<distCoeffs<<endl;
 
    //将参数写入parameters.txt文件中
 
    ofstream outfile("parameters.txt");  //打开一个文件流，准备写入
 
    if(!outfile.is_open())  //检查文件是否成功打开
    {
        cerr<<"Failed to open the output file!"<<endl;
        return -1;
    }
 
    try{
        //写入相机内参矩阵
        outfile<<"Camera Matrix:"<<endl<<cameraMatrix<<endl;
        //写入畸变系数
        outfile<<"Distortion Coefficients:"<<endl<<distCoeffs<<endl;
    }
 
    catch(exception& e)
    {
        cerr<<"An exception occurred!"<<endl;
        cerr<<e.what()<<endl;
        return -1;
    }
    
    outfile.close();  //关闭文件流
 
    return 0;
}