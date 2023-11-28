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

            // ���� �Ÿ� �̳��� ��ġ�ϴ� ��� �ش� Ư¡���� ����
            if (distance < distanceThreshold) {
                isMaximum = false;
                break;
            }
        }

        // ���������� ���õ� Ư¡���� �߰�
        if (isMaximum) {
            selectedKeypoints.push_back(keypoint);
        }
    }

    // ����� �ٽ� ������ keypoints�� ����
    keypoints = selectedKeypoints;
}

void FASTCORNER() {
    cv::Ptr<cv::Feature2D> fast = cv::FastFeatureDetector::create();

    std::vector<cv::KeyPoint> keypoints;
    fast->detect(src, keypoints);

    // NMS ������ ���� �Ÿ� �Ӱ谪
    float distanceThreshold = 10.0f;

    // NMS ����
    nonMaximumSuppression(keypoints, distanceThreshold);

    // ��� Ư¡�� ���
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
	// ����Ʈ����

	// 1. FAST �Լ� ���, �ڳʰ��� (O)
	// 2. �ڳ��� x,y��ǥ ������ nMS����(��)
    // 3.�ش� ���� Ŭ���� ������� ���� �簢������ Ȯ�� (algorithm)..??
	// 4. �� �ڳ��� �ش簪 ���� �簢�������� ���·� ����, (algorithm)
	// 5. �ڳʰ����� warpPerspective(guide ����)



	cv::namedWindow("src", cv::WINDOW_AUTOSIZE);
	double scale = 1;
	cv::resize(src, src, {}, scale, scale);
	//cv::imshow("src", src);
	//cv::waitKey(0);
	cv::destroyAllWindows();
}