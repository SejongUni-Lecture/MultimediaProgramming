#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	int y1 = size.height / 3;
	int y2 = size.height / 3 * 2;

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			if (y <= y1){
				CvScalar g;
				g.val[0] = 0;
				g.val[0] = 0;
				g.val[2] = f.val[2];
				cvSet2D(dst, y, x, g);
			}
			else if (y1 <= y && y <= y2) {
				CvScalar g;
				g.val[0] = f.val[0];
				g.val[1] = 0;
				g.val[2] = 0;
				cvSet2D(dst, y, x, g);
			}
			else {
				CvScalar g;
				g.val[0] = 0;
				g.val[1] = f.val[1];
				g.val[2] = 0;
				cvSet2D(dst, y, x, g);
			}
		}
	}

	cvShowImage("(10)", dst);
	cvSaveImage("(10).png", dst);
	cvWaitKey();
}