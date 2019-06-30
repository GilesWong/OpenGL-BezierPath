// BezierPath.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include <GL/GLUT.h>
#include <math.h>
#include <vector>
#include <iterator>
#include <iostream>
#define GL_SILENCE_DEPRECATION
//这是一个点的类，用于存储其中点的坐标
class Point
{
public:
	int x, y;
	void setxy(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

int winWidth = 640;
int winHeight = 480;
//点的数量
int POINTSNUM = 0;
int POINTNEEDED = 0;
//用于存储点的向量
std::vector<Point> points;

//初始化函数
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0); //设定背景为黑色
	glColor3f(0.0, 0.0, 0.0); //绘图颜色为白色
	glPointSize(2.0); //设定点的大小为2*2像素的
	glMatrixMode(GL_PROJECTION); // 设定合适的矩阵
	glLoadIdentity(); // 是一个无参的无值函数，其功能是用一个4×4的单位矩阵来替换当前矩阵，实际上就是对当前矩阵进行初始化。也就是说，无论以前进行了多少次矩阵变换，在该命令执行后，当前矩阵均恢复成一个单位矩阵，即相当于没有进行任何矩阵变换状态
	gluOrtho2D(0.0, 600.0, 0.0, 480.0); //平行投影，四个参数分别是x,y范围
}

//绘制点
void setPoint(Point p) {
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
	glFlush();
}

// 绘制直线
void setline(Point p1, Point p2) {
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
	glFlush();
}

//递归算法
std::vector<Point> getSubPointWithSuperPoints(std::vector<Point> points, float progress) {
	if (points.size() == 1) {
		return points;
	}

	std::vector<Point> *tempArr = new std::vector<Point>();
	for (int i = 0; i < points.size() - 1; i++) {
		Point prePoint = points[i];
		Point lastPoint = points[i + 1];

		float diffX = lastPoint.x - prePoint.x;
		float diffY = lastPoint.y - prePoint.y;

		Point currentPoint = Point();
		currentPoint.setxy(prePoint.x + diffX * progress, prePoint.y + diffY * progress);
		tempArr->push_back(currentPoint);
	}
	return getSubPointWithSuperPoints(*tempArr, progress);
}

//获取所有要画的点
std::vector<Point> getBezierPathWithPoints(std::vector<Point> originalPoints) {

	std::vector<Point> *pathPoints = new std::vector<Point>();
	for (float i = 0; i <= 1.0; i += 0.01) {
		std::vector<Point> point = getSubPointWithSuperPoints(originalPoints, i);
		pathPoints->insert(pathPoints->end(), point.begin(), point.end());
	}
	return *pathPoints;
}



//display函数
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}



// 鼠标事件
void mymouseFunction(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) // 如果鼠标按下，不区分左右键的
	{
		Point *now = new Point();
		now->setxy(x, winHeight - y);
		points.push_back(*now);
		glColor3f(1.0, 0.0, 0.0);
		setPoint(*now);
		glColor3f(1.0, 0.0, 0.0);
		if (POINTSNUM > 0) {
			setline(points[POINTSNUM - 1], points[POINTSNUM]);
		}
		//如果达到了n+1个点就绘制贝塞尔曲线，并在之后给计数器清零
		if (POINTSNUM == POINTNEEDED) {
			//绘制贝塞尔曲线
			glColor3f(0.0, 0.0, 1.0); // 设定贝塞尔曲线的颜色
			std::vector<Point> pathPoints = getBezierPathWithPoints(points);
			for (int i = 0; i < pathPoints.size() - 1; i++) {
				Point pre = pathPoints[i];
				Point last = pathPoints[i + 1];
				setline(pre, last);
			}
			points.clear();
			POINTSNUM = 0;
		}
		else {
			POINTSNUM++;
		}
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	points.clear();

	winWidth = w;
	winHeight = h;
}

int main(int argc, char *argv[])
{
	int n;
	std::cout << "请输入要阶数：" << std::endl;
	std::cin >> n;
	POINTNEEDED = n;
	std::cout << "请点击窗口内来标点." << std::endl << "点数足够后，将自动进行绘制." << std::endl;
	glutInit(&argc, argv); //固定格式
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);   //缓存模式
	glutInitWindowSize(winWidth, winHeight);    //显示框的大小
	glutInitWindowPosition(100, 100); //确定显示框左上角的位置
	glutCreateWindow("贝塞尔曲线");

	init(); // 初始化
	glutMouseFunc(mymouseFunction); // 添加鼠标事件
	glutDisplayFunc(display); // 执行显示
	glutReshapeFunc(reshape); //l窗口缩放时清除内容
	glutMainLoop(); //进人GLUT事件处理循环
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
