#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	int c1 = size.height / 2;
	int c2 = size.width / 2;
	int k1 = size.height / 20;
	int k2 = size.width / 20;
	int radius = size.height / 2 + 5*k1;
	int save = radius;

	for (int y = 0; y < size.height; y++){
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			int r = sqrt((y - c1)*(y - c1) + (x - c2)*(x - c2));

			if (r<radius){
				if ((radius / k2) % 2!=0)
				{
					cvSet2D(dst, y, x, f);
				}
			}
			if (x < size.width / 2) {
				if ((r / k1) == 0) continue;
				if (r == radius - k1) radius = radius - k1;	
			}
			else if (x >= size.width / 2){
				if (r == radius) radius = radius + k1;
			}
		}
		if (y < size.height / 2) {
			if (sqrt((y - c1)*(y - c1) + (k2-c2)*(k2-c2)) == save+k1)
			{	
				save = save + k1;
				radius = save;
			}
		}
		else if (y >= size.height / 2) {
			if (sqrt((y - c1)*(y - c1) + (k2-c2)*(k2-c2)) == save-k1)
			{
				save = save + k1;
				radius = save;
			}
		}
	}

	cvShowImage("(20)", dst);
	cvSaveImage("(20).png", dst);
	cvWaitKey();

	return 0;
}