#include <opencv2/opencv.hpp>


using namespace cv;



bool mouse_is_pressing = false;
int start_x = -1, start_y = -1;
Mat img_color;


void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
	Mat img_result = img_color.clone();


	if (event == EVENT_LBUTTONDOWN) {

		mouse_is_pressing = true;
		start_x = x;
		start_y = y;

		circle(img_result, Point(x, y), 10, Scalar(0, 255, 0), -1);
		imshow("img_color", img_result);

	}
	else if (event == EVENT_MOUSEMOVE) {

		if (mouse_is_pressing) {

			rectangle(img_result, Point(start_x, start_y), Point(x, y),
				Scalar(0, 255, 0), 3);

			imshow("img_color", img_result);
		}

	}
	else if (event == EVENT_LBUTTONUP) {

		mouse_is_pressing = false;

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
	img_color = imread("dot_base.png", IMREAD_COLOR);

	imshow("img_color", img_color);
	setMouseCallback("img_color", mouse_callback);

	waitKey(0);
	destroyAllWindows();
}