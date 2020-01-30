#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);


	int mid = 255 / 2;
	
	for (int y = 0; y < size.height; y++){
		for (int x = 0; x < size.width; x++){
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			if (f.val[0] <= mid) g.val[0] = 2 * f.val[0];
			if (f.val[1] <= mid) g.val[1] = 2 * f.val[1];
			if (f.val[2] <= mid) g.val[2] = 2 * f.val[2];
			if (f.val[0] > mid) g.val[0] = 510 - 2 * f.val[0];
			if (f.val[1] > mid) g.val[1] = 510 - 2 * f.val[1];
			if (f.val[2] > mid) g.val[2] = 510 - 2 * f.val[2];
			cvSet2D(dst, y, x, g);
		}
	}
	
	cvShowImage("(12)", dst);
	cvSaveImage("(12).png", dst);
	cvWaitKey();
}