#include"main.h"
#include"preproce.h"
#include"pnp.h"

int main() {
    // 读取图像
    Mat image = imread("/home/zcsl/Pictures/test6.png");

    PnPClass PNP;

    //图像预处理
    Mat pre_img = PreProce(image,Blue);

    //提取光条
    vector<RotatedRect> vc = light(image,pre_img);
    //绘制光条
    darw_light(vc,image);
   
    //识别装甲版
    vector<RotatedRect> ARMORs;//装甲板容器
    for (size_t i = 0; i < vc.size(); i++) {
        for (size_t j = i + 1; (j < vc.size()); j++) {
            // 判断是否为相同灯条
            float Contour_angle = std::abs(vc[i].angle - vc[j].angle); // 角度差
            if (Contour_angle >= 30)
                continue;
            //长度差比率
            float Contour_Len1 = abs(vc[i].size.height - vc[j].size.height) / max(vc[i].size.height, vc[j].size.height);
            //宽度差比率
            float Contour_Len2 = abs(vc[i].size.width - vc[j].size.width) / max(vc[i].size.width, vc[j].size.width);
            if (Contour_Len1 > 0.65|| Contour_Len2 > 0.65)
                continue;
            //这里区分一下左右光条
            RotatedRect left_ligh,right_light;
            if(vc[i].center.x<vc[j].center.x)
            {
                left_ligh = vc[i];
                right_light = vc[j];
            }else{
                left_ligh = vc[j];
                right_light = vc[i];
            }
            //将光条信息存入装甲板
            RotatedRect ARMOR;
            ARMOR.center.x = (vc[i].center.x + vc[j].center.x) / 2.; //x坐标
            ARMOR.center.y = (vc[i].center.y + vc[j].center.y) / 2.; //y坐标
            ARMOR.angle = (vc[i].angle + vc[j].angle) / 2.; //角度
            float nh, nw;
            nh = (vc[i].size.height + vc[j].size.height) / 2; //高度
            // 宽度
            nw = sqrt((vc[i].center.x - vc[j].center.x) * (vc[i].center.x - vc[j].center.x) + (vc[i].center.y - vc[j].center.y) * (vc[i].center.y - vc[j].center.y));
            ARMOR.size.height = nh;
            ARMOR.size.width = nw;
            ARMORs.push_back(ARMOR);

            //printf("%f\n",nh);
        }
    }
    PNP.PnP(ARMORs);

    printf("pitch:%f,yaw:%f",PNP.pitch,PNP.yaw);
    
    //绘制装甲板
    darw_light(ARMORs,image);

    //显示结果图像
    imshow("原图", image);
    imshow("pre",pre_img);
    waitKey(0);

    return 0;
}