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
			if (f.val[0] <= mid) g.val[0] = 255 - 2 * f.val[0];
			if (f.val[1] <= mid) g.val[1] = 255 - 2 * f.val[1];
			if (f.val[2] <= mid) g.val[2] = 255 - 2 * f.val[2];
			if (f.val[0] > mid) g.val[0] = 2 * f.val[0] - 255;
			if (f.val[1] > mid) g.val[1] = 2 * f.val[1] - 255;
			if (f.val[2] > mid) g.val[2] = 2 * f.val[2] - 255;
			cvSet2D(dst, y, x, g);
		}
	}
	
	cvShowImage("(14)", dst);
	cvSaveImage("(14).png", dst);
	cvWaitKey();
}