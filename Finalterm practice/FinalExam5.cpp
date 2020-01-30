#include<opencv2\opencv.hpp>

IplImage *img;
IplImage *buf;
CvPoint g_pt;
int g_size = 50;

void drawMagnifier(IplImage *img, IplImage *buf, CvPoint pt)
{
	cvCopy(img, buf);
	
	for (int y2 = pt.y - g_size; y2 < pt.y + g_size; y2++)
	{
		for (int x2 = pt.x - g_size; x2 < pt.x + g_size; x2++)
		{
			if (x2 < 0 || x2>buf->width) continue;
			if (y2<0 || y2>buf->height) continue;

			float x1 = (x2 - pt.x) / 2.0f + pt.x;
			float y1 = (y2 - pt.y) / 2.0f + pt.y;

			if (x1<0 || x1>buf->width - 1) continue;
			if (y1<0 || y1>buf->height - 1) continue;
			
			cvSet2D(buf, y2, x2, cvGet2D(img, y1, x1));
		}
	}
}
void myMouse(int event, int x, int y, int flags, void *)
{
	if (event == CV_EVENT_MOUSEMOVE)
	{
		g_pt.x = x;
		g_pt.y = y;
		drawMagnifier(img,buf,g_pt);
		cvShowImage("image", buf);
	}
}
int main()
{
	img = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(img);
	buf = cvCreateImage(size, 8, 3);

	cvShowImage("image", img);
	cvSetMouseCallback("image", myMouse);
	cvWaitKey();
	return 0;
}