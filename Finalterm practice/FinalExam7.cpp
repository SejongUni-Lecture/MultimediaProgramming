#include<opencv2\opencv.hpp>
#include<math.h>

IplImage *src = cvLoadImage("d:\\lena.png");
IplImage *dst;
float M[3][3];
float M1[3][3];
float M2[3][3];
float M3[3][3];
float M4[3][3];
float pivotX;
float pivotY;
float prevX;
float prevY;
float theta = 0;

void setIdentity(float A[][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == j) A[i][j] = 1.0;
			else A[i][j] = 0.0;
		}
	}
}

void setRotation(float A[][3], float theta)
{
	setIdentity(A);
	float rad = theta / 180.0 * 3.141592;
	A[0][0] = cos(rad);		A[0][1] = -sin(rad);
	A[1][0] = sin(rad);		A[1][1] = cos(rad);
}

void setTranslate(float A[][3],float tx, float ty)
{
	setIdentity(A);
	A[0][2] = tx;
	A[1][2] = ty;
}

void setMultiply(float M[][3], float A[][3], float B[][3])
{
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			M[i][j] = 0;
			for (int k = 0; k < 3; k++)
			{
				M[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void applyMatrix(IplImage *src, IplImage *dst, float M[][3])
{
	cvSet(dst, cvScalar(0, 0, 0));
	for (float y2 = 0; y2 < dst->height; y2++)
	{
		for (float x2 = 0; x2 < dst->width; x2++)
		{
			float w2 = 1;
			float x1 = M[0][0] * x2 + M[0][1] * y2 + M[0][2] * w2;
			float y1 = M[1][0] * x2 + M[1][1] * y2 + M[1][2] * w2;
			float w1 = M[2][0] * x2 + M[2][1] * y2 + M[2][2] * w2;
			x1 /= w1;
			y1 /= w1;

			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}

}

float computeAngle(float x1, float y1, float x2, float y2)
{
	float x1_diff = pivotX - x1;
	float x2_diff = pivotX - x2;
	float y1_diff = pivotY - y1;
	float y2_diff = pivotY - y2;

	float rad1 = atan2(y1_diff, x1_diff);
	float rad2 = atan2(y2_diff, x2_diff);

	float rad = rad2 - rad1;
	return rad / 3.141592 * 180.0f;
}
void myMouse(int event, int x, int y, int nFlags, void *)
{

	if (event == CV_EVENT_MOUSEMOVE)
	{
		if (nFlags == CV_EVENT_FLAG_LBUTTON)
		{
			theta += computeAngle(prevX, prevY, x, y);

			setTranslate(M1, pivotX, pivotY);
			setRotation(M2, -theta);
			setMultiply(M3, M1, M2);
			setTranslate(M4, -pivotX, -pivotY);
			setMultiply(M, M3, M4);

			applyMatrix(src, dst, M);
			cvShowImage("image", dst);
			
			prevX = x;
			prevY = y;
		}
	}

	if (event == CV_EVENT_LBUTTONDOWN)
	{
		prevX = x;
		prevY = y;

		//applyMatrix(src, dst, M);
		//cvShowImage("image", dst);
	}
}

int main()
{
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size,8,3);

	pivotX = size.width / 2.0;
	pivotY = size.height / 2.0;

	cvShowImage("image", src);
	cvSetMouseCallback("image", myMouse);
	cvWaitKey();
}