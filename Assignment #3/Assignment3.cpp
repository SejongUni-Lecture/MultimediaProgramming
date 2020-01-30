#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdlib.h>
using namespace std;

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

void UI();
void myFastestMeanFilter(IplImage *src, IplImage *dst, int k);

int main()
{
	char Imagepath[100];
	IplImage *src = NULL;

	UI();
	while (src == NULL)
	{
		cout << "Input File Path: ";
		cin >> Imagepath;
		src = cvLoadImage(Imagepath);
		if (src == NULL) cout << "Wrong File Path!" << endl;
	}

	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	int k = 1;

	while (k > 0)
	{
		cout << "Input Kernel Size K: ";
		cin >> k;
		if (k <= 0) {
			cout << "Quit the program" << endl;
			return 0;
		}
		else {
			myFastestMeanFilter(src, dst, k);
			cvShowImage("src", src);
			cvShowImage("dst", dst);
			cvWaitKey();//dst이미지에 아무 키보드를 입력하게 되면 Input Kernel Size 문구가 뜨게 된다.
			//원래는 실행이되면 바로 문구가 떠야하지만 dst의 응답없음 오류로 어쩔 수 없이 예시 프로그램과 맞지 않는 부분이 있음.
		}
	}
}

void UI() {
	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;

	cout << "Dept. of Digital Contents, Sejong University" << endl;
	cout << "Multimedia Programming Class" << endl;
	cout << "Homework #3: Fast Mean Filter" << endl;

	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;
}

void myFastestMeanFilter(IplImage *src, IplImage *dst, int k)
{
	CvSize size = cvGetSize(src);
	class RGB **tmp = new RGB*[size.height];
	for (int i = 0; i < size.height; i++) tmp[i] = new RGB[size.width];

	class RGB **tmp2 = new RGB*[size.height];
	for (int i = 0; i < size.height; i++) tmp2[i] = new RGB[size.width];

	int num = (2 * k + 1)*(2 * k + 1);

	for (int y = 0; y < size.height; y++)//가로 합 연산
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


	for (int x = 0; x < size.width; x++)//세로 합 연산
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

	for (int y = k; y < size.height - k; y++){
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

			result.val[0] = total.color_b / num;
			result.val[1] = total.color_g / num;
			result.val[2] = total.color_r / num;

			cvSet2D(dst, y, x, result);
		}
	}
}

