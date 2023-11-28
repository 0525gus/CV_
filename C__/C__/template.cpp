#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
int main() {
    // 이미지를 로드합니다.
    cv::Mat image = cv::imread("testC.jpg");

    // 그레이스케일로 변환합니다.
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // 이진화를 수행합니다.
    cv::Mat binary;
    cv::threshold(gray, binary, 128, 255, cv::THRESH_BINARY);

    // 외곽선을 찾습니다.
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 가장 바깥쪽의 컨투어를 찾습니다.
    size_t largestContourIndex = 0;
    double largestContourArea = 0.0;

    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        if (area > largestContourArea) {
            largestContourArea = area;
            largestContourIndex = i;
        }
    }

    // 가장 바깥쪽의 컨투어를 담을 변수를 생성합니다.
    std::vector<cv::Point> largestContour = contours[largestContourIndex];

    // 결과 이미지에 컨투어를 그립니다.
    cv::Mat result = image.clone();
    cv::drawContours(result, std::vector<std::vector<cv::Point>>{largestContour}, 0, cv::Scalar(0, 255, 0), 2);

    // 결과 이미지를 출력합니다.
    cv::imshow("Result", result);
    cv::waitKey(0);

    return 0;
}
