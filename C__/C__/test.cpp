#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void houghLinesCallback(int, void*);
void clusterLines(const std::vector<cv::Vec2f>& lines, std::vector<std::vector<cv::Vec2f>>& clusters, float distanceThreshold);

int rho_slider = 10;  // 초기 rho 값
int theta_slider = 235;  // 초기 theta 값
int threshold_slider = 67;  // 초기 threshold 값

Mat image;
Mat edges;


int main() {
    image = imread("dot45.png");

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    Canny(gray, gray, 0, 150);
    imshow("Hough Lines", gray);

    waitKey(0);

    // 결과를 표시합니다.
    namedWindow("Hough Lines");
    imshow("Hough Lines", image);


    // 트랙바를 생성하고 콜백 함수를 등록합니다.
    createTrackbar("Rho * 10", "Hough Lines", &rho_slider, 100, houghLinesCallback);
    createTrackbar("Theta", "Hough Lines", &theta_slider, 360, houghLinesCallback);
    createTrackbar("Threshold", "Hough Lines", &threshold_slider, 500, houghLinesCallback);

    waitKey(0);

    return 0;
}

void houghLinesCallback(int, void*) {
    // 이미지를 초기화
    Mat result = image.clone();

    // 트랙바의 값을 실수로 변환
    double rho = static_cast<double>(rho_slider) / 10.0;

    // 트랙바의 값이 변경될 때마다 새로운 허프 선 검출을 수행
    std::vector<Vec2f> lines;
    HoughLines(edges, lines, rho, CV_PI / (theta_slider), threshold_slider);

    // 클러스터링을 수행합니다.
    std::vector<std::vector<cv::Vec2f>> lineClusters;
    clusterLines(lines, lineClusters, 20);  // 20은 클러스터링 거리 기준

    // 클러스터 결과를 출력합니다.
    for (const auto& cluster : lineClusters) {
        // 각 클러스터에 대한 작업 수행

        // 대표 선을 그리기 위해 가장 긴 선을 찾습니다.
        float maxLength = 0;
        Vec2f representativeLine;
        for (const auto& line : cluster) {
            float length = line[0];
            if (length > maxLength) {
                maxLength = length;
                representativeLine = line;
            }
        }

        // 대표 선을 그림
        float rho = representativeLine[0], theta = representativeLine[1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(result, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    // 새로운 결과를 표시
    imshow("Hough Lines", result);
}

// 라인 클러스터링 함수
void clusterLines(const std::vector<cv::Vec2f>& lines, std::vector<std::vector<cv::Vec2f>>& clusters, float distanceThreshold) {
    clusters.clear();

    for (const auto& line : lines) {
        bool foundCluster = false;

        for (auto& cluster : clusters) {
            const auto& representativeLine = cluster[0];
            float distance = std::abs(line[0] - representativeLine[0]) + std::abs(line[1] - representativeLine[1]);

            if (distance < distanceThreshold) {
                cluster.push_back(line);
                foundCluster = true;
                break;
            }
        }

        if (!foundCluster) {
            clusters.push_back({ line });
        }
    }
}
