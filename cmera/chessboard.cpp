// #include <iostream>
// #include <opencv2/opencv.hpp>

// using namespace std;
// using namespace cv;

// int main()
// {
//     VideoCapture cap(0);
//     if (!cap.isOpened())
//     {
//         cout << "无法打开摄像头" << endl;
//         return -1;
//     }

//     int imageCount = 0;

//     while (imageCount < 20)
//     {
//         Mat frame;
//         cap >> frame;

//         // 转为灰度图像
//         Mat gray;
//         cvtColor(frame, gray, COLOR_BGR2GRAY);

//         // 棋盘格尺寸
//         Size boardSize(7, 7);
//         vector<Point2f> corners;

//         // 寻找棋盘角点
//         bool found = findChessboardCorners(gray, boardSize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FAST_CHECK);

//         if (found)
//         {
//             // 细化角点位置
//             cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));

//             // 绘制角点
//             //drawChessboardCorners(frame, boardSize, corners, found);

//             // 保存图片
//             string imageName = "chessboard_image_" + to_string(imageCount) + ".jpg";
//             imwrite(imageName, frame);
//             imageCount++;
//         }

//         // 显示图像
//         imshow("Camera", frame);

//         if (waitKey(1) == 27)
//         {
//             break;
//         }
//     }

//     cap.release();
//     destroyAllWindows();

//     return 0;
// }
