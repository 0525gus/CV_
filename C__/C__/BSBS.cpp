#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// 허프 선 검출 함수
void houghLinesCallback(int, void*);

// 전역 변수
int rho_slider = 5;  // 초기 rho 값
int theta_slider = 180;  // 초기 theta 값
int threshold_slider = 150;  // 초기 threshold 값

Mat image; // 이미지 전역 변수
Mat edges; // 엣지 전역 변수

int main() {
    image = imread("dot_base.png");
    double resize_num = 0.5;
    resize(image, image, {},resize_num, resize_num);
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY); 
    Canny(gray, edges, 50, 150);

    // 초기 허프 선 검출을 수행합니다.
    std::vector<Vec2f> lines;
    HoughLines(edges, lines, rho_slider, CV_PI / (theta_slider ), threshold_slider);

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
    double rho = static_cast<double>(rho_slider) / 10.0;

    std::vector<Vec2f> lines;
    HoughLines(edges, lines, rho, CV_PI / (theta_slider ), threshold_slider);

    Mat result = image.clone();  // 이미지 크기와 채널을 복사된 이미지에 맞춤
    for (size_t i = 0; i < lines.size(); ++i) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 2000 * (-b));
        pt1.y = cvRound(y0 + 2000 * (a));
        pt2.x = cvRound(x0 - 2000 * (-b));
        pt2.y = cvRound(y0 - 2000 * (a));
        line(result, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    // 새로운 결과를 표시합니다.
    imshow("Hough Lines", result);
}
