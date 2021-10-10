#if 1 //����ǰ�ĳ�1
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cv.hpp>
#include <highgui\highgui.hpp>
#include <calib3d\calib3d.hpp>
#include <imgproc\imgproc.hpp>
#include <core\core.hpp>
#include <stdlib.h>
#include <Windows.h>

//�˴�������Ҫ�������̸�����޸�    
//���� �ڰ����̸� ��w��Ϊ10�����̸� ��ô w Ϊ 10 -1 = 9
#define  w  9      //���̸��ĺڰ׽�������    
#define  h  6      //���̸�ߵĺڰ׽�������    

const  float chessboardSquareSize = 27.5f;  //ÿ�����̸񷽿�ı߳� ��λ Ϊ mm


using namespace std;
using namespace cv;


//�� xml �ļ��ж�ȡͼƬ�洢·�� 

static bool readStringList(const string& filename, vector<string>& list)
{
	list.resize(0);
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	FileNode n = fs.getFirstTopLevelNode();
	if (n.type() != FileNode::SEQ)
		return false;
	FileNodeIterator it = n.begin(), it_end = n.end();
	for (; it != it_end; ++it)
		list.push_back((string)*it);
	return true;
}

//��¼���̸�ǵ����

static void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners)
{
	corners.resize(0);
	for (int i = 0; i < boardSize.height; i++)        //height��widthλ�ò��ܵߵ�
		for (int j = 0; j < boardSize.width; j++)
		{
			corners.push_back(Point3f(j*squareSize, i*squareSize, 0));
		}
}


bool calibrate(Mat& intrMat, Mat& distCoeffs, vector<vector<Point2f>>& imagePoints,
	vector<vector<Point3f>>& ObjectPoints, Size& imageSize, const int cameraId,
	vector<string> imageList)
{

	double rms = 0;  //��ͶӰ���

	Size boardSize;
	boardSize.width = w;
	boardSize.height = h;  

	vector<Point2f> pointBuf;
	float squareSize = chessboardSquareSize;

	vector<Mat> rvecs, tvecs; //������������ͷ����ת���� ��ƽ������

	bool ok = false;

	int nImages = (int)imageList.size() / 2;
	cout <<"ͼƬ����"<< nImages;
	namedWindow("View", 1);

	int nums = 0; //��Ч���̸�ͼƬ����

	for (int i = 0; i< nImages; i++)
	{
		Mat view, viewGray;
		view = imread(imageList[i * 2 + cameraId], 1); //��ȡͼƬ
		imageSize = view.size();
		cvtColor(view, viewGray, COLOR_BGR2GRAY); //ת���ɻҶ�ͼ

		bool found = findChessboardCorners(view, boardSize, pointBuf,
			CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);  //Ѱ�����̸�ǵ�

		if (found)
		{
			nums++;
			cornerSubPix(viewGray, pointBuf, Size(11, 11),
				Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(view, boardSize, Mat(pointBuf), found);
			bitwise_not(view, view);
			imagePoints.push_back(pointBuf);
			cout << '.';
		}
		imshow("View", view);
		waitKey(50);
	}

	cout << "��Ч���̸�����" << nums << endl;

	//calculate chessboardCorners
	calcChessboardCorners(boardSize, squareSize, ObjectPoints[0]);
	ObjectPoints.resize(imagePoints.size(), ObjectPoints[0]);

	rms = calibrateCamera(ObjectPoints, imagePoints, imageSize, intrMat, distCoeffs,
		rvecs, tvecs);
	ok = checkRange(intrMat) && checkRange(distCoeffs);

	if (ok)
	{
		cout << "done with RMS error=" << rms << endl;
		return true;
	}
	else
		return false;
}

int main()
{
	//initialize some parameters
	bool okcalib = false;
	Mat intrMatFirst, intrMatSec, distCoeffsFirst, distCoffesSec;
	Mat R, T, E, F, RFirst, RSec, PFirst, PSec, Q;
	vector<vector<Point2f>> imagePointsFirst, imagePointsSec;
	vector<vector<Point3f>> ObjectPoints(1);
	Rect validRoi[2];
	Size imageSize;
	int cameraIdFirst = 0, cameraIdSec = 1;
	double rms = 0;

	//get pictures and calibrate
	vector<string> imageList;
	string filename = "stereo_calibration.xml";
	bool okread = readStringList(filename, imageList);
	if (!okread || imageList.empty())
	{
		cout << "can not open " << filename << " or the string list is empty" << endl;
		return false;
	}
	if (imageList.size() % 2 != 0)
	{
		cout << "Error: the image list contains odd (non-even) number of elements\n";
		return false;
	}

	FileStorage fs("intrinsics.yml", FileStorage::WRITE);
	//calibrate

	cout << "calibrate left camera..." << endl;
	okcalib = calibrate(intrMatFirst, distCoeffsFirst, imagePointsFirst, ObjectPoints,
		imageSize, cameraIdFirst, imageList);

	if (!okcalib)
	{
		cout << "fail to calibrate left camera" << endl;
		return -1;
	}
	else
	{
		cout << "calibrate the right camera..." << endl;
	}


	okcalib = calibrate(intrMatSec, distCoffesSec, imagePointsSec, ObjectPoints,
		imageSize, cameraIdSec, imageList);

	fs << "M1" << intrMatFirst << "D1" << distCoeffsFirst <<
		"M2" << intrMatSec << "D2" << distCoffesSec;

	if (!okcalib)
	{
		cout << "fail to calibrate the right camera" << endl;
		return -1;
	}
	destroyAllWindows();

	//estimate position and orientation
	cout << "estimate position and orientation of the second camera" << endl
		<< "relative to the first camera..." << endl;
	cout << intrMatFirst;
	cout << distCoeffsFirst;
	cout << intrMatSec;
	cout << distCoffesSec;

	rms = stereoCalibrate(ObjectPoints, imagePointsFirst, imagePointsSec,
		intrMatFirst, distCoeffsFirst, intrMatSec, distCoffesSec,
		imageSize, R, T, E, F, CALIB_USE_INTRINSIC_GUESS,//CV_CALIB_FIX_INTRINSIC,
		TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 1e-6));          //������ͶӰ���
	cout << "done with RMS error=" << rms << endl;

	//stereo rectify
	cout << "stereo rectify..." << endl;
	stereoRectify(intrMatFirst, distCoeffsFirst, intrMatSec, distCoffesSec, imageSize, R, T, RFirst,
		RSec, PFirst, PSec, Q, CALIB_ZERO_DISPARITY, -1, imageSize, &validRoi[0], &validRoi[1]);
	cout << "Q" << Q << endl;
	cout << "P1" << PFirst << endl;
	cout << "P2" << PSec << endl;
	//read pictures for 3d-reconstruction

	if (fs.isOpened())
	{
		cout << "in";
		fs << "R" << R << "T" << T << "R1" << RFirst << "R2" << RSec << "P1" << PFirst << "P2" << PSec << "Q" << Q;
		fs.release();
	}

	namedWindow("canvas", 1);
	cout << "read the picture for 3d-reconstruction...";
	Mat canvas(imageSize.height, imageSize.width * 2, CV_8UC3), viewLeft, viewRight;
	Mat canLeft = canvas(Rect(0, 0, imageSize.width, imageSize.height));
	Mat canRight = canvas(Rect(imageSize.width, 0, imageSize.width, imageSize.height));

	viewLeft = imread(imageList[6], 1);//cameraIdFirst
	viewRight = imread(imageList[7], 1); //cameraIdSec
	viewLeft.copyTo(canLeft);
	viewRight.copyTo(canRight);
	cout << "done" << endl;
	imshow("canvas", canvas);
	waitKey(50); //����Ҫ��waitKey ��������ܴ����޷���ʾͼ������
	

	//stereoRectify
	Mat rmapFirst[2], rmapSec[2], rviewFirst, rviewSec;
	initUndistortRectifyMap(intrMatFirst, distCoeffsFirst, RFirst, PFirst,
		imageSize, CV_16SC2, rmapFirst[0], rmapFirst[1]);//CV_16SC2
	initUndistortRectifyMap(intrMatSec, distCoffesSec, RSec, PSec,//CV_16SC2
		imageSize, CV_16SC2, rmapSec[0], rmapSec[1]);
	remap(viewLeft, rviewFirst, rmapFirst[0], rmapFirst[1], INTER_LINEAR);
	imshow("remap", rviewFirst);
	waitKey(40);

	remap(viewRight, rviewSec, rmapSec[0], rmapSec[1], INTER_LINEAR);
	rviewFirst.copyTo(canLeft);
	rviewSec.copyTo(canRight);

	//rectangle(canLeft, validRoi[0], Scalar(255, 0, 0), 3, 8);
	//rectangle(canRight, validRoi[1], Scalar(255, 0, 0), 3, 8);

	Mat before_rectify = imread("./picture/total_0.png");

	for (int j = 0; j <= canvas.rows; j += 16)  //������
		line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);

	for (int j = 0; j <= canvas.rows; j += 16)  //������
		line(before_rectify, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
	cout << "stereo rectify done" << endl;

	imshow("У��ǰ", before_rectify); //��ʾ�����ߵ�У����ͼ��
	imshow("У����", canvas); //��ʾ�����ߵ�У��ǰͼ��

	waitKey(400000);//����Ҫ��waitKey ��������ܴ����޷���ʾͼ������

	//����Ҫ��waitKey ��������ܴ����޷���ʾͼ������
	/* �ٷ�����   http://masikkk.com/article/OpenCV-imshow-waitkey/ 
	A common mistake for OpenCV newcomers is to call cv::imshow() in a loop through video frames,
	without following up each draw with cv::waitKey(30).In this case, nothing appears on screen,
	because highgui is never given time to process the draw requests from cv::imshow().
	*/


	return 0;
}
#endif