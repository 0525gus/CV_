#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;


bool mouse_is_pressing = false;
int start_x = 0, start_y = 0;
Mat img_color;

void swap(int* x, int* y) { int temp = *x;*x = *y;*y = temp; }
void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
	Mat img_result = img_color.clone();

	
	if (event == EVENT_LBUTTONDOWN) { //keydown

		mouse_is_pressing = true;
		start_x = x; //스타트값
		start_y = y; 

		circle(img_result, Point(x, y), 10, Scalar(0, 255, 0), -1);
		imshow("img_color", img_result);

	}
	else if (event == EVENT_MOUSEMOVE) { //move

		if (mouse_is_pressing) {

			rectangle(img_result, Point(start_x, start_y), Point(x, y),
				Scalar(0, 255, 0), 5);

			imshow("img_color", img_result);
		}

	}
	else if (event == EVENT_LBUTTONUP) {//keyup

		mouse_is_pressing = false;

		if (start_x == x || start_y == y) { // BASE
			start_x -= 50;
			x += 50;
			start_y -= 50; 
			y += 50;
		}
		else if (start_x < x) { int temp = start_x; start_x = x;x = temp; }
		else if (start_y < y) { int temp = start_y; start_y = y;y = temp; }
		cout << start_x<<x << endl;
		Mat img_cat(img_color, Rect(start_x, start_y, x - start_x, y - start_y));
		cvtColor(img_cat, img_cat, COLOR_BGR2GRAY);
		cvtColor(img_cat, img_cat, COLOR_GRAY2BGR);

		img_cat.copyTo(img_result(Rect(start_x, start_y, x - start_x, y - start_y)));
		imshow("img_color", img_result);
		imshow("img_cat", img_cat);
	}
}




int main()
{
	img_color = imread("cat.jpg", IMREAD_COLOR);

	imshow("img_color", img_color);

	setMouseCallback("img_color", mouse_callback);

	waitKey(0);
	
	destroyAllWindows();
}