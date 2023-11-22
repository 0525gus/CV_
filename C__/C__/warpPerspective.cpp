#include <iostream>
#include "opencv2/opencv.hpp"

cv::Mat src;
cv::Point2f srcQuad[4], dstQuad[4]; //src ��İ� dst ��� ����κ�

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
    static int cnt = 0;

    if (event == cv::EVENT_LBUTTONDOWN) {
        if (cnt < 4) {
            srcQuad[cnt++] = cv::Point2f(x, y);

            cv::circle(src, cv::Point(x, y), 5, cv::Scalar(12, 255, 12), -1); //this part
            cv::imshow("src", src);

            if (cnt == 4) {
                int w = 200, h = 200; // ���簢������ ��µǰ� ����

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

    //����ȭ ��Ʈ
    int threshold_value = 100; //�Ӱ谪 ����
    cv::Mat binary_image;
    cv::threshold(src, src, threshold_value, 255, cv::THRESH_BINARY);//255?
    // cv::imshow("Binary Image", src);
    // cv::imshow("src", src); //���� ���� ���� src
    


    // ����þ� �� ����
    cv::GaussianBlur(src, src, cv::Size(5, 5), 0);

    // canny edge ����
    cv::Canny(src, src, 50, 270);



    cv::namedWindow("src");
    cv::setMouseCallback("src", on_mouse);//�̰�

    cv::imshow("src", src);
    cv::waitKey();

    cv::destroyAllWindows();
}