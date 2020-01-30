#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	
	int x1 = size.width / 2;
	int y1 = size.height / 2;
	int slope = y1 / x1;

	for (int y = 0; y < size.height; y++){
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			if (y <= y1&&x <= x1){
				if (y <= -slope* (x - x1)) {
					CvScalar g = cvGet2D(src, y, x);
					int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
					cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
				}
				else cvSet2D(dst, y, x, f);
			}
			else if (y <= y1&&x >= x1){
				if (y <= slope*(x - x1))
				{
					CvScalar g = cvGet2D(src, y, x);
					int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
					cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
				}
				else cvSet2D(dst, y, x, f);
			}
			else if (y >= y1&&x <= x1) {
				if (y <= slope*x + y1) cvSet2D(dst, y, x, f);
				else {
					CvScalar g = cvGet2D(src, y, x);
					int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
					cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
				}
			}
			else if (y >= y1&&x >= x1) {
				if (y < -slope*x + 3 * y1) cvSet2D(dst, y, x, f);
				else {
					CvScalar g = cvGet2D(src, y, x);
					int bri = (g.val[0] + g.val[1] + g.val[2]) / 3;
					cvSet2D(dst, y, x, cvScalar(bri, bri, bri));
				}
			}
		}
	}
	
	cvShowImage("(6)", dst);
	cvSaveImage("(6).png", dst);
	cvWaitKey();
}