#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<math.h>
using namespace std;

void UI();
void GammaUI();
void SinUI();
void setGamma(IplImage *img, IplImage *result, double gamma);
void GammaGraph(IplImage *graph, double gamma);
void setSin(IplImage* img, IplImage *result, double gamma);
void SinGraph(IplImage *graph2, double freq);
double Rounding(double x, int digit)
{
	return (floor((x)* pow(double(10), digit) + 0.5 + 0.0000000001) / pow(double(10), digit));
}


int main()
{
	IplImage *img = NULL, *result, *graph, *graph2;
	char ImagePath[100];
	int key, key2;
	double gamma = 1.000000, slope = 1.0, freq = 0.50000;

	graph = cvCreateImage(cvSize(250, 250), 8, 3);
	graph2 = cvCreateImage(cvSize(250, 250), 8, 3);
	cvSet(graph, cvScalar(255, 255, 255));
	cvSet(graph2, cvScalar(255, 255, 255));

	UI();
	cout << endl << "Input File Path: ";
	cin >> ImagePath;
	img = cvLoadImage(ImagePath);
	if (img == NULL) cout << "File not Found!" << endl;

	while (img == NULL)
	{
		cout << "Input File Path: ";
		cin >> ImagePath;
		img = cvLoadImage(ImagePath);
		if (img == NULL) cout << "File not Found!" << endl;
	}

	CvSize size = cvGetSize(img);
	result = cvCreateImage(size, 8, 3);

	for (int y = 0; y < size.height; y++)
		for (int x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);
			cvSet2D(result, y, x, color);
		}

	for (int y = 0; y < 250; y++)
		for (int x = 0; x < 250; x++)
		{
			int y2 = 249 - y;
			if (y == slope*x) cvSet2D(graph, y2, x, cvScalar(0, 0, 0));
		}

	for (int x = 0; x < 250; x++)
		for (int y = 0; y < 250; y++)
		{
			int y3 = y;
			int y2 = 249 - y3;
			int result = (125 * sin((double)((3.14159 / 125))*x) + 125);
			if (y3==result)
			{
				cvSet2D(graph2, y2, x, cvScalar(0, 0, 0));
			}
		}
	
	while (true) {
		cout << "Select function (1=gamma, 2=sinusoidal): ";
		cin >> key;
		if (key == 1)
		{
			cout << endl;
			GammaUI();
			while (true)
			{
				cvShowImage("Image", result);
				cvShowImage("Graph", graph);
				key2 = cvWaitKey();
				if (key2 == '1')
				{
					(double)gamma += 0.1;
					if (gamma < 0) gamma = 0.0;
					cout << "Current Gamma = ";
					printf("%f\n", gamma);

					setGamma(img, result, gamma);
					GammaGraph(graph, gamma);
				}
				if (key2 == '2')
				{
					(double)gamma -= 0.1;
					if (gamma < 0) gamma = 0.0;
					cout << "Current Gamma = ";
					printf("%f\n", gamma);
					setGamma(img, result, gamma);
					GammaGraph(graph, gamma);
				}
				if (key2 == 'Q' || key2 == 'q') break;
			}
		}

		else if (key == 2)
		{
			cout << endl;
			GammaUI();
			setSin(img, result, 1.0);
			while (true)
			{
				cvShowImage("Image", result);
				cvShowImage("Graph", graph2);
				key2 = cvWaitKey();
				if (key2 == '1')
				{
					(double)freq += 0.5;
					if (freq < 0.5) freq = 0.5;
					cout << "Current Freq. = ";
					printf("%f\n", freq);
					setSin(img, result, freq);
					SinGraph(graph2, freq);
				}
				if (key2 == '2')
				{
					(double)freq -= 0.5;
					if (freq < 0.5) freq = 0.5;
					cout << "Current Freq. = ";
					printf("%f\n", freq);
					setSin(img, result, freq);
					SinGraph(graph2, freq);
				}
				if (key2 == 'Q' || key2 == 'q') break;
			}
		}
		else cout << "Wrong function!" << endl;
	}
	return 0;
}

void UI() {
	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;

	cout << "Dept. of Digital Contents, Sejong University" << endl;
	cout << "Multimedia Programming Homework #2" << endl;
	cout << "Point Processing - Gamma Correction and more" << endl;

	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;
}

void GammaUI()
{
	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;

	cout << "Gamma Function" << endl << "Press following keys!" << endl;
	cout << "\t" << "1 : increasing Gamma by 0.1" << endl;
	cout << "\t" << "2 : decreasing Gamma by 0.1" << endl;
	cout << "\t" << "Q : quit the program" << endl;

	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;
}

void SinUI()
{
	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;

	cout << "Sinusoidal Function" << endl << "Press following keys!" << endl;
	cout << "\t" << "1 : increasing frequency" << endl;
	cout << "\t" << "2 : decreasing frequency" << endl;
	cout << "\t" << "Q : quit the program" << endl;

	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;
}

void setGamma(IplImage *img, IplImage* result, double gamma)
{
	CvSize size = cvGetSize(img);

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);
			CvScalar copy;
			copy.val[0] = pow(color.val[0] / 255, double(gamma)) * 255;
			copy.val[1] = pow(color.val[1] / 255, double(gamma)) * 255;
			copy.val[2] = pow(color.val[2] / 255, double(gamma)) * 255;
			cvSet2D(result, y, x, copy);
		}
	}
}

void setSin(IplImage* img, IplImage *result, double freq)
{
	CvSize size = cvGetSize(img);
	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);
			CvScalar copy;
			copy.val[0] = sin(6*freq*(color.val[0] / 255)) * 255 + 60;
			copy.val[1] = sin(6*freq*(color.val[1] / 255)  + (4*freq * 3.14159 / (double)(250 / 3))) * 255 + 40;
			copy.val[2] = sin(6*freq*(color.val[2] / 255)  + (2*freq * 3.14159 / (double)(250 / 3))) * 255 + 50;
			cvSet2D(result, y, x, copy);
		}
	}
}

void GammaGraph(IplImage *graph, double gamma)
{
	cvSet(graph, cvScalar(255, 255, 255));
	CvSize size = cvGetSize(graph);

	for (double x = 0; x <= 1; x += 0.004)
	{
		double result = pow(x, (double)gamma);
		double result2 = Rounding((double)result, 3);
		for (double y = 0; y <= 1; y += 0.001)
		{
			int y4 = y * 1000;
			int result3 = result * 1000;
			if (y4 == result3)
			{
				int y2 = y*size.height;
				int x2 = x*size.width;
				int y3 = size.height - y2 - 1;
				cvSet2D(graph, y3, x2, cvScalar(0, 0, 0));
				break;
				cout << y2 << x2 << endl;
			}
		}
	}
}

void SinGraph(IplImage *graph2, double freq)
{
	cvSet(graph2, cvScalar(255, 255, 255));
	CvSize size = cvGetSize(graph2);

	for (int x = 0; x < 250; x++)
		for (int y = 0; y < 250; y++)
		{
			int y3 = y;
			int y2 = 249 - y3;
			int result = (125 * sin((double)((3.14159 / 125)*freq)*x) + 125);
			if (y3 == result)
			{
				cvSet2D(graph2, y2, x, cvScalar(0, 0, 0));
				break;
			}
		}
}