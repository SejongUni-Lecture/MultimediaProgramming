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
	CvScalar palette[5];						//���� �ȷ�Ʈ 5���� �����
	palette[0] = cvScalar(0, 0, 0);
	palette[1] = cvScalar(255, 255, 255);
	palette[2] = cvScalar(255, 0, 0);
	palette[3] = cvScalar(0, 255, 0);
	palette[4] = cvScalar(0, 0, 255);
	int numColor = 5;


	for (int y = 0; y < size.height - 1; y++)				//Ŀ�� ����ؼ� ó��
	{
		for (int x = 1; x < size.width - 1; x++)
		{
			CvScalar f = cvGet2D(src, y, x);
			float min_d = FLT_MAX;						//float min_d=FLT_MAX�� ����
			int min_color;								//min_color�� ������ ���� ����
			for (int i = 0; i < numColor; i++)
			{	
				float d = diff(f, palette[i]);			//5�� �ȷ�Ʈ ������ ���� �÷����� �ȷ�Ʈ�� �Ÿ�
				if (d < min_d)							//���� �Ÿ��� FLT_MAX���� ������ �ش� �ȷ�Ʈ �� ����
				{
					min_color = i;
					min_d = d;
				}
			}
			cvSet2D(dst, y, x, palette[min_color]);


			CvScalar g = palette[min_color];			//�ȷ�Ʈ min color g�� ����
			CvScalar err;								//f(���� �÷���) - g(���� ����� �ȷ�Ʈ)
			err.val[0] = f.val[0] - g.val[0];
			err.val[1] = f.val[1] - g.val[1];
			err.val[2] = f.val[2] - g.val[2];

			CvScalar f1 = cvGet2D(src, y, x + 1);		//error �� �󵵼� ����ŭ ���� src�� ����
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