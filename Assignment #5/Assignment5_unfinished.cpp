#include<opencv2\opencv.hpp>
#include<iostream>
using namespace std;

void UI();				//user interface 

IplImage * src;			//�ҷ��� �̹����� �����ϴ� ������
IplImage * dst;			//src�� ����� �̹����� �ҷ��� ó���Ͽ� ����ϴ� ������
IplImage * dst2;

char file_input[100];	//���ϰ�� ������ �迭 ����

int prevX = 0;
int prevY = 0;

float pivotX = 0;		//pivot�� x��ǥ�� �����ϴ� �������� ����
float pivotY = 0;		//pivot�� y��ǥ�� �����ϴ� �������� ����
float theta = 0;		//theta�� �����ϴ� �������� ����
float scale;			//�̹����� scale�� �����ϴ� �������� ����
float moveX;			//�̹����� shift ������ �����ϴ� x��ǥ �������� ����
float moveY;			//�̹����� shift ������ �����ϴ� y��ǥ �������� ����
float M[3][3];
float M1[3][3];
float M2[3][3];
float M3[3][3];
float M4[3][3];			//������ ó���ϰ� �� �������(M~M4) ����
float pivot[3][3];		//�������� �̵��ϰԵ� ������� ����(Rotate, Scaleó���� ����)
float pivot2[3][3];		//������ ��ġ�� �������� ������� ����

void setIdentity(float M[][3])									//function that sets Identity Matrix
{
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			M[i][j] = (i == j) ? 1.0 : 0.0;
}

void copyMatrix(float M1[][3], float M2[][3])					//function that copies Matrix
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			M1[i][j] = M2[i][j];
		}
	}
}

void setTranslate(float M[][3], float tx, float ty)				//function that sets Translation Matrix
{
	setIdentity(M);
	M[0][2] = tx;
	M[1][2] = ty;
}
void setScale(float M[][3], float sx, float sy)					//function that sets Scale Matrix
{
	setIdentity(M);
	M[0][0] = sx;
	M[1][1] = sy;
}

void setRotate(float M[][3], float theta)						//function that sets Rotation Matrix
{
	setIdentity(M);
	float rad = theta / 180.0*3.141592;
	M[0][0] = cos(rad);		M[0][1] = -sin(rad);
	M[1][0] = sin(rad);		M[1][1] = cos(rad);
}

void setMultiply(float M[][3], float A[][3], float B[][3])		//function that Multiplies Matrix
{
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
		{
			M[i][j] = 0;
			for (int k = 0; k<3; k++)
				M[i][j] += A[i][k] * B[k][j];
		}
}

void applyTransform(IplImage *src, IplImage *dst, float M[][3])	//function that applies transformation 
{
	cvSet(dst, cvScalar(0, 0, 0));
	for (float y2 = 0; y2<dst->height; y2++)
		for (float x2 = 0; x2<dst->width; x2++)
		{
			float w2 = 1;
			float x1 = M[0][0] * x2 + M[0][1] * y2 + M[0][2] * w2;
			float y1 = M[1][0] * x2 + M[1][1] * y2 + M[1][2] * w2;
			float w1 = M[2][0] * x2 + M[2][1] * y2 + M[2][2] * w2;;
			x1 /= w1;
			y1 /= w1;
			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;
			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
}

float computeAngle(float x1, float y1, float x2, float y2)		//function that computes angle(what's new apart from class)
{
	float x1_diff = pivotX - x1;								//pivot�� prev ��ǥ�� ����
	float y1_diff = pivotY - y1;								//pivot�� prev ��ǥ�� ����
	float x2_diff = pivotX - x2;								//pivot�� ���� ���콺 ��ǥ�� ����
	float y2_diff = pivotY - y2;								//pivot�� ���� ���콺 ��ǥ�� ����
	float rad1 = atan2(y1_diff, x1_diff);						
	float rad2 = atan2(y2_diff, x2_diff);						//�� �� ��ź��Ʈ �Լ��� ���� ������ ���Ѵ�
	float rad = rad2 - rad1;									
	return rad / 3.141592*180.0f;								
}

float computeScale(float x1, float y1, float x2, float y2)		//function that computes scale(what's new apart from class)
{
	float prevX_diff = pivotX - x1;								//pivot�� prev ��ǥ�� ����
	float prevY_diff = pivotY - y1;								//pivot�� prev ��ǥ�� ����
	
	float X_diff = pivotX - x2;									//pivot�� ���� ���콺 ��ǥ�� ����
	float Y_diff = pivotY - y2;									//pivot�� ���� ���콺 ��ǥ�� ����
	
	float prev_diff = sqrt(prevX_diff*prevX_diff + prevY_diff * prevY_diff);	//prev ��ǥ�� pivot ���� ����
	float diff = sqrt(X_diff*X_diff + Y_diff * Y_diff);							//���� ��ǥ�� pivot ���� ����

	float scale = diff / prev_diff;												//scale ���
	return scale;																//scale ��ȯ
}

void myMouse(int event, int x, int y, int flags, void *)		//���콺 �Է� ���� ó���ϴ� �Լ�
{
	if (event == CV_EVENT_MOUSEMOVE)
	{
		if (flags == CV_EVENT_FLAG_LBUTTON)						//���콺 ���� ��ư�� ����ä ���� �� ����
		{
			theta += computeAngle(prevX, prevY, x, y);			//���� theta
			scale /= computeScale(prevX, prevY, x, y);			//���� scale

			setTranslate(pivot, pivotX, pivotY);				//pivot��ŭ �������� �̵��Ѵ�
			setRotate(M1, -theta);								//theta��ŭ rotate
			setMultiply(M2, pivot, M1);							//pivot�Լ��� M1 mutiply(����� ��ȯ��Ģ�� �����ȵȴٴ� �� ����)

			setScale(M4, scale, scale);							//scale 
			setMultiply(M3, M2, M4);							//���� ��İ� scale ��� multiply
				
			setTranslate(pivot2,-pivotX+moveX, -pivotY+moveY);	//shift�� �̵��� ������ + ����ġ�� �̵� ���� pivot2�� ����
			setMultiply(M, M3, pivot2);							//M3 ��İ� pivot2 multiply
			applyTransform(src, dst, M);	
			
			float centerX = (pivotX - moveX);					//pivot ȭ�鼭 ���
			float centerY = (pivotY - moveY);
			for (int i = centerY - 5; i <= centerY + 5; i++)
				for (int j = centerX - 5; j <= centerX + 5; j++)
				{
					if ((i-centerY)*(i-centerY)+(j-centerX)*(j-centerX)< 25) 
						cvSet2D(dst, i, j, cvScalar(255, 0, 0));

					//cvSet2D(dst, y+i, x+j, cvScalar(0, 255, 0));					//���콺 ��ġ Ȯ��
					//cvSet2D(dst, prevY+i, prevX+j, cvScalar(0, 0, 255));			//���� ���콺 ��ġ Ȯ��
					if (centerY - 5<0 || centerY + 5>src->height - 1) continue;
					if (centerX - 5<0 || centerX + 5>src->width - 1) continue;
				}

			cvShowImage("dst", dst);												//dst ���
			prevX = x;											
			prevY = y;
		}

		if (flags == (CV_EVENT_FLAG_LBUTTON + CV_EVENT_FLAG_SHIFTKEY))				//���콺 ��Ŭ��+ShiftKey�� �̺�Ʈ(before dragging)
		{
			moveX = prevX - x;														//�̵��� ��ŭ�� �Ÿ� ���
			moveY = prevY - y;

			setTranslate(pivot, pivotX, pivotY);									//LBUTTONDOWN���� �����ߴ� theta�� scale ����
			setRotate(M1, -theta);
			setMultiply(M2, pivot, M1);

			setScale(M4, scale, scale);
			setMultiply(M3, M2, M4);

			setTranslate(pivot2, -pivotX + moveX, -pivotY + moveY);					//�̵��� ��ŭ shift
			setMultiply(M, M3, pivot2);
			applyTransform(src, dst, M);

			float centerX = (pivotX - moveX);										//pivot ȭ�鼭 ���
			float centerY = (pivotY - moveY);
			for (int i = centerY - 5; i <= centerY + 5; i++)
			{
				for (int j = centerX - 5; j <= centerX + 5; j++)
				{

					if ((i - centerY)*(i - centerY) + (j - centerX)*(j - centerX) < 25)
						cvSet2D(dst, i, j, cvScalar(255, 0, 0));

					//cvSet2D(dst, y+i, x+j, cvScalar(0, 255, 0));
					//cvSet2D(dst, prevY+i, prevX+j, cvScalar(0, 0, 255));
					if (centerY - 5<0 || centerY + 5>src->height - 1) continue;
					if (centerX - 5<0 || centerX + 5>src->width - 1) continue;
				}
			}
			cvShowImage("dst", dst);
		}
		if (flags == CV_EVENT_FLAG_RBUTTON)
		{
			pivotX = x;
			pivotY = y;
		}
	}
	else if (event == CV_EVENT_LBUTTONDOWN)										//���콺 ��Ŭ�� �� �̺�Ʈ(before dragging)
	{
		prevX = x;
		prevY = y;
		applyTransform(src, dst, M);											//dst�� M ��� ����

		float centerX = (pivotX - moveX);										//pivot ȭ�鼭 ���
		float centerY = (pivotY - moveY);
		for (int i = centerY - 5; i <= centerY + 5; i++)
		{
			for (int j = centerX - 5; j <= centerX + 5; j++)
			{

				if ((i - centerY)*(i - centerY) + (j - centerX)*(j - centerX)< 25)
					cvSet2D(dst, i, j, cvScalar(255, 0, 0));
				//cvSet2D(dst, y+i, x+j, cvScalar(0, 255, 0));					//���콺 ��ġ Ȯ��
				//cvSet2D(dst, prevY+i, prevX+j, cvScalar(0, 0, 255));			//���� ���콺 ��ġ Ȯ��
				if (centerY - 5<0 || centerY + 5>src->height - 1) continue;
				if (centerX - 5<0 || centerX + 5>src->width - 1) continue;
			}
		}
		cvShowImage("dst", dst);												//�̹��� ���
	}

	else if (event == CV_EVENT_RBUTTONDOWN)										//���콺 ��Ŭ�� �� �̺�Ʈ
	{
		pivotX = x;																//pivot setting
		pivotY = y;

		applyTransform(src, dst, M);											//���� pivot�� ��� �̹��� �ʱ�ȭ

		float centerX = (pivotX );												//pivot ȭ�鼭 ���
		float centerY = (pivotY );
		for (int i = centerY - 5; i <= centerY + 5; i++)
		{
			for (int j = centerX - 5; j <= centerX + 5; j++)
			{

				if ((i - centerY)*(i - centerY) + (j - centerX)*(j - centerX) < 25)
					cvSet2D(dst, i, j, cvScalar(255, 0, 0));

				//cvSet2D(dst, y+i, x+j, cvScalar(0, 255, 0));
				//cvSet2D(dst, prevY+i, prevX+j, cvScalar(0, 0, 255));
				if (centerY - 5<0 || centerY + 5>src->height - 1) continue;
				if (centerX - 5<0 || centerX + 5>src->width - 1) continue;
			}
		}
		cvShowImage("dst", dst);														//dst ���
	}
}

int main()
{			
	src = NULL;						//�̹����� Load�ϴ� IplImage ������

	UI();							//User Interface ȭ�� ��� 

	while (src == NULL)				//src�� NULL�� �� ���� �ݺ�
	{
		cout << "Input File Path: ";	//cmdâ�� Input File Path ���
		cin >> file_input;				//���� ��� �Է�
		src = cvLoadImage(file_input);	//�Է��� ���ϰ�� src�� �����Ѵ�.
		if (src != NULL) break;			//���� src�� NULL�� �ƴϸ� ���� �ݺ������� ������ �ȴ�
		else cout << "File not found!" << endl;	//src�� NULL�̶��, file not found�� ����Ѵ�
	}

	CvSize size = cvGetSize(src);		//�ҷ��� �̹����� ũ�⸦ �����Ѵ�
	dst = cvCreateImage(size, 8, 3);	//dst setting
	pivotX = size.width / 2;			
	pivotY = size.height / 2;			//�ʱ�ȭ�� pivot�� �̹����� �߽����� �����Ѵ�

	setIdentity(M);						//M ������� �ʱ�ȭ
	setIdentity(pivot);					//pivot ������� �ʱ�ȭ
	moveX = 0;	
	moveY = 0;
	scale = 1;							//scale 1�� �ʱ�ȭ

	applyTransform(src, dst, M);		//������� M, src�̹����� ���� dst�� ó��	
	cvShowImage("dst", dst);			//ó���� dst ���
		
	cvSetMouseCallback("dst", myMouse);	//���콺�� �Է¹��� ������ ȣ��
	cvWaitKey();	
}

void UI()								//cmdâ�� ��µǴ� User Interface
{
	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;

	cout << "Dept. of Digital Contents, Sejong University" << endl;
	cout << "Multimedia Programming Homework #5" << endl;
	cout << "Image Affine Transformation" << endl;

	for (int i = 1; i <= 45; i++) cout << "-";
	cout << endl;

	cout << "Left Mouse Dragging: Rotating/Scaling" << endl;
	cout << "Left Mouse Dragging+ShiftKey: Translating" << endl;
	cout << "Right Mouse Click: Resetting the pivot point" << endl;

	for (int i = 1; i <= 45; i++) cout << "=";
	cout << endl;
}