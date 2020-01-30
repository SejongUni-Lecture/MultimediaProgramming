#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	int dx = size.width / 10;

	for (int x = 0; x < size.width; x++)
	{
		for (int y = 0; y < size.height; y++)
		{
			CvScalar f = cvGet2D(src, y, x);
			int x2 = x + (size.width / 4)*sin((2 * 3.14159* y)/size.height)% size.width;
			if (x2 < 0) x2 += size.width;
			if ((x / dx)%2)
			{
				f.val[0] = 0;
				f.val[1] = 0;
				f.val[2] = 0;
				cvSet2D(src, y, x2, f);
			}
		}
	}

	cvShowImage("(24)", src);
	cvSaveImage("(24).png", src);
	cvWaitKey();

	return 0;
}