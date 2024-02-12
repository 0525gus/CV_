#include "LBS_ImgPreprocessor.h"
#include "LBS_digitization_main.h"
#include "GenBGArray.h"
#include <format>
#include <filesystem>
#include <fstream>

#define USE_VIDEO
#define USE_DEBUG
using namespace cv;
using std::cout, std::endl;
using namespace LBS;

/// for Digitizing
vector<vector<int>> digitizied_array;
pair<int, int> movementIndex;



/// for Debruijn ///
const std::size_t subRow = 5;
const std::size_t subCol = 5;
const std::string alpha = "0123";
std::vector<int> subLocation(2);
LBS::BGArray BA(alpha, subRow, subCol);
std::size_t alphaLen = alpha.length();


LBS::Correction Cor;




#ifdef USE_DEBUG
int sequece_number = 0;
void snapError(Mat& img, int rotue_select = 0) {
    std::string route;
    if (rotue_select == 0) {
        route = "../test_case/5X5/real-time/source/origin/src";
    }
    else if (rotue_select == 1) {
        route = "../test_case/5X5/real-time/source/Processed/proImg";
    }
    else if (rotue_select == 2) {
        route = "../test_case/5X5/real-time/source/warped/warpImg";
    }

    std::string route_formatted = std::format("{}{}.jpg", route, sequece_number);
    cout << route_formatted << endl;
    bool isSaved = imwrite(route_formatted, img); // 이미지 저장
    if (isSaved) {
        cout << "Image saved successfully!" << endl;
    }
    else {
        cout << "Error in saving image!" << endl;
    }
}
void AppendTextToFile(int x, int y) {
    std::string file_route = "../test_case/5X5/real-time/source/indexing_result.txt";
    std::string formattted = std::format("result{}= {},{}\n", sequece_number, x, y);

    std::ofstream outfile(file_route, std::ios::app);
    outfile << formattted;
    if (!outfile) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return;
    }
    outfile.close();
}
#endif //!USE DEBUG




void drawBaseLineCP() {
    cv::circle(Cor.src, Cor.closest_baseline_dot.pt, 5, cv::Scalar(255, 255, 0), -1); //CP 그림
}
void drawImgCenterPoint() {
    cv::circle(Cor.src, Cor.image_center_point, 5, cv::Scalar(255, 255, 0), -1); //CP 그림
}


int main() {
    while (true) {
        cv::TickMeter tm;
        Cor.preProcessImg();
        Mat ORIGIN = Cor.src.clone();

        imshow("test", Cor.src);
        if (waitKey(0) == 27) {
            return 0;

        }
    Cor.resetData();
    }
    return 0;
}







