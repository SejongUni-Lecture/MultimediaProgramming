#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	int x1 = size.width / 2;
	int y1 = size.height / 2;

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			f.val[0] += 255.0f / (size.width)*x;
			f.val[1] += 255.0f / (size.width)*x;
			f.val[2] += 255.0f / (size.width)*x;
			cvSet2D(dst, y, x, f);
		}
	}

	cvShowImage("(13)", dst);
	cvSaveImage("(13).png", dst);
	cvWaitKey();

}