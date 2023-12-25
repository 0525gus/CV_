#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void houghLinesCallback(int, void*);
void clusterLines(const std::vector<cv::Vec2f>& lines, std::vector<std::vector<cv::Vec2f>>& clusters, float distanceThreshold);

int rho_slider = 10;  // �ʱ� rho ��
int theta_slider = 235;  // �ʱ� theta ��
int threshold_slider = 67;  // �ʱ� threshold ��

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

    // ����� ǥ���մϴ�.
    namedWindow("Hough Lines");
    imshow("Hough Lines", image);


    // Ʈ���ٸ� �����ϰ� �ݹ� �Լ��� ����մϴ�.
    createTrackbar("Rho * 10", "Hough Lines", &rho_slider, 100, houghLinesCallback);
    createTrackbar("Theta", "Hough Lines", &theta_slider, 360, houghLinesCallback);
    createTrackbar("Threshold", "Hough Lines", &threshold_slider, 500, houghLinesCallback);

    waitKey(0);

    return 0;
}

void houghLinesCallback(int, void*) {
    // �̹����� �ʱ�ȭ
    Mat result = image.clone();

    // Ʈ������ ���� �Ǽ��� ��ȯ
    double rho = static_cast<double>(rho_slider) / 10.0;

    // Ʈ������ ���� ����� ������ ���ο� ���� �� ������ ����
    std::vector<Vec2f> lines;
    HoughLines(edges, lines, rho, CV_PI / (theta_slider), threshold_slider);

    // Ŭ�����͸��� �����մϴ�.
    std::vector<std::vector<cv::Vec2f>> lineClusters;
    clusterLines(lines, lineClusters, 20);  // 20�� Ŭ�����͸� �Ÿ� ����

    // Ŭ������ ����� ����մϴ�.
    for (const auto& cluster : lineClusters) {
        // �� Ŭ�����Ϳ� ���� �۾� ����

        // ��ǥ ���� �׸��� ���� ���� �� ���� ã���ϴ�.
        float maxLength = 0;
        Vec2f representativeLine;
        for (const auto& line : cluster) {
            float length = line[0];
            if (length > maxLength) {
                maxLength = length;
                representativeLine = line;
            }
        }

        // ��ǥ ���� �׸�
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

    // ���ο� ����� ǥ��
    imshow("Hough Lines", result);
}

// ���� Ŭ�����͸� �Լ�
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
