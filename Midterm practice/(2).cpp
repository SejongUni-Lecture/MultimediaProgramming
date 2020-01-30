#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	for (int y = 0; y < size.height; y++){
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			int x2 = size.width - x - 1;
			cvSet2D(dst, y, x2, f);
		}
	}

	cvShowImage("(2)", dst);
	cvSaveImage("(2).png", dst);
	cvWaitKey();
}