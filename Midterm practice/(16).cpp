#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	int count = 0;

	for (int y = 0; y < size.height; y++){
		for (int x = 0; x < size.width; x++){
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			int y1 = (size.height) / 8 * count;

			if (y <= y1){
				g.val[0] = f.val[0] + ((255.0f) / (size.height+100))*y1;
				g.val[1] = f.val[1] + ((255.0f) / (size.height+100))*y1;
				g.val[2] = f.val[2] + ((255.0f) / (size.height+100))*y1;

				if (y == y1){
					count++;
					y1 = y1*(count);
				}
			}

			cvSet2D(dst, y, x, g);
		}
	}
	
	cvShowImage("(16)", dst);
	cvSaveImage("(16).png", dst);
	cvWaitKey();
}