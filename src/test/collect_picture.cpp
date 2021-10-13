#if 1//运行前改成1
#include<opencv2/opencv.hpp>
#include<stdlib.h>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap;
	cap.open(2); //打开摄像头
	//cap.open("http://192.168.1.100:8080/?action=stream?dummy=param.mjpg");
	if (!cap.isOpened())//如果视频不能正常打开则返回
		return -1;
	cvWaitKey(30);
	Mat frame;//用于保存每一帧图像
	cap >> frame;
	imshow("【双目原始视图】", frame);
	cvWaitKey(300);
	//system("C:\\\"Program Files (x86)\"\\\"CHUSEI 3D Webcam\"\\\"CHUSEI 3D Webcam.exe\"");    //Windows系统下，此处改成你的上位机安装绝对路径
    system("../../sh/camera_switch.sh");  //Ubuntu系统下，此处改成你的脚本存放相对路径

	char buf[100]; //保存路径变量

	while (1)
	{
		cap >> frame;          //等价于cap.read(frame);
		if (frame.empty())     //如果某帧为空则退出循环
			break;

		imshow("【双目原始视图】", frame);    //显示双目原始图像  原始分辨率为 640*480

		Mat DoubleImage;

		resize(frame, DoubleImage, Size(640, 240), (0, 0), (0, 0), INTER_AREA);   // 纵向分辨率缩小一半
		imshow("【双目缩小视图】", DoubleImage);  //显示图像

		Mat	LeftImage = DoubleImage(Rect(0, 0, 320, 240));      //分割得到左视图
		Mat RightImage = DoubleImage(Rect(320, 0, 320, 240));   //分割得到右视图

		imshow("【左视图】", LeftImage);         //显示左视图
		imshow("【右视图】", RightImage);        //显示右视图

		char c = cvWaitKey(30);
		if (c == 27)//Esc键退出
		{
			break;
		}

		static int i = 1;
		if (32 == char(c))  //keycode   32 = space 空格
		{
			sprintf(buf, "./picture/left_%d.png", i);  //保存左视图
			imwrite(buf, LeftImage);  //如果没有保存图片是由于相对路径设置不正确
			//imwrite("./picture/left.png",LeftImage);    //一定要注意Linux下的相对路径

			sprintf(buf, "./picture/right_%d.png", i);  //保存右视图
			imwrite(buf, RightImage);

			sprintf(buf, "./picture/total_%d.png", i);  //保存整体图像
			imwrite(buf, DoubleImage);
			
			printf("第%d次保存图片\n",i);
			i++;
		}
	}
	cap.release();//释放资源
}


#endif