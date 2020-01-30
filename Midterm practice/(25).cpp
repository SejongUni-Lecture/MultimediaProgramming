#include<opencv2\opencv.hpp>

int main()
{
	IplImage *src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	int dx = size.width / 10;
	int dy = size.height / 10;
	int check, y2;
	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			y2 = y + (size.height / 4)*sin((x * 2 * 3.141592) / size.width);
			y2 %= size.height;//?:y2는 사인함수 그래프를 적용해서 나오는 결과인데
			//범위 외 값들은 안에 맞추려고 나머지 연산자 사용.
			if (y2 < 0) { y2 += size.height; }

			if ((y / dy) % 2) {
				f.val[0] = 0; f.val[1] = 0; f.val[2] = 0;
				cvSet2D(src, y2, x, f);
			}
		}
	}

	cvShowImage("(25)", src);
	cvSaveImage("(25).png", src);
	cvWaitKey();

	return 0;
}