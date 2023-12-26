#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

struct Line {
    Point pt1, pt2;
};

void houghLinesCallback(int, void*);

// 트랙바
int rho_slider = 2;
int theta_slider = 180;
int threshold_slider = 109;

Mat img;
Mat edges;

vector<Line> linePoints;
vector<vector<Line>> clusters;

int main() {
    img = imread("b.png");
    double rsizeNum = 1;
    resize(img, img, {}, rsizeNum, rsizeNum);

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(7, 7), 0);
    cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    Canny(gray, edges, 50, 150);

    namedWindow("Hough Lines");
    imshow("Hough Lines", img);

    // 트랙바 콜백함수 생성
    createTrackbar("Rho * 10", "Hough Lines", &rho_slider, 100, houghLinesCallback);
    createTrackbar("Theta", "Hough Lines", &theta_slider, 360, houghLinesCallback);
    createTrackbar("Threshold", "Hough Lines", &threshold_slider, 500, houghLinesCallback);
    waitKey(0);

    return 0;
}

void houghLinesCallback(int, void*) {
    double rho = static_cast<double>(rho_slider) / 10.0;
    vector<Vec2f> lines;
    HoughLines(edges, lines, rho, CV_PI / (theta_slider), threshold_slider);

    Mat result = img.clone();

    linePoints.clear();
    clusters.clear();

    for (size_t i = 0; i < lines.size(); ++i) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1500 * (-b));
        pt1.y = cvRound(y0 + 1500 * (a));
        pt2.x = cvRound(x0 - 1500 * (-b));
        pt2.y = cvRound(y0 - 1500 * (a));

        linePoints.push_back({ pt1, pt2 });
    }

    for (const auto& line : linePoints) {
        bool foundCluster = false;
        for (auto& cluster : clusters) {
            const auto& representative = cluster.front();
            if (abs(representative.pt1.x - line.pt1.x) < 10 && abs(representative.pt1.y - line.pt1.y) < 10 &&
                abs(representative.pt2.x - line.pt2.x) < 10 && abs(representative.pt2.y - line.pt2.y) < 10) {
                cluster.push_back(line);
                foundCluster = true;
                break;
            }
        }

        if (!foundCluster) {
            clusters.push_back({ line });
        }
    }

    for (const auto& cluster : clusters) {
        Point pt1(0, 0), pt2(0, 0);
        for (const auto& line : cluster) {
            pt1 += line.pt1;
            pt2 += line.pt2;
        }
        pt1.x /= cluster.size();
        pt1.y /= cluster.size();
        pt2.x /= cluster.size();
        pt2.y /= cluster.size();

        line(result, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    imshow("Hough Lines", result);
}
