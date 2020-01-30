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
			if ((x - x1)*(x - x1) + (y - y1)*(y - y1) <= y1*y1) cvSet2D(dst, y, x, f);
			else {
				CvScalar g = f;
				int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
				cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
			}
		}
	}

	cvShowImage("(8)", dst);
	cvSaveImage("(8).png", dst);
	cvWaitKey();

}