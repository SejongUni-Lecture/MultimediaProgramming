#include<opencv2\opencv.hpp>
#include<iostream>
using namespace std;

void UI();				//user interface 

IplImage * src;			//불러올 이미지를 저장하는 포인터
IplImage * dst;			//src에 저장된 이미지를 불러와 처리하여 출력하는 포인터
IplImage * dst2;

char file_input[100];	//파일경로 저장할 배열 선언

int prevX = 0;
int prevY = 0;

float pivotX = 0;		//pivot의 x좌표를 저장하는 전역변수 선언
float pivotY = 0;		//pivot의 y좌표를 저장하는 전역변수 선언
float theta = 0;		//theta를 저장하는 전역변수 선언
float scale;			//이미지의 scale을 저장하는 전역변수 선언
float moveX;			//이미지의 shift 정보를 저장하는 x좌표 전역변수 선언
float moveY;			//이미지의 shift 정보를 저장하는 y좌표 전역변수 선언
float M[3][3];
float M1[3][3];
float M2[3][3];
float M3[3][3];
float M4[3][3];			//정보를 처리하게 될 동차행렬(M~M4) 선언
float pivot[3][3];		//원점으로 이동하게될 동차행렬 선언(Rotate, Scale처리를 위함)
float pivot2[3][3];		//원상태 위치로 돌려놓을 동차행렬 선언

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
	float x1_diff = pivotX - x1;								//pivot과 prev 좌표와 차이
	float y1_diff = pivotY - y1;								//pivot과 prev 좌표와 차이
	float x2_diff = pivotX - x2;								//pivot과 현재 마우스 좌표와 차이
	float y2_diff = pivotY - y2;								//pivot과 현재 마우스 좌표와 차이
	float rad1 = atan2(y1_diff, x1_diff);						
	float rad2 = atan2(y2_diff, x2_diff);						//각 각 역탄젠트 함수를 통해 라디안을 구한다
	float rad = rad2 - rad1;									
	return rad / 3.141592*180.0f;								
}

float computeScale(float x1, float y1, float x2, float y2)		//function that computes scale(what's new apart from class)
{
	float prevX_diff = pivotX - x1;								//pivot과 prev 좌표와 차이
	float prevY_diff = pivotY - y1;								//pivot과 prev 좌표와 차이
	
	float X_diff = pivotX - x2;									//pivot과 현재 마우스 좌표와 차이
	float Y_diff = pivotY - y2;									//pivot과 현재 마우스 좌표와 차이
	
	float prev_diff = sqrt(prevX_diff*prevX_diff + prevY_diff * prevY_diff);	//prev 좌표와 pivot 사이 길이
	float diff = sqrt(X_diff*X_diff + Y_diff * Y_diff);							//현재 좌표와 pivot 사이 길이

	float scale = diff / prev_diff;												//scale 계산
	return scale;																//scale 반환
}

void myMouse(int event, int x, int y, int flags, void *)		//마우스 입력 관련 처리하는 함수
{
	if (event == CV_EVENT_MOUSEMOVE)
	{
		if (flags == CV_EVENT_FLAG_LBUTTON)						//마우스 왼쪽 버튼을 누른채 있을 때 실행
		{
			theta += computeAngle(prevX, prevY, x, y);			//누적 theta
			scale /= computeScale(prevX, prevY, x, y);			//누적 scale

			setTranslate(pivot, pivotX, pivotY);				//pivot만큼 원점으로 이동한다
			setRotate(M1, -theta);								//theta만큼 rotate
			setMultiply(M2, pivot, M1);							//pivot함수와 M1 mutiply(행렬은 교환법칙이 성립안된다는 점 유의)

			setScale(M4, scale, scale);							//scale 
			setMultiply(M3, M2, M4);							//이전 행렬과 scale 행렬 multiply
				
			setTranslate(pivot2,-pivotX+moveX, -pivotY+moveY);	//shift로 이동한 곳까지 + 원위치로 이동 정보 pivot2에 저장
			setMultiply(M, M3, pivot2);							//M3 행렬과 pivot2 multiply
			applyTransform(src, dst, M);	
			
			float centerX = (pivotX - moveX);					//pivot 화면서 출력
			float centerY = (pivotY - moveY);
			for (int i = centerY - 5; i <= centerY + 5; i++)
				for (int j = centerX - 5; j <= centerX + 5; j++)
				{
					if ((i-centerY)*(i-centerY)+(j-centerX)*(j-centerX)< 25) 
						cvSet2D(dst, i, j, cvScalar(255, 0, 0));

					//cvSet2D(dst, y+i, x+j, cvScalar(0, 255, 0));					//마우스 위치 확인
					//cvSet2D(dst, prevY+i, prevX+j, cvScalar(0, 0, 255));			//이전 마우스 위치 확인
					if (centerY - 5<0 || centerY + 5>src->height - 1) continue;
					if (centerX - 5<0 || centerX + 5>src->width - 1) continue;
				}

			cvShowImage("dst", dst);												//dst 출력
			prevX = x;											
			prevY = y;
		}

		if (flags == (CV_EVENT_FLAG_LBUTTON + CV_EVENT_FLAG_SHIFTKEY))				//마우스 좌클릭+ShiftKey시 이벤트(before dragging)
		{
			moveX = prevX - x;														//이동한 만큼의 거리 계산
			moveY = prevY - y;

			setTranslate(pivot, pivotX, pivotY);									//LBUTTONDOWN에서 저장했던 theta와 scale 적용
			setRotate(M1, -theta);
			setMultiply(M2, pivot, M1);

			setScale(M4, scale, scale);
			setMultiply(M3, M2, M4);

			setTranslate(pivot2, -pivotX + moveX, -pivotY + moveY);					//이동한 만큼 shift
			setMultiply(M, M3, pivot2);
			applyTransform(src, dst, M);

			float centerX = (pivotX - moveX);										//pivot 화면서 출력
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
	else if (event == CV_EVENT_LBUTTONDOWN)										//마우스 좌클릭 시 이벤트(before dragging)
	{
		prevX = x;
		prevY = y;
		applyTransform(src, dst, M);											//dst에 M 행렬 적용

		float centerX = (pivotX - moveX);										//pivot 화면서 출력
		float centerY = (pivotY - moveY);
		for (int i = centerY - 5; i <= centerY + 5; i++)
		{
			for (int j = centerX - 5; j <= centerX + 5; j++)
			{

				if ((i - centerY)*(i - centerY) + (j - centerX)*(j - centerX)< 25)
					cvSet2D(dst, i, j, cvScalar(255, 0, 0));
				//cvSet2D(dst, y+i, x+j, cvScalar(0, 255, 0));					//마우스 위치 확인
				//cvSet2D(dst, prevY+i, prevX+j, cvScalar(0, 0, 255));			//이전 마우스 위치 확인
				if (centerY - 5<0 || centerY + 5>src->height - 1) continue;
				if (centerX - 5<0 || centerX + 5>src->width - 1) continue;
			}
		}
		cvShowImage("dst", dst);												//이미지 출력
	}

	else if (event == CV_EVENT_RBUTTONDOWN)										//마우스 우클릭 시 이벤트
	{
		pivotX = x;																//pivot setting
		pivotY = y;

		applyTransform(src, dst, M);											//기존 pivot이 담긴 이미지 초기화

		float centerX = (pivotX );												//pivot 화면서 출력
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
		cvShowImage("dst", dst);														//dst 출력
	}
}

int main()
{			
	src = NULL;						//이미지를 Load하는 IplImage 포인터

	UI();							//User Interface 화면 출력 

	while (src == NULL)				//src가 NULL일 때 무한 반복
	{
		cout << "Input File Path: ";	//cmd창에 Input File Path 출력
		cin >> file_input;				//파일 경로 입력
		src = cvLoadImage(file_input);	//입력한 파일경로 src에 저장한다.
		if (src != NULL) break;			//만약 src가 NULL이 아니면 무한 반복문에서 나오게 된다
		else cout << "File not found!" << endl;	//src가 NULL이라면, file not found을 출력한다
	}

	CvSize size = cvGetSize(src);		//불러온 이미지의 크기를 저장한다
	dst = cvCreateImage(size, 8, 3);	//dst setting
	pivotX = size.width / 2;			
	pivotY = size.height / 2;			//초기화할 pivot을 이미지의 중심으로 설정한다

	setIdentity(M);						//M 단위행렬 초기화
	setIdentity(pivot);					//pivot 단위행렬 초기화
	moveX = 0;	
	moveY = 0;
	scale = 1;							//scale 1로 초기화

	applyTransform(src, dst, M);		//단위행렬 M, src이미지를 토대로 dst에 처리	
	cvShowImage("dst", dst);			//처리된 dst 출력
		
	cvSetMouseCallback("dst", myMouse);	//마우스로 입력받을 때마다 호출
	cvWaitKey();	
}

void UI()								//cmd창에 출력되는 User Interface
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