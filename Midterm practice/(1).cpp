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
			int y1 = size.height - y - 1;
			cvSet2D(dst, y1, x, f);
		}
	}

	cvShowImage("(1)", dst);
	cvSaveImage("(1).png", dst);
	cvWaitKey();
}