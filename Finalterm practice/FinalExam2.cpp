//2�� ���� 
���� ���콺 ��ư�� ������ �� prev��ǥ�� �����ǰ�, ���ۿ� �̹����� �����Ѵ� cvCopy(img, buf)
�巡�� �� �� ���ۿ� ����Ǿ��� �̹����� �ҷ��´� cvCopy(buf, img)
�� �Ŀ� ���簢�� ó�� 

#include <opencv2\opencv.hpp>

IplImage *img;
IplImage *buf;
int prevX;
int prevY;
CvScalar color = cvScalar(0, 0, 0);

void myMouse(int event, int x, int y, int flags, void *)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cvCopy(img, buf);
		prevX = x;
		prevY = y;
	}

	if (event == CV_EVENT_MOUSEMOVE)
	{
		if (flags == CV_EVENT_FLAG_LBUTTON)
		{
			cvCopy(buf,img);
			cvRectangle(img, cvPoint(prevX, prevY), cvPoint(x, y), color, 2);
			cvShowImage("image", img);
		}
	}

	if (event == CV_EVENT_RBUTTONDOWN)
	{
		if (color.val[0] == 0) {
			color = cvScalar(255, 255, 255);
		}
		else {
			color = cvScalar(0, 0, 0);
		}
	}
}

int main()
{
	img = cvLoadImage("d:\\lena.png");
	CvSize size = cvGetSize(img);
	buf = cvCreateImage(size, 8, 3);

	printf("Example for the final exam:\nDrawing rectangles(LBUTTON) and changing colors(RBUTTON) :");

	cvShowImage("image", img);
	cvSetMouseCallback("image", myMouse);
	cvWaitKey();
}