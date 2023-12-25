#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


void houghLinesCallback(int, void*);

// 트랙바
int rho_slider = 2;
int theta_slider = 180;
int threshold_slider = 105;

Mat img;
Mat edges;

int main() {
    img = imread("c.png");
    double rsizeNum = 1;
    resize(img, img, {}, rsizeNum, rsizeNum);

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(7, 7), 0);
    cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    //imshow("check", gray);waitKey(0);

    Canny(gray, edges, 50, 150);

    // 초기 허프 선 검출을 수행
    std::vector<Vec2f> lines;
    HoughLines(edges, lines, rho_slider, CV_PI / (theta_slider), threshold_slider);

    namedWindow("Hough Lines");
    imshow("Hough Lines", img);

    // 트랙바 콜백함수 생성
    createTrackbar("Rho * 10", "Hough Lines", &rho_slider, 100, houghLinesCallback);
    createTrackbar("Theta", "Hough Lines", &theta_slider, 360, houghLinesCallback);
    createTrackbar("Threshold", "Hough Lines", &threshold_slider, 500, houghLinesCallback);
    waitKey(0);

    return 0;
}




void houghLinesCallback(int, void*) {
    double rho = static_cast<double>(rho_slider) / 10.0;
    std::vector<Vec2f> lines;
    HoughLines(edges, lines, rho, CV_PI / (theta_slider), threshold_slider);

    Mat result = img.clone();  // 이미지 크기와 채널을 복사된 이미지에 맞춤

    for (size_t i = 0; i < lines.size(); ++i) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1500 * (-b));
        pt1.y = cvRound(y0 + 1500 * (a));
        pt2.x = cvRound(x0 - 1500 * (-b));
        pt2.y = cvRound(y0 - 1500 * (a));


        line(result, pt1, pt2, Scalar(0, 255, 0), 2, LINE_AA);
    }


    //결과 표시 
    imshow("Hough Lines", result);
}
