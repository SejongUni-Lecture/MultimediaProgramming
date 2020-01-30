#include<opencv2\opencv.hpp>

IplImage *src;
IplImage *dst;
IplImage *buf;

void drawRectangle(IplImage *src, IplImage *dst, CvPoint p1, CvPoint p2)	//drawing rectangle
{
	if (p1.x > p2.x) std::swap(p1.x, p2.x);
	if (p1.y > p2.y) std::swap(p1.y, p2.y);			//������ �������� p2���� ���� ���� �ƴϴ� std�� �ִ� swap �Լ� �̿�

	int w = p2.x - p1.x;							//�׸� �簢���� �ʺ�� ����
	int h = p2.y - p1.y;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int x1 = float(x) / w * src->width;		//��ʽ� �̿� x1�� src���� �Ⱦ��� �ȼ���ǥ�̹Ƿ� ��� �̿�
			int y1 = float(y) / h * src->height;
			int x2 = x + p1.x;						//���콺 ��ǥ ���� start point���� ����� ��ŭ ������..!
			int y2 = y + p1.y;

			if (x1<0 || x1>src->width - 1) continue;//�̿� ���� ����
			if (x2<0 || x2>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;
			if (y2<0 || y2>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);		//x1��ǥ�� �����̹����� ���� ��Į�� ����
			cvSet2D(dst, y2, x2, f);				//dst�� ����
		}
	}

}


void myMouse(int event, int x, int y, int flags, void *)
{
	static CvPoint pt1 = cvPoint(-1, -1);
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		pt1 = cvPoint(x, y);						//rectagngle�� starting point ����
		cvCopy(dst, buf);							//���� �̹����� buf�� �����Ѵ�
	}

	if (event == CV_EVENT_MOUSEMOVE)
	{
		if (flags == CV_EVENT_FLAG_LBUTTON)
		{
			CvPoint pt2 = cvPoint(x, y);

			if (pt1.x<0 || pt1.x>dst->width - 1) return;
			if (pt1.y<0 || pt1.y>dst->height - 1) return;
			if (pt2.x<0 || pt2.x>dst->width - 1) return;
			if (pt2.y<0 || pt2.y>dst->height - 1) return;

			cvCopy(buf, dst);						//���� �̹����� ������ buf�� dst�� ������ dst���� �۾�
			drawRectangle(src, dst, pt1, pt2);
			cvShowImage("image", dst);
		}
	}
}

int main()
{
	src = cvLoadImage("d:\\lena.png");
	dst = cvCreateImage(cvGetSize(src), 8, 3);
	buf = cvCreateImage(cvGetSize(src), 8, 3);

	cvSet(dst, cvScalar(255, 255, 255));

	cvShowImage("image", dst);
	cvSetMouseCallback("image", myMouse);
	cvWaitKey();
	return 0;
}