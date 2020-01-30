#include<opencv2\opencv.hpp>
#include<iostream>
#include<stdlib.h>

//�������� ��� Thresholding�� �������� ���� ������� ���� �� �־�����
//s4�� ���� �׸��ڰ� £�� �� �������� ��� ���� �Ӱ谪�� ���� ���ϴ� ������� ������ ���� �� �ִ�
//���� ������ ���� �ƴ�, ���� �Ӱ谪�� �������־�� �Ѵ�.
//Adaptive thresholding ����� ä��,
//1.Ŀ�κ��� ���� ��հ��� ã�´�(ã�ƺ� �ڷῡ ���ϸ�, n=73, c=10�� �����غ�����)
//2.���� ��հ����� ���� ũ�� �Ͼ����, ������ ���������� ó���Ѵ�.
//���⼭ ���� ���α׷��� ������� �ٸ���.. �� �� £�� ó���ϴ� ����� ����Ѵ�.

class RGB {
public:
	int color_r, color_g, color_b;

	RGB() {				//default constructor
		color_r = 0;
		color_g = 0;
		color_b = 0;
	}
	RGB(IplImage *src, int y, int x)	//constructor
	{
		CvScalar p = cvGet2D(src, y, x);
		color_b = p.val[0];
		color_g = p.val[1];
		color_r = p.val[2];
	}

	RGB(RGB &pixel)		//copy constructor
	{
		int r, g, b;
		r = color_r;
		g = color_g;
		b = color_b;
		this->color_r = r;
		this->color_g = g;
		this->color_b = b;
	}
};

void myFastestMeanFilter(IplImage *src, IplImage *dst, int k);

int main()
{
	char file_input[100];
	int bri = 0;
	int bri2 = 0;
	IplImage * src = NULL;

	while (src == NULL)
	{
		printf("Input Filename:");
		scanf("%s", file_input);
		getchar();
		src = cvLoadImage(file_input);
		if (src != NULL) break;
		else printf("File not found!\n");
	}
	
	CvSize size = cvGetSize(src);
	IplImage *out = cvCreateImage(size, 8, 3);
	IplImage *dst = cvLoadImage(file_input);
	IplImage *in_img = cvCreateImage(size, 8, 3);

	cvSmooth(src, dst, CV_GAUSSIAN, 3, 3);
	
	myFastestMeanFilter(dst, in_img, 2);

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			float bri = 0;
			float bri2 = 0;
			CvScalar f = cvGet2D(dst, y, x);
			CvScalar g = cvGet2D(in_img, y, x);
			for (int i = 0; i < 3; i++)
			{
				bri += f.val[i];
				bri2 += g.val[i];
			}

			bri = bri / 3.0;
			bri2 = bri2 / 3.0;
			if (bri < bri2) cvSet2D(out, y, x, cvScalar(0, 0, 0));
			else cvSet2D(out, y, x, cvScalar(255, 255, 255));
		}
	}

	cvShowImage("dst", out);
	cvShowImage("src", src);
	cvWaitKey();
}

void myFastestMeanFilter(IplImage *src, IplImage *dst, int k)
{
	CvSize size = cvGetSize(src);
	class RGB **tmp = new RGB*[size.height];
	for (int i = 0; i < size.height; i++) tmp[i] = new RGB[size.width];

	class RGB **tmp2 = new RGB*[size.height];
	for (int i = 0; i < size.height; i++) tmp2[i] = new RGB[size.width];

	int num = (2 * k + 1)*(2 * k + 1);

	for (int y = 0; y < size.height; y++)//���� �� ����
	{
		class RGB *f = new RGB[size.width];
		class RGB *g = new RGB[size.width];
		for (int x = 0; x < size.width; x++)
		{
			class RGB input(src, y, x);
			f[x] = input;
		}

		for (int x = 0; x < size.width; x++)
		{
			for (int i = 0; i <= x; i++) {
				(g + x)->color_b += (f + i)->color_b;
				(g + x)->color_g += (f + i)->color_g;
				(g + x)->color_r += (f + i)->color_r;
			}
			tmp[y][x] = g[x];
		}
		delete[]f;
		delete[]g;
	}


	for (int x = 0; x < size.width; x++)//���� �� ����
	{
		class RGB *f = new RGB[size.height];
		class RGB *g = new RGB[size.height];

		for (int y = 0; y < size.height; y++)
		{
			f[y] = tmp[y][x];
			for (int i = 0; i <= y; i++) {
				(g + y)->color_b += (f + i)->color_b;
				(g + y)->color_g += (f + i)->color_g;
				(g + y)->color_r += (f + i)->color_r;
			}
			tmp2[y][x] = g[y];
		}
		delete[]f;
		delete[]g;
	}

	for (int y = k; y < size.height - k; y++) {
		for (int x = k; x < size.width - k; x++)
		{
			class RGB total, a, b, c, d;
			CvScalar result;

			if (y + k < size.height && x + k < size.width) a = tmp2[y + k][x + k];
			if (y - (k + 1) >= 0 && x + k <size.width) b = tmp2[y - (k + 1)][x + k];
			if (y - (k + 1) >= 0 && x - (k + 1) >= 0) c = tmp2[y - (k + 1)][x - (k + 1)];
			if (y + k < size.height &&  x - (k + 1) >= 0) d = tmp2[y + k][x - (k + 1)];

			total.color_b = a.color_b - b.color_b - d.color_b + c.color_b;
			total.color_g = a.color_g - b.color_g - d.color_g + c.color_g;
			total.color_r = a.color_r - b.color_r - d.color_r + c.color_r;

			result.val[0] = total.color_b / num - 4;
			result.val[1] = total.color_g / num - 4;
			result.val[2] = total.color_r / num - 4;

			cvSet2D(dst, y, x, result);
		}
	}
}
