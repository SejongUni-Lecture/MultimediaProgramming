1�� cvLine�� ����ϴ� ����
//prev ��ǥ�� ��� ���Ǵ��� ������ ��

#include<opencv2\opencv.hpp>

IplImage *src;
IplImage *dst;
int prevX;
int prevY;

void myMouse(int event, int x, int y, int flags, void *)
{
	if (event == CV_EVENT_MOUSEMOVE)
	{
		if (flags==CV_EVENT_FLAG_LBUTTON)
		{
			cvLine(src, cvPoint(prevX,prevY), cvPoint(x,y), cvScalar(255, 255, 255), 2);
			prevX = x;
			prevY = y;
			cvShowImage("image", src);
			cvWaitKey();
		}
		
	}
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		prevX = x;
		prevY = y;
	}
}

int main()
{
	src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);

	cvShowImage("image", src);
	cvSetMouseCallback("image", myMouse);
	cvWaitKey();
	
}