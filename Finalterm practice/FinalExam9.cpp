#include<opencv2\opencv.hpp>

CvScalar myGet2D(IplImage *img, float y, float x)
{
	int i = (int)x;
	int j = (int)y;				//정수형으로 형변환
	float a = x - i;				//a를 만들기 위해 각 점에서 minus
	float b = y - j;

	int i2 = i + 1; if (i2 > img->width - 1) i2 = i;	//i2는 i+1이되, 이미지 범위에서 벗어나면 그대로 i
	int j2 = j + 1; if (j2 > img->height - 1) j2 = j;

	CvScalar f1 = cvGet2D(img, j, i);		//각 구간 스칼라 값
	CvScalar f2 = cvGet2D(img, j, i2);
	CvScalar f3 = cvGet2D(img, j2, i);
	CvScalar f4 = cvGet2D(img, j2, i2);
	CvScalar f;

	for (int k = 0; k < 3; k++)			//bilinear interpolation
	{
		f.val[k] = (1 - a)*(1 - b)*f1.val[k] +
			a * (1 - b)*f2.val[k] +
			(1 - a)*b*f3.val[k] +
			a * b*f4.val[k];

	}
	return f;
}


int main()
{
	IplImage *src = cvLoadImage("d:\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	float scale = 5.0f;			//scale 설정
	CvPoint pivot = cvPoint(size.width / 2, size.height / 2);
	for (float y2 = 0; y2 < dst->height; y2++)
	{
		for (float x2 = 0; x2 < dst->width; x2++)
		{
			float x1 = (x2 - pivot.x) / scale + pivot.x;
			float y1 = (y2 - pivot.y) / scale + pivot.y;
			

			//비례식 이용
			(x1-pivotX):(x2-pivotX)=1:scale
			
			if (x1<0 || x1>src->width - 1)continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = myGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}


	cvShowImage("iamge", dst);
	cvWaitKey();
	return 0;
}