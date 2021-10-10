#if 1

#include <opencv2/opencv.hpp>  
#include <iostream>  

using namespace std;
using namespace cv;

Mat rectifyImageL, rectifyImageR;


Rect validROIL;                                   //ͼ��У��֮�󣬻��ͼ����вü��������validROI����ָ�ü�֮�������  
Rect validROIR;
Mat Q;
Mat xyz;                                          //��ά����
Ptr<StereoBM> bm = StereoBM::create(16, 9);
int blockSize = 0, uniquenessRatio = 0, numDisparities = 0;
void stereo_match_sgbm(int, void*)                                         //SGBMƥ���㷨
{
	bm->setBlockSize(2 * blockSize + 5);     //SAD���ڴ�С��5~21֮��Ϊ��
	bm->setROI1(validROIL);
	bm->setROI2(validROIR);
	bm->setPreFilterCap(31);
	bm->setMinDisparity(0);  //��С�ӲĬ��ֵΪ0, �����Ǹ�ֵ��int��
	bm->setNumDisparities(numDisparities * 16 + 16);//�Ӳ�ڣ�������Ӳ�ֵ����С�Ӳ�ֵ֮��,���ڴ�С������16����������int��
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


Point origin;                                     //��갴�µ���ʼ��
Rect selection;                                   //�������ѡ��
bool selectObject = false;                        //�Ƿ�ѡ�����

												  //--�������������ص�--------------------------------------------------
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
		cout << origin << "in world coordinate is: " << xyz.at<Vec3f>(origin) << endl;
		break;
	case EVENT_LBUTTONUP:    //�����ť�ͷŵ��¼�
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
			break;
	}

}



int main()
{
	Mat intrMatFirst, intrMatSec, distCoeffsFirst, distCoffesSec;
	Mat R, T, E, F, RFirst, RSec, PFirst, PSec ;


	Rect validRoi[2];
	Mat viewLeft, viewRight;

	viewLeft = imread("E:\\Visual Studio Code\\CodeSpace\\quadrotor\\Stereo\\Test\\picture\\left_9.png", 1);//cameraIdLef  ����Ϊ����ɼ���ͼ��·��
	viewRight = imread("E:\\Visual Studio Code\\CodeSpace\\quadrotor\\Stereo\\Test\\picture\\right_9.png", 1); //cameraIRight
																						 

	imshow("viewLeft", viewLeft);
	waitKey(40);
	cout << "done" << endl;

	Size imageSize = viewLeft.size();
	FileStorage fs("intrinsics.yml", FileStorage::READ);
	if (fs.isOpened())
	{
		cout << "read";
		fs["M1"] >> intrMatFirst;fs["D1"] >> distCoeffsFirst;
		fs["M2"] >> intrMatSec;fs["D2"] >> distCoffesSec;

		fs["R"] >> R;
		fs["T"] >> T;
		fs["Q"] >> Q;
		cout << "M1" << intrMatFirst << endl << distCoeffsFirst;

		fs.release();
	}

	cout << "stereo rectify..." << endl;
	stereoRectify(intrMatFirst, distCoeffsFirst, intrMatSec, distCoffesSec, imageSize, R, T, RFirst,
		RSec, PFirst, PSec, Q, CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);
		

	//stereoRectify
	Mat rmapFirst[2], rmapSec[2], rviewFirst, rviewSec;
	initUndistortRectifyMap(intrMatFirst, distCoeffsFirst, RFirst, PFirst,
		imageSize, CV_16SC2, rmapFirst[0], rmapFirst[1]);//CV_16SC2
	initUndistortRectifyMap(intrMatSec, distCoffesSec, RSec, PSec,//CV_16SC2
		imageSize, CV_16SC2, rmapSec[0], rmapSec[1]);

	remap(viewLeft, rectifyImageL, rmapFirst[0], rmapFirst[1], INTER_LINEAR);
	remap(viewRight, rectifyImageR, rmapSec[0], rmapSec[1], INTER_LINEAR);
	cvtColor(rectifyImageL, rectifyImageL, CV_BGR2GRAY);
	cvtColor(rectifyImageR, rectifyImageR, CV_BGR2GRAY);

	imshow("remap_left", rectifyImageL);
	imshow("remap_right", rectifyImageR);

	//--��ʾ���-------------------------------------------------------------------------------------
	namedWindow("disparity", WINDOW_NORMAL);

	//--����SAD���� Trackbar-------------------------------------------------------------------------
	createTrackbar("BlockSize:\n", "disparity", &blockSize, 8, stereo_match_sgbm);

	//--�����Ӳ�Ψһ�԰ٷֱȴ��� Trackbar------------------------------------------------------------
	createTrackbar("UniquenessRatio:\n", "disparity", &uniquenessRatio, 50, stereo_match_sgbm);

	//--�����Ӳ�� Trackbar------------------------------------------------------------------------
	createTrackbar("NumDisparities:\n", "disparity", &numDisparities, 16, stereo_match_sgbm);

	//--�����Ӧ����setMouseCallback(��������, ���ص�����, �����ص������Ĳ�����һ��ȡ0)------------
	setMouseCallback("disparity", onMouse, 0);

	stereo_match_sgbm(0, 0);   //--����Ҫ����������λ��5�������е���sgbm�㷨

	waitKey(400000);//����Ҫ��waitKey ��������ܴ����޷���ʾͼ������

	while (1);

}

#endif