#include <opencv2/opencv.hpp>
#include <iostream>

// 전역 변수
cv::Point startPoint, endPoint;
bool selectingROI = false;

// 콜백 함수
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
    // 이미지를 로드합니다.
    cv::Mat image = cv::imread("test2.jpg");

    // 윈도우를 생성하고 마우스 이벤트 콜백 함수를 등록합니다.
    cv::namedWindow("Select ROI");
    cv::setMouseCallback("Select ROI", onMouse, NULL);

    while (true) {
        cv::Mat displayImage = image.clone();

        // ROI 선택 중인 경우 사각형을 표시합니다.
        if (selectingROI) {
            cv::rectangle(displayImage, startPoint, endPoint, cv::Scalar(0, 255, 0), 2);
        }

        // 결과 이미지를 출력합니다.
        cv::imshow("Select ROI", displayImage);

        // ESC 키를 누르면 종료합니다.
        int key = cv::waitKey(1);
        if (key == 27) break;
    }

    // 선택한 ROI를 추출합니다.
    cv::Rect roiRect(std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y),
        std::abs(startPoint.x - endPoint.x), std::abs(startPoint.y - endPoint.y));
    cv::Mat roi = image(roiRect);

    // 추출한 ROI에 대해 작업을 수행합니다. (예: 가장 바깥쪽의 컨투어를 찾기)
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

    // 원본 이미지에 컨투어를 그립니다.
    cv::drawContours(image, std::vector<std::vector<cv::Point>>{largestContour}, 0, cv::Scalar(0, 255, 0), 2);

    // 결과 이미지를 출력합니다.
    cv::imshow("Result", image);
    cv::waitKey(0);

    return 0;
}
