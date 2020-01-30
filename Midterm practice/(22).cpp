#include<opencv2\opencv.hpp>
#include<math.h>


int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	int k = (size.height) / 10;
	int x1 = 0;

	for (int x = 0; x < size.width; x++)
	{
		for (int y = 0; y < size.height; y++)
		{
			CvScalar f = cvGet2D(src, y, x);
			int x2 = (size.width / 4)*sin(((2*3.14159)/size.height)*y)+x1;

			if (x2 >= size.width||x2<0) continue;
			else cvSet2D(dst, y, x2, f);
		}
		x1++;
	}
	cvShowImage("(22)", dst);
	cvSaveImage("(22).png", dst);
	cvWaitKey();
}