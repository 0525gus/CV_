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

void fastCorner() {
    cv::Ptr<cv::Feature2D> fast = cv::FastFeatureDetector::create();

    std::vector<cv::KeyPoint> keypoints;
    fast->detect(src, keypoints);

    float distanceThreshold = 10.0f;

    nonMaximumSuppression(keypoints, distanceThreshold);

    // std::cout << "����� �ڳ� ����: " << keypoints.size() << std::endl;

    cv::Mat corner_image;
    cv::drawKeypoints(src, keypoints, corner_image, cv::Scalar(0, 255,0 ));

    //cv::imshow("NMS", corner_image);
    cv::waitKey(0);
}

void warpProcess(int event, int flags)
{      
    cv::Point2f srcQuad[4], dstQuad[4];

    for (int i = 0;i < 4;i++) {
      //  srcQuad[i] = Point2f(x,y); �̺κ� ����
    };

    int w = 400, h = 400; 

    dstQuad[0] = cv::Point2f(0, 0);
    dstQuad[1] = cv::Point2f(w - 1, 0);
    dstQuad[2] = cv::Point2f(w - 1, h - 1);
    dstQuad[3] = cv::Point2f(0, h - 1);

    cv::Mat pers = cv::getPerspectiveTransform(srcQuad, dstQuad);//���ٺ�ȯ

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


    // 1. FAST �Լ� ���, �ڳʰ��� (O)
    // 2. �ڳ��� x,y��ǥ ������ nMS����(��)
    // 3.�ش� ���� Ŭ���� ������ �Ʒ��� ���� �簢������ Ȯ�� (algorithm)..??
    // 4. �� �ڳ��� �ش簪 ���� �簢�������� ���·� ����, (algorithm)
    // 5. �ڳʰ����� warpPerspective(guide ����)



    cv::namedWindow("src", cv::WINDOW_AUTOSIZE);


    double scale = 1;
    cv::resize(src, src, {}, scale, scale);
    //cv::imshow("src", src);
    //cv::waitKey(0);
    cv::destroyAllWindows();
}