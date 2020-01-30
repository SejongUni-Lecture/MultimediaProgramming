//unfinished

#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	double k1 = size.width / 64;
	double k2 = size.height / 64;
	int x_m = k1;
	int y_m = k2;

	CvScalar g = cvScalar(0, 0, 0);

	for (int y = k2; y < size.height - k2; y++) {
		for (int x = k1; x < size.width - k1; x++)
		{
			for (int u = -k2; u <= k2; u++) {
				for (int v = -k1; v <= k1; v++)
				{
					CvScalar f = cvGet2D(src, y, x);
					cvSet2D(dst, y + u, x + v, f);
				}
			}
			x_m += 2 * k1-1;
			x = x_m - 1;
		}
		if (y == y_m)
		{
			x_m = k1;
			y_m += 2 * k2 - 1;
			y = y_m - 1;
		}
	}

	cvShowImage("(15)", dst);
	cvSaveImage("(15).png", dst);
	cvWaitKey();
}