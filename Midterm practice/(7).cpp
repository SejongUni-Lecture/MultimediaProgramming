#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	
	int x1 = size.width / 2;
	int y1 = size.height / 2;
	int w = size.width;
	int h = size.height;

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			if (y <= y1&&x <= x1)
			{
				if (x*x + y*y <= y1*y1) cvSet2D(dst, y, x, f);
				else {
					CvScalar g = f;
					int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
					cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
				}
		
			}
			else if (y <= y1&&x >= x1)
			{
				if ((x - w)*(x - w) + (y)* (y) <= y1*y1) cvSet2D(dst, y, x, f);
				else {
					CvScalar g = f;
					int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
					cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
				}
			}
			else if (y >= y1&&x <= x1)
			{
				if ((x)*(x) + (y - h)* (y - h) <= y1*y1) cvSet2D(dst, y, x, f);
				else {
					CvScalar g = f;
					int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
					cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
				}
			}
			else if (y >= y1&&x >= x1)
			{
				if ((x - w)*(x - w) + (y - h)* (y - h) <= y1*y1) cvSet2D(dst, y, x, f);
				else {
					CvScalar g = f;
					int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
					cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
				}
			}
		}
	}

	cvShowImage("(7)", dst);
	cvSaveImage("(7).png", dst);
	cvWaitKey();

}