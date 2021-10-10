
#if 1
#define  _SCL_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <pcl/visualization/cloud_viewer.h>
#include <iostream>  
#include <pcl/io/io.h>  
#include <pcl/io/pcd_io.h>  
#include <opencv2/opencv.hpp>  


using namespace pcl;

using namespace std;
using namespace cv;

void viewerOneOff(visualization::PCLVisualizer& viewer)
{
	viewer.setBackgroundColor(0.0, 0.0, 0.0);
}
Mat color;
int user_data;


Mat rectifyImageL, rectifyImageR;


Rect validROIL;                                   //ͼ��У��֮�󣬻��ͼ����вü��������validROI����ָ�ü�֮�������  
Rect validROIR;
Mat Q;
Mat xyz;                                          //��ά����
Ptr<StereoBM> bm = StereoBM::create(16, 9);
int blockSize = 1, uniquenessRatio = 0, numDisparities = 5;


void stereo_match_sgbm(int, void*)                                         //SGBMƥ���㷨
{

	PointCloud<PointXYZRGB> cloud_a;
	PointCloud<PointXYZRGB>::Ptr cloud(new PointCloud<PointXYZRGB>);
	color = imread("rectified.png");  //У���õ������ͼ��
	imshow("color",color);
	
	int rowNumber = color.rows;
	int colNumber = color.cols;

	cloud_a.height = rowNumber;
	cloud_a.width = colNumber;
	cloud_a.points.resize(cloud_a.width * cloud_a.height);


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

	waitKey(1);
	for (unsigned int u = 0; u < rowNumber; ++u)
	{
		for (unsigned int v = 0; v < colNumber; ++v)
		{
			/*unsigned int num = rowNumber*colNumber-(u*colNumber + v)-1;*/
			unsigned int num = u*colNumber + v;


			cloud_a.points[num].b = color.at<Vec3b>(u, v)[0];
			cloud_a.points[num].g = color.at<Vec3b>(u, v)[1];
			cloud_a.points[num].r = color.at<Vec3b>(u, v)[2];

			cloud_a.points[num].x = u; //xyz.at<Vec3f>(u, v)[0];
			cloud_a.points[num].y = v;//xyz.at<Vec3f>(u, v)[1];
			cloud_a.points[num].z = xyz.at<Vec3f>(u, v)[2];
		}
	}
	*cloud = cloud_a;

	visualization::CloudViewer viewer("Cloud Viewer");

	viewer.showCloud(cloud);

	viewer.runOnVisualizationThreadOnce(viewerOneOff);

	while (!viewer.wasStopped())
	{
		user_data = 9;
	}

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
		cout << origin << "in world coordinate is: " << xyz.at<Vec3f>(origin)<< endl<< " ori"<<xyz.at<Vec3f>(origin)[0];
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
	Mat R, T, E, F, RFirst, RSec, PFirst, PSec;


	Rect validRoi[2];
	Mat viewLeft, viewRight;

	viewLeft = imread(".\\picture\\left_9.png", 1);//cameraIdLef  ����Ϊ����ɼ���ͼ��·��
	viewRight = imread(".\\picture\\right_9.png", 1); //cameraIRight


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
	imwrite("left_picture.png",rectifyImageL);
	cvtColor(rectifyImageL, rectifyImageL, CV_BGR2GRAY);
	cvtColor(rectifyImageR, rectifyImageR, CV_BGR2GRAY);

	imshow("remap_left", rectifyImageL);
	imshow("remap_right", rectifyImageR);

	//pcltest();
	
	//while (1);

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