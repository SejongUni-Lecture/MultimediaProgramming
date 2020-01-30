#include<opencv2\opencv.hpp>

IplImage *img;
IplImage *buf;
int prevX;
int prevY;
int g_dsize = 1;
int g_size = 10;

void myMouse(int event, int x, int y, int Flags, void *)
{
	if (event == CV_EVENT_MOUSEMOVE)
	{
		prevX = x;
		prevY = y;
	}
}

int main()
{
	img = cvLoadImage("d:\\lena.png");
	CvSize size = cvGetSize(img);
	buf = cvCreateImage(size, 8, 3);

	cvCopy(img, buf);
	
	cvShowImage("image", img);
	cvSetMouseCallback("image", myMouse);

	while (true)
	{
		cvCopy(img, buf);
		cvCircle(buf, cvPoint(prevX, prevY), g_size, cvScalar(255, 255, 255), -1);
		cvShowImage("image", buf);

		if (cvWaitKey(30) != -1) break;
		g_size += g_dsize;
		if (g_size < 1 || g_size > 30) g_dsize = -g_dsize;
	}
	cvReleaseImage(&img);
	return 0;
}