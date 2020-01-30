#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	int x1 = (size.width) / 20;
	int y1 = (size.height) / 20;
	int k = (size.height) / 20;
	int save = k;
	int x_m = (size.width) / 2;
	int y_m = (size.height) / 2;
	int slope = y_m / x_m;
	int y2 = y_m;

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			if (x <= x_m&&y <= y_m)
			{
				int fy = -slope*x + y1;
				if (y < fy)
				{
					if ((y1 / k) % 2 == 0) cvSet2D(dst, y, x, f);
				}
				if (y == fy)
				{
					y1 += k;
				}
			}
			/*if (x >= x_m&&y <= y_m)
			{
				int fy = slope*x-3*y1 ;
				if (y < fy)
				{
					if ((y1 / k) % 2 != 0) cvSet2D(dst, y, x, f);
				}
				if (y == fy)
				{
					y1 += k;
				}
			}*/
			if (x < x_m&&y > y_m)
			{
				int fy = slope*x + y2;
				if (y > fy)
				{
					if ((y2 / k) % 2 == 0) cvSet2D(dst, y, x, f);
				}
				if (y == fy)
				{
					y2 -= k;
				}
			}
			/*if (x >= x_m&&y >= y_m)
			{
				int fy = -slope*x + y1;
				if (y < fy)
				{
					if ((y1 / k) % 2 != 0) cvSet2D(dst, y, x, f);
				}
				if (y == fy)
				{
					y1 += k;
				}
			}*/
		}
		y1 = save;
		if ((y + 1) % y1 == 0)
		{
			save += k;
			y1 = save;
			y2 += k;
		}

		
	}
	cvShowImage("(21)", dst);
	cvSaveImage("(21).png", dst);
	cvWaitKey();
}