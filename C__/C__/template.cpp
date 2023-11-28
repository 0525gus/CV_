#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
int main() {
    // �̹����� �ε��մϴ�.
    cv::Mat image = cv::imread("testC.jpg");

    // �׷��̽����Ϸ� ��ȯ�մϴ�.
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // ����ȭ�� �����մϴ�.
    cv::Mat binary;
    cv::threshold(gray, binary, 128, 255, cv::THRESH_BINARY);

    // �ܰ����� ã���ϴ�.
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // ���� �ٱ����� ����� ã���ϴ�.
    size_t largestContourIndex = 0;
    double largestContourArea = 0.0;

    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        if (area > largestContourArea) {
            largestContourArea = area;
            largestContourIndex = i;
        }
    }

    // ���� �ٱ����� ����� ���� ������ �����մϴ�.
    std::vector<cv::Point> largestContour = contours[largestContourIndex];

    // ��� �̹����� ����� �׸��ϴ�.
    cv::Mat result = image.clone();
    cv::drawContours(result, std::vector<std::vector<cv::Point>>{largestContour}, 0, cv::Scalar(0, 255, 0), 2);

    // ��� �̹����� ����մϴ�.
    cv::imshow("Result", result);
    cv::waitKey(0);

    return 0;
}
