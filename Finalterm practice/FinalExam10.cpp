#include<opencv2\opencv.hpp>

IplImage *g_img;
IplImage *g_buf;

CvPoint g_pt[100];				//녹색 점 set
int g_num = 0;

void myMouse(int event, int x, int y, int flags, void *)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		g_pt[g_num] = cvPoint(x, y);
		g_num++;

		cvCopy(g_img, g_buf);			//buf에 원본이미지 저장
			
		for (int i = 0; i < g_num; i++)	//찍힌 점 수에 만큼
		{
			cvCircle(g_buf, g_pt[i], 4, cvScalar(0, 255, 0), -1);	//원을 그리고
			if (i < g_num - 1) cvLine(g_buf, g_pt[i], g_pt[i + 1], cvScalar(0, 255, 0), 1);	//선을 그린다
		}
		cvShowImage("image", g_buf);	//이미지 출력
	}
}

int main()
{
	g_img = cvLoadImage("d:\\lena.jpg");
	g_buf = cvCreateImage(cvGetSize(g_img), 8, 3);

	cvShowImage("image", g_img);
	cvSetMouseCallback("image", myMouse);

	cvWaitKey();		//키보드 입력을 받을 때

	for (float time = 0; time < g_num - 1; time+=0.01)
	{
		cvCopy(g_img, g_buf);

		//linear interpolation 사용

		int ind1 = (int)time;
		int ind2 = ind1 + 1;
		if (ind2 > g_num - 1) ind2 = g_num - 1;		//범위 조절

		float alpha = time - ind1;					//미세한 차이...! 

		CvPoint pt;
		pt.x = (1 - alpha)*g_pt[ind1].x + (alpha)*g_pt[ind2].x;		//(1-a)g.val[k]+a*g.val[k+1]
		pt.y = (1 - alpha)*g_pt[ind1].y + (alpha)*g_pt[ind2].y;
		
		cvCircle(g_buf, pt, 10, cvScalar(0, 255, 0), -1);

		cvShowImage("image", g_buf);
		cvWaitKey(10);												//키보드 입력..
	}

	cvWaitKey();

	cvReleaseImage(&g_img);
	cvReleaseImage(&g_buf);

	return 0;
}