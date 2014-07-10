#ifndef _FACE_REFINERY_HPP_
#define _FACE_REFINERY_HPP_

#include <vector>
#include <string>
#include <utility>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;

struct EyeRegion {
    double EYE_SX;
    double EYE_SY;
    double EYE_SW;
    double EYE_SH;

    EyeRegion(double EYE_SX, double EYE_SY, double EYE_SW, double EYE_SH)
        : EYE_SX(EYE_SX), EYE_SY(EYE_SY), EYE_SW(EYE_SW), EYE_SH(EYE_SH) {};
    void getTopHalves(const Mat& face, Mat& topLeft, Mat& topRight);
};

class FaceRefinery {
public:
    FaceRefinery();
    static void prepareImage(Mat& img);
    static void drawBoxes(const std::vector<Rect>& locations, Mat& img);
    void detectEyes(const Mat& face, std::vector<Rect>& locations);
    void extractFaces(const Mat& img, const std::vector<Rect>& locations,
            std::vector<Mat>& faces);
    void detectFaces(const Mat& img, std::vector<Rect>& locations);

private:
    static void loadCascadeClassifier(const std::string& path,
            CascadeClassifier& detector);
};

#endif

