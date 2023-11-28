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