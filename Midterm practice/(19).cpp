#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	
	int k1 = (size.height) / 8;
	int k2 = (size.width) / 8;
	int fy = k1;
	int fx = k2;

	for (int y = k1; y < size.height-k1; y++){
		for (int x = k2; x < size.width-k2; x++)
		{
			for (int u = -k1; u <= k1; u++){
				for (int v = -k2; v <= k2; v++)
				{
					CvScalar f = cvGet2D(src, y + u, x + v);
					int x1 = x + v;
					int y1 = y + u;
					
					if ((x1 - fx)*(x1 - fx) + (y1 - fy)*(y1 - fy) <= k1*k2)
					{
						cvSet2D(dst, y1, x1, f);
					}
					else {
						CvScalar g;
						g.val[0] = f.val[0] * 0.5;
						g.val[1] = f.val[1] * 0.5;
						g.val[2] = f.val[2] * 0.5;
						cvSet2D(dst, y1, x1, g);
					}
				}
			}
			fx = fx + 2 * k2-1;
			x = fx - 1;
		}
		fy = fy + 2 * k1-1;
		y = fy - 1;
		fx = k1;
	}

	cvShowImage("(19)", dst);
	cvSaveImage("(19).png", dst);
	cvWaitKey();
	return 0;
}