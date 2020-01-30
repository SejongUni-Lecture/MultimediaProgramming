#include<opencv2\opencv.hpp>
#include<math.h>


int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	int y1 = 0;

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			int y2 = (size.height / 4)*sin(((2*3.14159)/size.width)*x)+y1;
			if (y2 >= size.height||y2<0) continue;
			else cvSet2D(dst, y2, x, f);
		}
		y1++;
	}


	cvShowImage("(23)", dst);
	cvSaveImage("(23).png", dst);
	cvWaitKey();
}