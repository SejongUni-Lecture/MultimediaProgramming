//2번 문제 
왼쪽 마우스 버튼을 눌렀을 때 prev좌표가 설정되고, 버퍼에 이미지를 저장한다 cvCopy(img, buf)
드래깅 할 때 버퍼에 저장되었던 이미지를 불러온다 cvCopy(buf, img)
그 후에 직사각형 처리 

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