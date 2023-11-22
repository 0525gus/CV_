#include <opencv2/calib3d/calib3d.hpp> //ī�޶� Ķ���극�̼� ���� �������
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

Size CHECKERBOARD(6, 9);

// Criteria for corner refinement
TermCriteria criteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.001);

// Vector to store 3D world coordinates of checkerboard corners
vector<vector<Point3f>> objpoints;

// Vector to store 2D image coordinates of checkerboard corners
vector<vector<Point2f>> imgpoints;

// 3D world coordinates definition
vector<Point3f> objp;

int main() {
    // Initialize 3D world coordinates
    for (int i = 0; i < CHECKERBOARD.height; ++i) {
        for (int j = 0; j < CHECKERBOARD.width; ++j) {
            objp.push_back(Point3f(j, i, 0));
        }
    }

    // Path to images
    vector<String> images;
    glob("./images/*.jpg", images, false);

    Mat img, gray;

    // Loop through images
    for (size_t i = 0; i < images.size(); ++i) {
        img = imread(images[i]);

        // Convert to grayscale
        cvtColor(img, gray, COLOR_BGR2GRAY);

        // Find chessboard corners
        bool patternfound = findChessboardCorners(gray, CHECKERBOARD, imgpoints[i],
            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_FAST_CHECK + CALIB_CB_NORMALIZE_IMAGE);

        if (patternfound) {
            objpoints.push_back(objp);

            // Refine corner positions (angle correction)
            cornerSubPix(gray, imgpoints[i], Size(11, 11), Size(-1, -1), criteria);

            // Draw and display the corners
            drawChessboardCorners(img, CHECKERBOARD, imgpoints[i], patternfound);
            imshow("img", img);
            waitKey(0);
        }
    }

    destroyAllWindows();

    // Get image size
    Size img_size = img.size();

    // Calibrate the camera
    Mat cameraMatrix, distCoeffs;
    vector<Mat> rvecs, tvecs;
    calibrateCamera(objpoints, imgpoints, img_size, cameraMatrix, distCoeffs, rvecs, tvecs);

    return 0;
}