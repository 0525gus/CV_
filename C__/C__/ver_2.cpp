#include <iostream>
#include "opencv2/opencv.hpp"

cv::Mat src;

cv::Point2f srcQuad[4], dstQuad[4];

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

void FASTCORNER() {
    cv::Ptr<cv::Feature2D> fast = cv::FastFeatureDetector::create();

    std::vector<cv::KeyPoint> keypoints;
    fast->detect(src, keypoints);

    // NMS 적용을 위한 거리 임계값
    float distanceThreshold = 10.0f;

    // NMS 적용
    nonMaximumSuppression(keypoints, distanceThreshold);

    // 결과 특징점 출력
    std::cout << "Number of keypoints after NMS: " << keypoints.size() << std::endl;

    cv::Mat image_with_keypoints;
    cv::drawKeypoints(src, keypoints, image_with_keypoints, cv::Scalar(0, 0, 255));

    cv::imshow("FAST Keypoints with NMS", image_with_keypoints);
    cv::waitKey(0);
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
	FASTCORNER();




	//
	// FAST	//
	// 이파트에서

	// 1. FAST 함수 사용, 코너검출 (O)
	// 2. 코너의 x,y좌표 따오고 nMS적용(ㅇ)
    // 3.해당 영역 클릭시 사방으로 넓은 사각형으로 확장 (algorithm)..??
	// 4. 각 코너의 해당값 들을 사각형으로의 형태로 만듦, (algorithm)
	// 5. 코너값들을 warpPerspective(guide 있음)



	cv::namedWindow("src", cv::WINDOW_AUTOSIZE);
	double scale = 1;
	cv::resize(src, src, {}, scale, scale);
	//cv::imshow("src", src);
	//cv::waitKey(0);
	cv::destroyAllWindows();
}