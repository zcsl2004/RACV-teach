#include "main.h"


Mat PreProce(Mat image,int color)
{
    vector<Mat> splited;//创建分离通道容器
    split(image,splited);//将彩色原图像的三通道分离
    Mat color_img;
    if(color==Blue){subtract(splited[0], splited[2], color_img);}//0-2为蓝色
    else if(color==Red){subtract(splited[2], splited[0], color_img);}//2-0为红色

    //二值化
    Mat thres_img;
    threshold(color_img, thres_img,70, 255, THRESH_BINARY);

    //闭运算
    Mat down_image;
    Mat element_down = getStructuringElement(MORPH_RECT,Size(7,7));
    dilate(thres_img, down_image, element_down);
    erode(down_image, down_image, element_down);

    //腐蚀+高斯滤波（噪点处理）
    Mat element_er = getStructuringElement(MORPH_RECT,Size(5,5));
    erode(down_image, down_image, element_er);
    Mat blur_img;
    GaussianBlur(down_image, blur_img, Size(5, 1), 0);

    //开运算
    erode(blur_img,blur_img,element_er);
    dilate(blur_img,blur_img,element_er);


    return blur_img;
}

//获取光条并筛选
 vector<RotatedRect> light(Mat image,Mat pre_img)
 {
    vector<RotatedRect> vc; //创建装甲板容器
    vector<vector<Point>> Light_Contour;// 发现的轮廓
    findContours(pre_img.clone(),Light_Contour, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    //筛选轮廓
    for(size_t i=0; i<Light_Contour.size(); i++)
    {
            drawContours(image,Light_Contour,i,Scalar(0,255,0),2);//绘制光条轮廓
            //求轮廓面积
            float Light_Contour_Area = contourArea(Light_Contour[i]);
            //去除较小轮廓
            if (Light_Contour_Area < 15 || Light_Contour[i].size() <= 10)
                continue;
            // 用椭圆拟合区域得到外接矩形
            RotatedRect Light_Rec = fitEllipse(Light_Contour[i]);

            if (Light_Rec.angle > 10 )
                continue;
            // 长宽比和轮廓面积比限制
            if (Light_Rec.size.width / Light_Rec.size.height > 2
                    || Light_Contour_Area / Light_Rec.size.area() < 0.68)
                continue;
            // 扩大灯柱的面积
            Light_Rec. size.height *= 1.1;
            Light_Rec.size.width *= 1.1;
            vc.push_back(Light_Rec);
    }

  
    return vc;
 }

void darw_light(vector<RotatedRect> vc,Mat image)
{
  //绘制筛选后的光条
    for (const RotatedRect& rect : vc) 
    {
        Point2f vertices[4];
        rect.points(vertices);

        for (int i = 0; i < 4; i++){
        line(image, vertices[i], vertices[(i + 1) % 4], Scalar(255, 0, 0), 2);}
    }
}