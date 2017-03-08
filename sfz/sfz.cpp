
//--------------------------------------������˵����------------------------------------------ -
//����˵������OpenCV3������š�OpenC3���鱾����ʾ������70
//�������������Ҳ����������ۺ�ʾ��
//�����������ò���ϵͳ�� Windows 7 64bit
//������������IDE�汾��Visual Studio 2010
//������������OpenCV�汾��	3.0 beta
//2014��11�� Created by @ǳī_ë����
//2014��12�� Revised by @ǳī_ë����
//------------------------------------------------------------------------------------------------

//
//-------------------------------- - ��ͷ�ļ��������ռ�������֡�----------------------------
//����������������ʹ�õ�ͷ�ļ��������ռ�
//------------------------------------------------------------------------------------------------
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void OstuBeresenThreshold(const Mat &in, Mat &out);
bool isEligible(const RotatedRect &candidate);
//---------------------------------- - ���궨�岿�֡�--------------------------------------------
//����������һЩ������
//------------------------------------------------------------------------------------------------
//#include<opencvcv.h>
#include<opencv/highgui.h>
#include<opencv/cxcore.h>
/*#include iostream */ 
#include <stdio.h > 
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int getColSum(Mat src, int col)
{
	int sum = 0;
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i< height; i++)
	{
		sum = sum + src.at <uchar>(i, col);
	}
	return sum;
}

int getRowSum(Mat src, int row)
{
	int sum = 0;
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i< width; i++)
	{
		sum += src.at <uchar>(row, i);
	}
	return sum;
}


void cutTop(Mat& src, Mat& dstImg)//�����и�
{
	int top, bottom;
	top = 0;
	bottom = src.rows;

	int i;
	for (i = 0; i< src.rows; i++)
	{
		int colValue = getRowSum(src, i);
		//cout i th  colValue  endl;
		if (colValue>0)
		{
			top = i;
			break;
		}
	}
	for (; i <src.rows; i++)
	{
		int colValue = getRowSum(src, i);
		//cout  i   th   colValue  endl;
		if (colValue == 0)
		{
			bottom = i;
			break;
		}
	}

	int height = bottom - top;
	Rect rect(0, top, src.cols, height);
	dstImg = src(rect).clone();
}

int cutLeft(Mat& src, Mat& leftImg, Mat& rightImg)//�����и�
{
	int left, right;
	left = 0;
	right = src.cols;
	int colValue = 0;
	int i;
	for (i = 0; i< src.cols; i++)
	{
		colValue = getColSum(src, i);
		//cout i th  colValue  endl;
		if (colValue>0)
		{
			left = i;
			break;
		}
	}
	if (left == 0 && colValue == 0)
	{
		return 1;
	}

	for (; i< src.cols; i++)
	{
		int colValue = getColSum(src, i);
		//cout  i   th   colValue  endl;
		if (colValue == 0)
		{
			right = i;
			break;
		}
	}
	int width = right - left;
	Rect rect(left, 0, width, src.rows);
	leftImg = src(rect).clone();
	Rect rectRight(right, 0, src.cols - right, src.rows);
	rightImg = src(rectRight).clone();
	cutTop(leftImg, leftImg);
	return 0;
}


void getPXSum(Mat &src, int &a)//��ȡ�������ص��
{
	threshold(src, src, 100, 255, CV_THRESH_BINARY_INV);
	a = 0;
	for (int i = 0; i< src.rows; i++)
	{
		for (int j = 0; j< src.cols; j++)
		{
			a += src.at <uchar>(i, j);
		}
	}
}

int  getSubtract(Mat &src, int TemplateNum) //����ͼƬ���
{
	Mat img_result(32, 48, CV_8UC3);
	int min = 1000000;
	int serieNum = 0;
	for (int i = 0; i< TemplateNum; i++)
	{
		char name[20];
		sprintf_s(name, "CUserssDesktop5%d.jpg", i);
		char name[100];
		char windowname[100];
		sprintf(name, "CUserssDesktop2%d.jpg", i);
		cout<<  name  <<endl;
		Mat Template = imread(name, CV_LOAD_IMAGE_GRAYSCALE);

		threshold(Template, Template, 100, 255, CV_THRESH_BINARY);
		threshold(src, src, 100, 255, CV_THRESH_BINARY);
		resize(src, src, Size(32, 48), 0, 0, CV_INTER_LINEAR);


		resize(Template, Template, Size(32, 48), 0, 0, CV_INTER_LINEAR);
		imshow(src, src);
		imshow(temp, Template);

		cout<<  src.size()  <<   src.type() << endl;
		cout << Template.size()  <<   Template.type() << endl;
		imshow(name, Template);
		absdiff(Template, src, img_result);
		imshow(diff, img_result);
		waitKey();
		int diff = 0;
		getPXSum(img_result, diff);
		if (diff<  min)
		{
			min = diff;
			serieNum = i;

		}
		int xxxx = 0;
	}
	printf("��С������%d", min);
	printf("ƥ�䵽��%d��ģ��ƥ���������%dn", serieNum, serieNum);
	return serieNum;
}
int main()
{

	Mat threshold_R = imread("D://���֤8.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	resize(threshold_R, threshold_R, Size(1200, 700), 0, 0, CV_INTER_LINEAR);
	int blockSize = 25;
	int constValue = 10;
	Mat img_gray;
	adaptiveThreshold(threshold_R, img_gray, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
	Mat threshold = Matzeros(threshold_R.rows, threshold_R.cols, CV_8UC3);
	OstuBeresenThreshold(threshold_R, threshold); //��ֵ��
		threshold_R = threshold_R  119;

	imshow("��ֵ��", img_gray);
	medianBlur(img_gray, img_gray, 15);

	namedWindow("��ֵ�˲�", WINDOW_NORMAL);
	imshow("��ֵ�˲�", img_gray);

	Mat imgInv(cvScalar(255));
	Mat threshold_Inv = imgInv - img_gray; //�ڰ�ɫ��ת��������Ϊ��ɫ
		namedWindow("������ת", WINDOW_NORMAL);
	imshow("������ת", threshold_Inv);
	Mat threshold_Inv;
	Mat element = getStructuringElement(MORPH_RECT, Size(70, 50));  //����̬ѧ�ĽṹԪ��
		morphologyEx(img_gray, threshold_Inv, CV_MOP_CLOSE, element);
	namedWindow("��", WINDOW_NORMAL);
	imshow("��", threshold_Inv);
	imwrite("E������Ӿ�Դ�� ���֤ʶ��Id_recognition", threshold_Inv);
	waitKey(0);

	vector vector Point  contours;
	cout  <<contours.size() << endl;
	vectorVec4i hierarchy;
	findContours(threshold_Inv, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //ֻ����������Ժ�ѡ���������н�һ��ɸѡ
		int index = 0;
	cout << contours.size() << endl;
	for (; index = 0; index = hierarchy[index][0])
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		//�˾�����OpenCV2��Ϊ��
			//drawContours(dstImage, contours, index, color, CV_FILLED, 8, hierarchy);
		//�˾�����OpenCV3��Ϊ��
			drawContours(threshold, contours, index, color, FILLED, 8, hierarchy);
	}



	//��7����ʾ��������ͼ
		imshow("����ͼ", threshold);

	waitKey(0);
	vector vector Point  iterator itc = contours.begin();
	RotatedRect mr = minAreaRect(Mat(itc));
	vector RotatedRect  rects;
	while (itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat(itc)); //����ÿ����������С�н��������
			if (!isEligible(mr))  //�жϾ��������Ƿ����Ҫ��
			{
				itc = contours.erase(itc);
			}
			else
			{
				rects.push_back(mr);
				++itc;
			}
	}

	//�����Ƿ��ҵ��˺�������
		cout << rects.size() << endl;
	Point2f vertices[4];
	rects[0].points(vertices);
	for (int i = 0; i 4; i++)
		line(threshold_R, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 0)); //����ɫ����
		namedWindow("������ת", WINDOW_NORMAL);
	imshow("������ת", threshold_R);
	waitKey(0);
	Rect brect = rects[0].boundingRect();
	Mat src = threshold_R(brect);
	imshow("Test_Rplane", src);
	imwrite("CUserssDesktop���֤��1.jpg", src);

	int blockSize1 = 25;
	int constValue1 = 10;
	Mat img_gray1;
	adaptiveThreshold(src, img_gray1, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize1, constValue1);
	imshow("��ֵ��", img_gray1);
	Mat out;
	medianBlur(img_gray, out, 15);
	medianBlur(img_gray1, out, 11);
	medianBlur(out, out, 5);

	imshow("��ֵ�˲�3", out);
	Mat leftImg, rightImg;
	int res = cutLeft(out, leftImg, rightImg);
	int i = 0;
	cout  res;
	while (res == 0)
	{
		char nameLeft[10];
		sprintf(nameLeft, %d, i);
		char nameRight[10];
		sprintf(nameRight, %dRight, i);
		i++;
		imshow(nameLeft, leftImg);
		stringstream ss;
		ss  nameLeft;
		imwrite("CUserssDesktop7 + ss.str() + .jpg", leftImg);
		ss  nameLeft;
		Mat srcTmp = rightImg;
		getSubtract(leftImg, 10);
		res = cutLeft(srcTmp, leftImg, rightImg);
	}

	waitKey(0);
	return 0;
}

bool isEligible(const RotatedRect &candidate)
{
	float error = 0.5;
	const float aspect = 4.5  0.3; //�����
		int min = 10  aspect  100; //��С����
		int max = 50  aspect  50;  //�������
		float rmin = aspect - aspecterror; //�����������С�����
		float rmax = aspect + aspecterror; //�����������󳤿��

		int area = candidate.size.height  candidate.size.width;
	float r = (float)candidate.size.width(float)candidate.size.height;
	if (r 1)
		r = 1  r;

	if ((area  min  area  max)  (r rmin  r  rmax)) //�������������Ϊ��candidateΪ��������
		return false;
	else
		return true;
}
void OstuBeresenThreshold(const Mat &in, Mat &out) //����Ϊ��ͨ��
{

	double ostu_T = threshold(in, out, 0, 255, CV_THRESH_OTSU); //otsu���ȫ����ֵ

		double min;
	double max;
	minMaxIdx(in, &min, &max);
	const double CI = 0.12;
	double beta = CI(max - min + 1)  128;
	double beta_lowT = (1 - beta)ostu_T;
	double beta_highT = (1 + beta)ostu_T;

	Mat doubleMatIn;
	in.copyTo(doubleMatIn);
	int rows = doubleMatIn.rows;
	int cols = doubleMatIn.cols;
	double Tbn;
	for (int i = 0; i <rows; ++i)
	{
		//��ȡ�� i��������ָ��
			uchar  p = doubleMatIn.ptruchar(i);
		uchar outPtr = out.ptruchar(i);

		//�Ե�i �е�ÿ������(byte)����
			for (int j = 0; j< cols; ++j)
			{

				if (i 2  irows - 3  j2  jrows - 3)
				{

					if (p[j] = beta_lowT)
						outPtr[j] = 0;
					else
						outPtr[j] = 255;
				}
				else
				{
					Tbn = sum(doubleMatIn(Rect(i - 2, j - 2, 5, 5)))[0]  25; // ���ڴ�С2525
						if (p[j]  beta_lowT(p[j]  Tbn && (beta_lowT = p[j] && p[j] = beta_highT)))
							outPtr[j] = 0;
					if (p[j]  beta_highT(p[j] = Tbn && (beta_lowT = p[j] && p[j] = beta_highT)))
						outPtr[j] = 255;
				}
			}
	}

}