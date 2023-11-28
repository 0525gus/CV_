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