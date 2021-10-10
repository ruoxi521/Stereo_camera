#if 1//����ǰ�ĳ�1
#include<opencv2/opencv.hpp>
#include<stdlib.h>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap;
	cap.open(0); //������ͷ,�ʼǱ��Դ���һ������ͷ�����豸��Ϊ1������Ϊ0���ɴ�˫Ŀ����ͷ��Ĭ����������Դ�����ͷ����Ϊ0������Ϊ1.2.3�������е����
	//cap.open("http://192.168.1.100:8080/?action=stream?dummy=param.mjpg");
	if (!cap.isOpened())//�����Ƶ�����������򷵻�
		return -1;
	cvWaitKey(30);
	Mat frame;//���ڱ���ÿһ֡ͼ��
	cap >> frame;
	imshow("��˫Ŀԭʼ��ͼ��", frame);
	cvWaitKey(300);
	//system("C:\\\"Program Files (x86)\"\\\"CHUSEI 3D Webcam\"\\\"CHUSEI 3D Webcam.exe\"");

	char buf[30] = { 0 }; //����·������

	while (1)
	{
		cap >> frame;          //�ȼ���cap.read(frame);
		if (frame.empty())     //���ĳ֡Ϊ�����˳�ѭ��
			break;

		imshow("��˫Ŀԭʼ��ͼ��", frame);    //��ʾ˫Ŀԭʼͼ��  ԭʼ�ֱ���Ϊ 640*480

		Mat DoubleImage;

		resize(frame, DoubleImage, Size(640, 240), (0, 0), (0, 0), INTER_AREA);   // ����ֱ�����Сһ��
		imshow("��˫Ŀ��С��ͼ��", DoubleImage);  //��ʾͼ��


		Mat	LeftImage = DoubleImage(Rect(0, 0, 320, 240));      //�ָ�õ�����ͼ
		Mat RightImage = DoubleImage(Rect(320, 0, 320, 240));   //�ָ�õ�����ͼ

		imshow("������ͼ��", LeftImage);         //��ʾ����ͼ
		imshow("������ͼ��", RightImage);        //��ʾ����ͼ

		char c = cvWaitKey(30);
		if (c == 27)//Esc���˳�
		{
			break;
		}

		static int i = 9;
		if (13 == char(c))
		{
			sprintf(buf, ".\\picture\\left_%d.png", i);  //��������ͼ
			cout << buf;
			imwrite(buf, LeftImage);



			sprintf(buf, ".\\picture\\right_%d.png", i);  //��������ͼ
			imwrite(buf, RightImage);



			sprintf(buf, ".\\picture\\total_%d.png", i);  //��������ͼ��
			imwrite(buf, DoubleImage);
			i++;
		}

	}
	cap.release();//�ͷ���Դ
}


#endif