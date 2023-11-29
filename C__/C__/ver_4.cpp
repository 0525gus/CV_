#include <iostream>
#include "opencv2/opencv.hpp"
#include <vector>

cv::Mat src;

void nonMaximumSuppression(std::vector<cv::KeyPoint>& keypoints, float distanceThreshold) {
    std::sort(keypoints.begin(), keypoints.end(), [](const cv::KeyPoint& a, const cv::KeyPoint& b) {
        return a.response > b.response;
        });

    std::vector<cv::KeyPoint> selectedKeypoints;

    for (const auto& keypoint : keypoints) {
        bool isMaximum = true;

        for (const auto& selectedKeypoint : selectedKeypoints) {
            float distance = cv::norm(keypoint.pt - selectedKeypoint.pt);

            // 일정 거리 이내에 위치하는 경우 해당 특징점을 제거
            if (distance < distanceThreshold) {
                isMaximum = false;
                break;
            }
        }

        // 최종적으로 선택된 특징점에 추가
        if (isMaximum) {
            selectedKeypoints.push_back(keypoint);
        }
    }

    // 결과를 다시 원래의 keypoints에 복사
    keypoints = selectedKeypoints;
}

void fastCorner() {
    cv::Ptr<cv::Feature2D> fast = cv::FastFeatureDetector::create();

    std::vector<cv::KeyPoint> keypoints;
    fast->detect(src, keypoints);

    float distanceThreshold = 10.0f;

    nonMaximumSuppression(keypoints, distanceThreshold);

    // std::cout << "검출된 코너 개수: " << keypoints.size() << std::endl;

    cv::Mat corner_image;
    cv::drawKeypoints(src, keypoints, corner_image, cv::Scalar(0, 255,0 ));

    //cv::imshow("NMS", corner_image);
    cv::waitKey(0);
}

void warpProcess(int event, int flags)
{      
    cv::Point2f srcQuad[4], dstQuad[4];

    for (int i = 0;i < 4;i++) {
      //  srcQuad[i] = Point2f(x,y); 이부분 구현
    };

    int w = 400, h = 400; 

    dstQuad[0] = cv::Point2f(0, 0);
    dstQuad[1] = cv::Point2f(w - 1, 0);
    dstQuad[2] = cv::Point2f(w - 1, h - 1);
    dstQuad[3] = cv::Point2f(0, h - 1);

    cv::Mat pers = cv::getPerspectiveTransform(srcQuad, dstQuad);//원근변환

    cv::Mat dst;
    cv::warpPerspective(src, dst, pers, cv::Size(w, h));

    cv::imshow("dst", dst);
}


int main(int argc, char* argv[]) {
    src = cv::imread("test.PNG", cv::IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cout << "Image Load failed! \n";
        return -1;
    }

    cv::GaussianBlur(src, src, cv::Size(5, 5), 0);
    cv::threshold(src, src, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::Canny(src, src, 50, 150);
    fastCorner();


    // 1. FAST 함수 사용, 코너검출 (O)
    // 2. 코너의 x,y좌표 따오고 nMS적용(ㅇ)
    // 3.해당 영역 클릭시 오른쪽 아래로 넓은 사각형으로 확장 (algorithm)..??
    // 4. 각 코너의 해당값 들을 사각형으로의 형태로 만듦, (algorithm)
    // 5. 코너값들을 warpPerspective(guide 있음)



    cv::namedWindow("src", cv::WINDOW_AUTOSIZE);


    double scale = 1;
    cv::resize(src, src, {}, scale, scale);
    //cv::imshow("src", src);
    //cv::waitKey(0);
    cv::destroyAllWindows();
}