#include <opencv2/opencv.hpp>
#include "LBS_ImgPreprocessor.h"
using namespace cv;
using std::cout, std::endl;
// 전역 변수
int minT = 20;
int maxT = 120;
int Tstep = 10;
int minA = 6;
//int dist = 3; //고정

LBS::ImgPreprocessor Cor;

void on_trackbar(int, void*) {
    Cor.resetData();
    //Cor.preProcessImg();
    TickMeter tm;
    tm.start();
    cv::SimpleBlobDetector::Params params;
    //Circularity
    params.filterByCircularity = true;
    params.minCircularity =0.75;
    params.maxCircularity = 1; //fixed

    //Threshold
    params.minThreshold = minT;
    params.maxThreshold = maxT;
    params.thresholdStep = Tstep;

    //Area
    params.filterByArea = true;
    params.minArea = minA;
    params.maxArea = 150; //fixed
    params.minDistBetweenBlobs = 3; //fixed

    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
    detector->detect(Cor.src, Cor.detected_all_keypoints);

    //processing time
    tm.stop();
    cout << tm.getTimeMilli() / 1000 << endl;
    Cor.drawDetectedDotsBySBD();
    imshow("Detected Blobs", Cor.src);
}

int main() {
    namedWindow("Trackbars", WINDOW_NORMAL);
    createTrackbar("MinT", "Trackbars", &minT, maxT, on_trackbar);
    createTrackbar("MaxT ", "Trackbars", &maxT, 255, on_trackbar);
    createTrackbar("TStep", "Trackbars", &Tstep, 100, on_trackbar);
    createTrackbar("minA", "Trackbars", &minA, 100, on_trackbar);
    //createTrackbar("Dist", "Trackbars", &dist, 100, on_trackbar);

    //on_trackbar(0, 0);//초기 on_trackbar setting

    while (true) {
        if (waitKey(1) == 27) {
            break;
        }
    }


    return 0;
}
