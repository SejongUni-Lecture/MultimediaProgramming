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
			int bri = (f.val[0] + f.val[1] + f.val[2]) / 3;
			CvScalar g = cvScalar(bri, bri, bri);

			f.val[0] = f.val[0] - x*((f.val[0] - bri) / size.width);
			f.val[1] = f.val[1] - x*((f.val[1] - bri) / size.width);
			f.val[2] = f.val[2] - x*((f.val[2] - bri) / size.width);

			cvSet2D(dst, y, x, f);
		}
	}

	cvShowImage("(9)", dst);
	cvSaveImage("(9).png", dst);
	cvWaitKey();

}