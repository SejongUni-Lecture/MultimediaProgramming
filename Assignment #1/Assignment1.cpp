#include<opencv2/opencv.hpp>

int main()
{
	IplImage *img, *result, *result2, *result3, *result4, *result5, *result6;
	img = cvLoadImage("d:\\sejong_small.jpg");

	CvSize size = cvGetSize(img);

	result = cvCreateImage(size, 8, 3);
	result2 = cvCreateImage(size, 8, 3);
	result3 = cvCreateImage(size, 8, 3); 
	result4 = cvCreateImage(size, 8, 3);
	result5 = cvCreateImage(size, 8, 3);
	result6 = cvCreateImage(size, 8, 3);

	cvSet(result, cvScalar(0, 0, 0));
	cvSet(result2, cvScalar(0, 0, 0));
	cvSet(result3, cvScalar(0, 0, 0));
	cvSet(result4, cvScalar(0, 0, 0));
	cvSet(result5, cvScalar(0, 0, 0));
	cvSet(result6, cvScalar(0, 0, 0));

    //#1 UpsideDown Image
	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);
			int y2 = size.height - y - 1;
			cvSet2D(result, y2, x, color);
		}
	}

	//#2 half Image
	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);
			int x2 = 0;
			if (x < size.width / 2) x2= x + size.width / 2;
			else x2 = x - size.width / 2;

			cvSet2D(result2, y, x2, color);
		}
	}

	//#3 Rhombus Image(GreyScale)
	float fx = size.width / 2;
	float fy = size.height / 2;
	float a = fy / fx;

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);
			if (x < fx&&y < fy)
			{
				if (-a*x + fy > y)
				{
					int bri = color.val[0] + color.val[1] + color.val[2];
					CvScalar grey = cvScalar(bri / 3, bri / 3, bri / 3);
					cvSet2D(result3, y, x, grey);
				}
				else cvSet2D(result3, y, x, color);
			}

			else if (x >= fx&&y < fy)
			{
				if (a*x - fy > y)
				{
					int bri = color.val[0] + color.val[1] + color.val[2];
					CvScalar grey = cvScalar(bri / 3, bri / 3, bri / 3);
					cvSet2D(result3, y, x, grey);
				}
				else cvSet2D(result3, y, x, color);
			}

			else if (x < fx&&y >= fy)
			{
				if (a*x + fy < y)
				{
					int bri = color.val[0] + color.val[1] + color.val[2];
					CvScalar grey = cvScalar(bri / 3, bri / 3, bri / 3);
					cvSet2D(result3, y, x, grey);
				}
				else cvSet2D(result3, y, x, color);
			}

			else if (x >= fx && y >= fy)
			{
				if (-a*x + 3 * fy < y)
				{
					int bri = color.val[0] + color.val[1] + color.val[2];
					CvScalar grey = cvScalar(bri / 3, bri / 3, bri / 3);
					cvSet2D(result3, y, x, grey);
				}
				else cvSet2D(result3, y, x, color);
			}
		}
	}
	//#4 Elipse Image(GreyScale)

	float a2 = size.width / 2.0;
	float b = size.height / 2.0;

	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);
			if ((((x - a2)*(x - a2)) / (a2*a2)) + (((y - b)*(y - b)) / (b*b)) >= 1)
			{
				int bri = color.val[0] + color.val[1] + color.val[2];
				CvScalar grey = cvScalar(bri / 3, bri / 3, bri / 3);
				cvSet2D(result4, y, x, grey);
			}
			else cvSet2D(result4, y, x, color);
		}
	}

	//#5 Elipse Pattern Image
	float a3 = size.width / 2;
	float b2 = size.height / 2;
	int x, y;

	for (y = 0; y < size.height; y++)
	{
		for (x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);

			if ((x - a3)*(x - a3) / (15 * 15) + (y - b2)*(y - b2) / (11 * 11) <= 1
				|| ((x - a3)*(x - a3) / (30 * 30) + (y - b2)*(y - b2) / (22 * 22) > 1)
				&& ((x - a3)*(x - a3) / (45 * 45) + (y - b2)*(y - b2) / (33 * 33) <= 1))
				cvSet2D(result5, y, x, color);
			if (((x - a3)*(x - a3) / (60 * 60) + (y - b2)*(y - b2) / (44 * 44) > 1)
				&& ((x - a3)*(x - a3) / (75 * 75) + (y - b2)*(y - b2) / (55 * 55) <= 1))
				cvSet2D(result5, y, x, color);
			if (((x - a3)*(x - a3) / (90 * 90) + (y - b2)*(y - b2) / (66 * 66) > 1)
				&& ((x - a3)*(x - a3) / (105 * 105) + (y - b2)*(y - b2) / (77 * 77) <= 1))
				cvSet2D(result5, y, x, color);
			if (((x - a3)*(x - a3) / (120 * 120) + (y - b2)*(y - b2) / (88 * 88) > 1)
				&& ((x - a3)*(x - a3) / (135 * 135) + (y - b2)*(y - b2) / (99 * 99) <= 1))
				cvSet2D(result5, y, x, color);
			if (((x - a3)*(x - a3) / (150 * 150) + (y - b2)*(y - b2) / (110 * 110) > 1)
				&& ((x - a3)*(x - a3) / (165 * 165) + (y - b2)*(y - b2) / (121 * 121) <= 1))
				cvSet2D(result5, y, x, color);
			if (((x - a3)*(x - a3) / (180 * 180) + (y - b2)*(y - b2) / (132 * 132) > 1)
				&& ((x - a3)*(x - a3) / (195 * 195) + (y - b2)*(y - b2) / (154 * 154) <= 1))
				cvSet2D(result5, y, x, color);
		}
	}

	//#6 CheckerBoard Pattern Image
	int x_index = 30;
	int y_index = 23;

	for (y = 0; y < size.height; y++)
	{
		for (x = 0; x < size.width; x++)
		{
			CvScalar color = cvGet2D(img, y, x);
			int y2 = y;
			int x2 = x;

			if ((y_index / 23) % 2 == 0)
			{
				if ((x_index / 30) % 2 != 0)
				{
					if (x < x_index&&y < y_index) cvSet2D(result6, y2, x2, color);
				}
			}
			if ((y_index / 23) % 2 != 0)
			{
				if ((x_index / 30) % 2 == 0)
				{
					if (x < x_index&&y < y_index) cvSet2D(result6, y2, x2, color);
				}
			}
			if ((x + 1) % 30 == 0) x_index += 30;
		}
		if ((y + 1) % 23 == 0)
		{
			y_index += 23;
			x_index = 30;
		}
	}

	cvShowImage("(1)", result);
	cvShowImage("(2)", result2);
	cvShowImage("(3)", result3);
	cvShowImage("(4)", result4);
	cvShowImage("(5)", result5);
	cvShowImage("(6)", result6);
	cvWaitKey();
	cvDestroyWindow("Result");
	cvReleaseImage(&img);
	cvReleaseImage(&result);

	return 0;
}
