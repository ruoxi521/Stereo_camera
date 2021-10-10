
#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <math.h> 
 
using namespace std;
using namespace cv;
 
const int imageWidth = 320;                             //����ͷ�ķֱ���  
const int imageHeight = 240;
Vec3f  point3;   
float d;
Size imageSize = Size(imageWidth, imageHeight);   
 
Mat rgbImageL, grayImageL;
Mat rgbImageR, grayImageR;
Mat rectifyImageL, rectifyImageR;
 
Rect validROIL;//ͼ��У��֮�󣬻��ͼ����вü��������validROI����ָ�ü�֮�������  
Rect validROIR;

Mat cameraMatrixL,distCoeffL,cameraMatrixR,distCoeffR,R,T;
Mat mapLx, mapLy, mapRx, mapRy;     //ӳ���  
Mat Rl, Rr, Pl, Pr, Q;              //У����ת����R��ͶӰ����P ��ͶӰ����Q
Mat xyz;              //��ά����
 
Point origin;         //��갴�µ���ʼ��
Rect selection;      //�������ѡ��
bool selectObject = false;    //�Ƿ�ѡ�����

int blockSize = 0, uniquenessRatio = 0, numDisparities = 0;
Ptr<StereoBM> bm = StereoBM::create(16, 9);
 
	  /*****����ƥ��*****/
void stereo_match(int, void*)            //BM�㷨
{
	bm->setBlockSize(2 * blockSize + 5);     //SAD���ڴ�С��5~21֮��Ϊ��
	bm->setROI1(validROIL);
	bm->setROI2(validROIR);
	bm->setPreFilterCap(31);
	bm->setMinDisparity(0);  //��С�ӲĬ��ֵΪ0, �����Ǹ�ֵ��int��
	bm->setNumDisparities(numDisparities * 16 + 16);//�Ӳ�ڣ�������Ӳ�ֵ����С�Ӳ�ֵ֮��,���ڴ�С������16����������int��    nt blockSize = 0, uniquenessRatio = 0, numDisparities = 0;
	bm->setTextureThreshold(10);
	bm->setUniquenessRatio(uniquenessRatio);//uniquenessRatio��Ҫ���Է�ֹ��ƥ��
	bm->setSpeckleWindowSize(100);
	bm->setSpeckleRange(32);
	bm->setDisp12MaxDiff(-1);
	Mat disp, disp8;
	bm->compute(rectifyImageL, rectifyImageR, disp);//����ͼ�����Ϊ�Ҷ�ͼ
	disp.convertTo(disp8, CV_8U, 255 / ((numDisparities * 16 + 16)*16.));//��������Ӳ���CV_16S��ʽ
	reprojectImageTo3D(disp, xyz, Q, true); //��ʵ�������ʱ��ReprojectTo3D������X / W, Y / W, Z / W��Ҫ����16(Ҳ����W����16)�����ܵõ���ȷ����ά������Ϣ��
	xyz = xyz * 16;
	imshow("disparity", disp8);
}
 
/*****�������������ص�*****/
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
	case EVENT_LBUTTONDOWN:   //�����ť���µ��¼�
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		selectObject = true;
		//cout << origin << "in world coordinate is: " << xyz.at<Vec3f>(origin) << endl;
		  point3 = xyz.at<Vec3f>(origin);
		point3[0];
		//cout << "point3[0]:" << point3[0] << "point3[1]:" << point3[1] << "point3[2]:" << point3[2]<<endl;
		cout << "�������꣺" << endl;
		cout << "x: " << point3[0] << "  y: " << point3[1] << "  z: " << point3[2] << endl;
		 d = point3[0] * point3[0]+ point3[1] * point3[1]+ point3[2] * point3[2];
		 d = sqrt(d);   //mm
		// cout << "������:" << d << "mm" << endl;
		
		 d = d / 10.0;   //cm
         cout << "������:" << d << "cm" << endl;
 
		// d = d/1000.0;   //m
		// cout << "������:" << d << "m" << endl;
	
		break;
	case EVENT_LBUTTONUP:    //�����ť�ͷŵ��¼�
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
			break;
	}
}
 
 
/*****������*****/
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
	����У��
	*/
	// Rodrigues(rec, R); //Rodrigues�任
	stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q, CALIB_ZERO_DISPARITY,
		0, imageSize, &validROIL, &validROIR);
	initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pr, imageSize, CV_32FC1, mapLx, mapLy);
	initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_32FC1, mapRx, mapRy);
 
	/*
	��ȡͼƬ
	*/
	rgbImageL = imread("E:\\Visual Studio Code\\CodeSpace\\quadrotor\\Stereo\\Test\\picture\\left_9.png", CV_LOAD_IMAGE_COLOR);
	cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);
	rgbImageR = imread("E:\\Visual Studio Code\\CodeSpace\\quadrotor\\Stereo\\Test\\picture\\right_9.png", CV_LOAD_IMAGE_COLOR);
	cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);
 
	imshow("ImageL Before Rectify", grayImageL);
	imshow("ImageR Before Rectify", grayImageR);
 
	/*
	����remap֮�����������ͼ���Ѿ����沢���ж�׼��
	*/
	remap(grayImageL, rectifyImageL, mapLx, mapLy, INTER_LINEAR);
	remap(grayImageR, rectifyImageR, mapRx, mapRy, INTER_LINEAR);
 
	/*
	��У�������ʾ����
	*/
	Mat rgbRectifyImageL, rgbRectifyImageR;
	cvtColor(rectifyImageL, rgbRectifyImageL, CV_GRAY2BGR);  //α��ɫͼ
	cvtColor(rectifyImageR, rgbRectifyImageR, CV_GRAY2BGR);
 
	//������ʾ
	//rectangle(rgbRectifyImageL, validROIL, Scalar(0, 0, 255), 3, 8);
	//rectangle(rgbRectifyImageR, validROIR, Scalar(0, 0, 255), 3, 8);
	imshow("ImageL After Rectify", rgbRectifyImageL);
	imshow("ImageR After Rectify", rgbRectifyImageR);
 
	//��ʾ��ͬһ��ͼ��
	Mat canvas;
	double sf;
	int w, h;
	sf = 600. / MAX(imageSize.width, imageSize.height);
	w = cvRound(imageSize.width * sf);
	h = cvRound(imageSize.height * sf);
	canvas.create(h, w * 2, CV_8UC3);   //ע��ͨ��
 
										//��ͼ�񻭵�������
	Mat canvasPart = canvas(Rect(w * 0, 0, w, h));                                //�õ�������һ����  
	resize(rgbRectifyImageL, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);     //��ͼ�����ŵ���canvasPartһ����С  
	Rect vroiL(cvRound(validROIL.x*sf), cvRound(validROIL.y*sf),                //��ñ���ȡ������    
		cvRound(validROIL.width*sf), cvRound(validROIL.height*sf));
	//rectangle(canvasPart, vroiL, Scalar(0, 0, 255), 3, 8);                      //����һ������  
	cout << "Painted ImageL" << endl;
 
	//��ͼ�񻭵�������
	canvasPart = canvas(Rect(w, 0, w, h));                                      //��û�������һ����  
	resize(rgbRectifyImageR, canvasPart, canvasPart.size(), 0, 0, INTER_LINEAR);
	Rect vroiR(cvRound(validROIR.x * sf), cvRound(validROIR.y*sf),
		cvRound(validROIR.width * sf), cvRound(validROIR.height * sf));
	//rectangle(canvasPart, vroiR, Scalar(0, 0, 255), 3, 8);
	cout << "Painted ImageR" << endl;
 
	//���϶�Ӧ������
	for (int i = 0; i < canvas.rows; i += 16)
		line(canvas, Point(0, i), Point(canvas.cols, i), Scalar(0, 255, 0), 1, 8);
	imshow("rectified", canvas);
 
	/*
	����ƥ��
	*/
	namedWindow("disparity", CV_WINDOW_AUTOSIZE);
	// ����SAD���� Trackbar
	createTrackbar("BlockSize:\n", "disparity", &blockSize, 16, stereo_match);
	// �����Ӳ�Ψһ�԰ٷֱȴ��� Trackbar
	createTrackbar("UniquenessRatio:\n", "disparity", &uniquenessRatio, 50, stereo_match);
	// �����Ӳ�� Trackbar
	createTrackbar("NumDisparities:\n", "disparity", &numDisparities, 16, stereo_match);
	//�����Ӧ����setMouseCallback(��������, ���ص�����, �����ص������Ĳ�����һ��ȡ0)
	setMouseCallback("disparity", onMouse, 0);
	stereo_match(0, 0);
 
	waitKey(0);
	return 0;

}
