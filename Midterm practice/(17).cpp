#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	int count = 0;
	int y1 = (size.height) / 8;
	int x1 = (size.width) / 8;
	int y_index = y1;
	int x_index = x1;

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			if ((y1 / y_index) % 2 != 0)
			{
				if ((x1 / x_index) % 2 != 0)
				{
					if (x < x1&&y < y1) cvSet2D(dst, y, x, f); 
				}				
				if ((x1 / x_index) % 2 == 0)
				{
					if (x < x1&&y < y1) cvSet2D(dst, y, x, cvScalar(0,0,0));
				}
			}

			if ((y1 / y_index) % 2 == 0)
			{
				if ((x1 / x_index) % 2 == 0)
				{
					if (x < x1&&y < y1) cvSet2D(dst, y, x, f);
				}
				if ((x1 / x_index) % 2 != 0)
				{
					if (x < x1&&y < y1) cvSet2D(dst, y, x, cvScalar(0, 0, 0));
				}
			}
			if ((x + 1) % x_index == 0)
			{
				x1 += x_index;
			}
		}
		if ((y + 1) % y_index == 0)
		{
			x1 = x_index;
			y1 += y_index;
		}
	}
	cvShowImage("(17)", dst);
	cvSaveImage("(17).png", dst);
	cvWaitKey();
}