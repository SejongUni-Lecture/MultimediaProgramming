#include<opencv2\opencv.hpp>
#include<math.h>

IplImage *src;
IplImage *dst;

float diff(CvScalar a, CvScalar b)
{
	float d = 0;
	for (int i = 0; i < 3; i++)
	{
		d += (a.val[i] - b.val[i])*(a.val[i] - b.val[i]);
	}
	return sqrt(d);
}

int main()
{
	src = cvLoadImage("d:\\lena.jpg");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	CvScalar palette[5];						//원색 팔레트 5개를 만든다
	palette[0] = cvScalar(0, 0, 0);
	palette[1] = cvScalar(255, 255, 255);
	palette[2] = cvScalar(255, 0, 0);
	palette[3] = cvScalar(0, 255, 0);
	palette[4] = cvScalar(0, 0, 255);
	int numColor = 5;


	for (int y = 0; y < size.height - 1; y++)				//커널 고려해서 처리
	{
		for (int x = 1; x < size.width - 1; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			float min_d = FLT_MAX;						//float min_d=FLT_MAX로 설정
			int min_color;								//min_color을 저장할 변수 선언
			for (int i = 0; i < numColor; i++)
			{	
				float d = diff(f, palette[i]);			//5개 팔레트 내에서 현재 컬러값과 팔레트의 거리
				if (d < min_d)							//만약 거리가 FLT_MAX보다 작으면 해당 팔레트 값 저장
				{
					min_color = i;
					min_d = d;
				}
			}
			cvSet2D(dst, y, x, palette[min_color]);


			CvScalar g = palette[min_color];			//팔레트 min color g에 저장
			CvScalar err;								//f(현재 컬러값) - g(가장 가까운 팔레트)
			err.val[0] = f.val[0] - g.val[0];
			err.val[1] = f.val[1] - g.val[1];
			err.val[2] = f.val[2] - g.val[2];

			CvScalar f1 = cvGet2D(src, y, x + 1);		//error 각 빈도수 값만큼 대입 src에 대입
			for (int k = 0; k < 3; k++)
			{
				f1.val[k] += 7.0f / 16 * err.val[k];
			}
			cvSet2D(src, y, x + 1, f1);

			CvScalar f2 = cvGet2D(src, y + 1, x);
			for (int k = 0; k < 3; k++)
			{
				f2.val[k] += 5.0f / 16 * err.val[k];
			}
			cvSet2D(src, y + 1, x, f2);

			CvScalar f3 = cvGet2D(src, y + 1, x - 1);
			for (int k = 0; k < 3; k++)
			{
				f3.val[k] += 3.0f / 16 * err.val[k];
			}
			cvSet2D(src, y + 1, x - 1, f3);

			CvScalar f4 = cvGet2D(src, y + 1, x + 1);
			for (int k = 0; k < 3; k++)
			{
				f4.val[k] += 1.0f / 16 * err.val[k];
			}
			cvSet2D(src, y + 1, x + 1, f4);
			
		}
	}

	cvShowImage("image", src);
	cvShowImage("dst", dst);
	cvWaitKey();
}