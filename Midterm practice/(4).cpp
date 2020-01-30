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
			if (x < size.width / 2)
			{
				int x1 = x + (size.width/2)-1;
				cvSet2D(dst, y, x1, f);
			}
			else {
				int x1 = x - (size.width/2);
				cvSet2D(dst, y, x1, f);
			}
			
		}
	}

	cvShowImage("(4)", dst);
	cvSaveImage("(4).png", dst);
	cvWaitKey();
}