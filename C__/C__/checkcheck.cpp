#include <opencv2/opencv.hpp>
#include <iostream>

// ���� ����
cv::Point startPoint, endPoint;
bool selectingROI = false;

// �ݹ� �Լ�
void onMouse(int event, int x, int y, int flags, void* param) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        startPoint = cv::Point(x, y);
        selectingROI = true;
    }
    else if (event == cv::EVENT_LBUTTONUP) {
        endPoint = cv::Point(x, y);
        selectingROI = false;
    }
}

int main() {
    // �̹����� �ε��մϴ�.
    cv::Mat image = cv::imread("test2.jpg");

    // �����츦 �����ϰ� ���콺 �̺�Ʈ �ݹ� �Լ��� ����մϴ�.
    cv::namedWindow("Select ROI");
    cv::setMouseCallback("Select ROI", onMouse, NULL);

    while (true) {
        cv::Mat displayImage = image.clone();

        // ROI ���� ���� ��� �簢���� ǥ���մϴ�.
        if (selectingROI) {
            cv::rectangle(displayImage, startPoint, endPoint, cv::Scalar(0, 255, 0), 2);
        }

        // ��� �̹����� ����մϴ�.
        cv::imshow("Select ROI", displayImage);

        // ESC Ű�� ������ �����մϴ�.
        int key = cv::waitKey(1);
        if (key == 27) break;
    }

    // ������ ROI�� �����մϴ�.
    cv::Rect roiRect(std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y),
        std::abs(startPoint.x - endPoint.x), std::abs(startPoint.y - endPoint.y));
    cv::Mat roi = image(roiRect);

    // ������ ROI�� ���� �۾��� �����մϴ�. (��: ���� �ٱ����� ����� ã��)
    cv::Mat gray;
    cv::cvtColor(roi, gray, cv::COLOR_BGR2GRAY);

    cv::Mat binary;
    cv::threshold(gray, binary, 128, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    size_t largestContourIndex = 0;
    double largestContourArea = 0.0;

    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        if (area > largestContourArea) {
            largestContourArea = area;
            largestContourIndex = i;
        }
    }

    std::vector<cv::Point> largestContour = contours[largestContourIndex];

    // ���� �̹����� ����� �׸��ϴ�.
    cv::drawContours(image, std::vector<std::vector<cv::Point>>{largestContour}, 0, cv::Scalar(0, 255, 0), 2);

    // ��� �̹����� ����մϴ�.
    cv::imshow("Result", image);
    cv::waitKey(0);

    return 0;
}
