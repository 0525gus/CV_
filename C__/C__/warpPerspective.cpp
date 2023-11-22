#include <iostream>
#include "opencv2/opencv.hpp"

cv::Mat src;
cv::Point2f srcQuad[4], dstQuad[4]; //src 행렬과 dst 행렬 선언부분

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
    static int cnt = 0;

    if (event == cv::EVENT_LBUTTONDOWN) {
        if (cnt < 4) {
            srcQuad[cnt++] = cv::Point2f(x, y);

            cv::circle(src, cv::Point(x, y), 5, cv::Scalar(12, 255, 12), -1); //this part
            cv::imshow("src", src);

            if (cnt == 4) {
                int w = 200, h = 200; // 정사각형으로 출력되게 만듬

                dstQuad[0] = cv::Point2f(0, 0);
                dstQuad[1] = cv::Point2f(w - 1, 0);
                dstQuad[2] = cv::Point2f(w - 1, h - 1);
                dstQuad[3] = cv::Point2f(0, h - 1);

                cv::Mat pers = cv::getPerspectiveTransform(srcQuad, dstQuad);

                cv::Mat dst;
                cv::warpPerspective(src, dst, pers, cv::Size(w, h));

                cv::imshow("dst", dst);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    src = cv::imread("SRCC.jpg", cv::IMREAD_GRAYSCALE);//

    if (src.empty()) { 
        std::cout << "Image load failed!\n";
        return -1;
    }

    //이진화 파트
    int threshold_value = 100; //임계값 산정
    cv::Mat binary_image;
    cv::threshold(src, src, threshold_value, 255, cv::THRESH_BINARY);//255?
    // cv::imshow("Binary Image", src);
    // cv::imshow("src", src); //점을 찍을 원본 src
    


    // 가우시안 블러 적용
    cv::GaussianBlur(src, src, cv::Size(5, 5), 0);

    // canny edge 적용
    cv::Canny(src, src, 50, 270);



    cv::namedWindow("src");
    cv::setMouseCallback("src", on_mouse);//이곳

    cv::imshow("src", src);
    cv::waitKey();

    cv::destroyAllWindows();
}