#include <opencv2/opencv.hpp>

using namespace cv;

void houghLinesCallback(int, void*);

int rho_slider = 10;  // �ʱ� rho ��
int theta_slider = 235;  // �ʱ� theta ��
int threshold_slider = 67;  // �ʱ� threshold ��

Mat img;


int main() {
    img = imread("dot_base.png");

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    Canny(gray, gray, 0, 150);
    imshow("Hough Lines", gray);
    waitKey(0);


    namedWindow("Hough Lines");
    imshow("Hough Lines", img);


    waitKey(0);
    
    return 0;
}


