#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	IplImage *dst2 = cvCreateImage(size, 8, 3);

	for (int y = 0; y < size.height; y++){
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			int y1 = size.height - y - 1;
			int x1 = size.width - x - 1;
			cvSet2D(dst, y1, x1, f);
		}
	}

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(dst, y, x);
			if (x < size.height / 2 && y < size.width / 2){
				int x1 = x + size.width / 2;
				int y1 = y + size.height / 2;
				cvSet2D(dst2, y1, x1, f);
			}
			else if (x < size.height / 2 && y >= size.width / 2) {
				int x1 = x + size.width / 2;
				int y1 = y - size.height / 2;
				cvSet2D(dst2, y1, x1, f);
			}
			else if (x >= size.height / 2 && y < size.width / 2) {
				int x1 = x - size.width / 2;
				int y1 = y + size.height / 2;
				cvSet2D(dst2, y1, x1, f);
			}
			else {
				int x1 = x - size.width / 2;
				int y1 = y - size.height / 2;
				cvSet2D(dst2, y1, x1, f);
			}
		}
	}

	cvShowImage("(5)", dst2);
	cvSaveImage("(5).png", dst2);
	cvWaitKey();
}