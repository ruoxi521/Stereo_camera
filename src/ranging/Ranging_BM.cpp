
#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <math.h> 
 
using namespace std;
using namespace cv;
 
const int imageWidth = 320;                             //摄像头的分辨率  
const int imageHeight = 240;
Vec3f  point3;   
float d;
Size imageSize = Size(imageWidth, imageHeight);   
 
Mat rgbImageL, grayImageL;
Mat rgbImageR, grayImageR;
Mat rectifyImageL, rectifyImageR;
 
Rect validROIL;//图像校正之后，会对图像进行裁剪，这里的validROI就是指裁剪之后的区域  
Rect validROIR;

Mat cameraMatrixL,distCoeffL,cameraMatrixR,distCoeffR,R,T;
Mat mapLx, mapLy, mapRx, mapRy;     //映射表  
Mat Rl, Rr, Pl, Pr, Q;              //校正旋转矩阵R，投影矩阵P 重投影矩阵Q
Mat xyz;              //三维坐标
 
Point origin;         //鼠标按下的起始点
Rect selection;      //定义矩形选框
bool selectObject = false;    //是否选择对象

int blockSize = 0, uniquenessRatio = 0, numDisparities = 0;
Ptr<StereoBM> bm = StereoBM::create(16, 9);
 
	  /*****立体匹配*****/
void stereo_match(int, void*)            //BM算法
{
	bm->setBlockSize(2 * blockSize + 5);     //SAD窗口大小，5~21之间为宜
	bm->setROI1(validROIL);
	bm->setROI2(validROIR);
	bm->setPreFilterCap(31);
	bm->setMinDisparity(0);  //最小视差，默认值为0, 可以是负值，int型
	bm->setNumDisparities(numDisparities * 16 + 16);//视差窗口，即最大视差值与最小视差值之差,窗口大小必须是16的整数倍，int型    nt blockSize = 0, uniquenessRatio = 0, numDisparities = 0;
	bm->setTextureThreshold(10);
	bm->setUniquenessRatio(uniquenessRatio);//uniquenessRatio主要可以防止误匹配
	bm->setSpeckleWindowSize(100);
	bm->setSpeckleRange(32);
	bm->setDisp12MaxDiff(-1);
	Mat disp, disp8;
	bm->compute(rectifyImageL, rectifyImageR, disp);//输入图像必须为灰度图
	disp.convertTo(disp8, CV_8U, 255 / ((numDisparities * 16 + 16)*16.));//计算出的视差是CV_16S格式
	reprojectImageTo3D(disp, xyz, Q, true); //在实际求距离时，ReprojectTo3D出来的X / W, Y / W, Z / W都要乘以16(也就是W除以16)，才能得到正确的三维坐标信息。
	xyz = xyz * 16;
	imshow("disparity", disp8);
}
 
/*****描述：鼠标操作回调*****/
static void onMouse(int event, int x, int y, int, void*)
{
	if (selectObject)
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);
	}
 
	switch (event)
	{
	case EVENT_LBUTTONDOWN:   //鼠标左按钮按下的事件
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		selectObject = true;
		//cout << origin << "in world coordinate is: " << xyz.at<Vec3f>(origin) << endl;
		  point3 = xyz.at<Vec3f>(origin);
		point3[0];
		//cout << "point3[0]:" << point3[0] << "point3[1]:" << point3[1] << "point3[2]:" << point3[2]<<endl;
		cout << "世界坐标：" << endl;
		cout << "x: " << point3[0] << "  y: " << point3[1] << "  z: " << point3[2] << endl;
		 d = point3[0] * point3[0]+ point3[1] * point3[1]+ point3[2] * point3[2];
		 d = sqrt(d);   //mm
		// cout << "距离是:" << d << "mm" << endl;
		
		 d = d / 10.0;   //cm
         cout << "距离是:" << d << "cm" << endl;
 
		// d = d/1000.0;   //m
		// cout << "距离是:" << d << "m" << endl;
	
		break;
	case EVENT_LBUTTONUP:    //鼠标左按钮释放的事件
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
			break;
	}
}
 
 
/*****主函数*****/
int main()
{
	FileStorage fs("E:\\Visual Studio Code\\CodeSpace\\quadrotor\\Stereo\\Camera_calibration\\intrinsics.yml", FileStorage::READ);
	if (fs.isOpened())
	{
		cout << "read";
		fs["M1"] >> cameraMatrixL;fs["D1"] >> distCoeffL;
		fs["M2"] >> cameraMatrixR;fs["D2"] >> distCoeffR;

		fs["R"] >> R;
		fs["T"] >> T;
		fs["Q"] >> Q;
		cout << "M1-D1" << cameraMatrixL << endl << distCoeffL << endl;
		cout << "M2-D2" << cameraMatrixL << endl << distCoeffL << endl;
		cout << "R" << R <<endl;
		cout << "T" << T <<endl;
		cout << "Q" << Q <<endl;

		fs.release();
	}
	/*
	立体校正
	*/
	// Rodrigues(rec, R); //Rodrigues变换
	stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q, CALIB_ZERO_DISPARITY,
		0, imageSize, &validROIL, &validROIR);
	initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pr, imageSize, CV_32FC1, mapLx, mapLy);
	initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_32FC1, mapRx, mapRy);
 
	/*
	读取图片
	*/
	rgbImageL = imread("E:\\Visual Studio Code\\CodeSpace\\quadrotor\\Stereo\\Test\\picture\\left_9.png", CV_LOAD_IMAGE_COLOR);
	cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);
	rgbImageR = imread("E:\\Visual Studio Code\\CodeSpace\\quadrotor\\Stereo\\Test\\picture\\right_9.png", CV_LOAD_IMAGE_COLOR);
	cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);
 
	imshow("ImageL Before Rectify", grayImageL);
	imshow("ImageR Before Rectify", grayImageR);
 
	/*
	经过remap之后，左右相机的图像已经共面并且行对准了
	*/
	remap(grayImageL, rectifyImageL, mapLx, mapLy, INTER_LINEAR);
	remap(grayImageR, rectifyImageR, mapRx, mapRy, INTER_LINEAR);
 
	/*
	把校正结果显示出来
	*/
	Mat rgbRectifyImageL, rgbRectifyImageR;
	cvtColor(rectifyImageL, rgbRectifyImageL, CV_GRAY2BGR);  //伪彩色图
	cvtColor(rectifyImageR, rgbRectifyImageR, CV_GRAY2BGR);
 
	//单独显示
	//rectangle(rgbRectifyImageL, validROIL, Scalar(0, 0, 255), 3, 8);
	//rectangle(rgbRectifyImageR, validROIR, Scalar(0, 0, 255), 3, 8);
	imshow("ImageL After Rectify", rgbRectifyImageL);
	imshow("ImageR After Rectify", rgbRectifyImageR);
 
	//显示在同一张图上
	Mat canvas;
	double sf;
	int w, h;
	sf = 600. / MAX(imageSize.width, imageSize.height);
	w = cvRound(imageSize.width * sf);
	h = cvRound(imageSize.height * sf);
	canvas.create(h, w * 2, CV_8UC3);   //注意通道
 
										//左图像画到画布上
	Mat canvasPart = canvas(Rect(w * 0, 0, w, h));                                //得到画布的一部分  
	resize(rgbRectifyImageL, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);     //把图像缩放到跟canvasPart一样大小  
	Rect vroiL(cvRound(validROIL.x*sf), cvRound(validROIL.y*sf),                //获得被截取的区域    
		cvRound(validROIL.width*sf), cvRound(validROIL.height*sf));
	//rectangle(canvasPart, vroiL, Scalar(0, 0, 255), 3, 8);                      //画上一个矩形  
	cout << "Painted ImageL" << endl;
 
	//右图像画到画布上
	canvasPart = canvas(Rect(w, 0, w, h));                                      //获得画布的另一部分  
	resize(rgbRectifyImageR, canvasPart, canvasPart.size(), 0, 0, INTER_LINEAR);
	Rect vroiR(cvRound(validROIR.x * sf), cvRound(validROIR.y*sf),
		cvRound(validROIR.width * sf), cvRound(validROIR.height * sf));
	//rectangle(canvasPart, vroiR, Scalar(0, 0, 255), 3, 8);
	cout << "Painted ImageR" << endl;
 
	//画上对应的线条
	for (int i = 0; i < canvas.rows; i += 16)
		line(canvas, Point(0, i), Point(canvas.cols, i), Scalar(0, 255, 0), 1, 8);
	imshow("rectified", canvas);
 
	/*
	立体匹配
	*/
	namedWindow("disparity", CV_WINDOW_AUTOSIZE);
	// 创建SAD窗口 Trackbar
	createTrackbar("BlockSize:\n", "disparity", &blockSize, 16, stereo_match);
	// 创建视差唯一性百分比窗口 Trackbar
	createTrackbar("UniquenessRatio:\n", "disparity", &uniquenessRatio, 50, stereo_match);
	// 创建视差窗口 Trackbar
	createTrackbar("NumDisparities:\n", "disparity", &numDisparities, 16, stereo_match);
	//鼠标响应函数setMouseCallback(窗口名称, 鼠标回调函数, 传给回调函数的参数，一般取0)
	setMouseCallback("disparity", onMouse, 0);
	stereo_match(0, 0);
 
	waitKey(0);
	return 0;

}
