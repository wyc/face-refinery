#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "face_refinery.hpp"


using namespace cv;

void EyeRegion::getTopHalves(const Mat& face, Mat& topLeft, Mat& topRight)
{
    int leftX = round(face.cols * EYE_SX);
    int topY = round(face.rows * EYE_SY);
    int widthX = round(face.cols * EYE_SW);
    int heightY = round(face.rows * EYE_SH);
    int rightX = round(face.cols * (1.0-EYE_SX-EYE_SW));

    face(Rect(leftX, topY, widthX, heightY)).copyTo(topLeft);
    face(Rect(rightX, topY, widthX, heightY)).copyTo(topRight);
}

FaceRefinery::FaceRefinery()
{
}

void FaceRefinery::prepareImage(Mat& img)
{
    if (img.channels() == 3) {
        cvtColor(img, img, CV_BGR2GRAY);
    } else if (img.channels() == 4) {
        cvtColor(img, img, CV_BGRA2GRAY);
    }
    equalizeHist(img, img);
}

void FaceRefinery::detectEyes(const Mat& face, std::vector<Rect>& locations)
{

    int flags = CASCADE_FIND_BIGGEST_OBJECT;// | CASCADE_DO_ROUGH_SEARCH;
    Size minFeatureSize = Size(20, 20);
    float searchScaleFactor = 1.1f;
    int minNeighbors = 4;
    Mat topLeft, topRight;

    std::vector<Rect> objects;
    CascadeClassifier detector;

    EyeRegion mcsRegion(0.10, 0.19, 0.40, 0.36);
    EyeRegion twoSplitsRegion(0.12, 0.17, 0.37, 0.36);

    /* mcs left */
    loadCascadeClassifier("/usr/share/OpenCV/haarcascades/haarcascade_mcs_lefteye.xml", detector);
    mcsRegion.getTopHalves(face, topLeft, topRight);
    prepareImage(topLeft);
    detector.detectMultiScale(topLeft, objects, searchScaleFactor,
                              minNeighbors, flags, minFeatureSize);
    if (objects.size() > 0) {
        locations.push_back(objects.at(0));
        drawBoxes(objects, topLeft);
        imshow("result", topLeft);
        waitKey();
        objects.clear();
        goto rightEyeDetectionStage;
    }
    objects.clear();

    /* 2splits left */
    loadCascadeClassifier("/usr/share/OpenCV/haarcascades/haarcascade_lefteye_2splits.xml", detector);
    mcsRegion.getTopHalves(face, topLeft, topRight);
    prepareImage(topLeft);
    detector.detectMultiScale(topLeft, objects, searchScaleFactor,
                              minNeighbors, flags, minFeatureSize);
    if (objects.size() > 0) {
        locations.push_back(objects.at(0));
        drawBoxes(objects, topLeft);
        imshow("result", topLeft);
        waitKey();
        objects.clear();
        goto rightEyeDetectionStage;
    }

rightEyeDetectionStage:
    /* mcs right */
    loadCascadeClassifier("/usr/share/OpenCV/haarcascades/haarcascade_mcs_righteye.xml", detector);
    mcsRegion.getTopHalves(face, topLeft, topRight);
    prepareImage(topRight);
    detector.detectMultiScale(topRight, objects, searchScaleFactor,
                              minNeighbors, flags, minFeatureSize);
    if (objects.size() > 0) {
        locations.push_back(objects.at(0));
        drawBoxes(objects, topRight);
        imshow("result", topRight);
        waitKey();
        objects.clear();
        goto finishedEyeStage;
    }
    objects.clear();

    /* 2splits right */
    loadCascadeClassifier("/usr/share/OpenCV/haarcascades/haarcascade_righteye_2splits.xml", detector);
    mcsRegion.getTopHalves(face, topLeft, topRight);
    prepareImage(topRight);
    detector.detectMultiScale(topRight, objects, searchScaleFactor,
                              minNeighbors, flags, minFeatureSize);
    if (objects.size() > 0) {
        locations.push_back(objects.at(0));
        drawBoxes(objects, topRight);
        imshow("result", topRight);
        waitKey();
        objects.clear();
        goto finishedEyeStage;
    }

finishedEyeStage:
    return;
}

void FaceRefinery::detectFaces(const Mat& img, std::vector<Rect>& locations)
{
    CascadeClassifier detector;
    std::string path("/usr/share/OpenCV/haarcascades/haarcascade_frontalface_alt2.xml");
    loadCascadeClassifier(path, detector);

    int flags = CASCADE_SCALE_IMAGE;
    Size minFeatureSize(20, 20);
    float searchScaleFactor = 1.1f;
    int minNeighbors = 4;

    detector.detectMultiScale(img, locations, searchScaleFactor, minNeighbors,
                              flags, minFeatureSize);
}

void FaceRefinery::extractFaces(const Mat& img,
        const std::vector<Rect>& locations, std::vector<Mat>& faces)
{
    std::vector<Rect>::const_iterator li;
    for (li = locations.begin(); li != locations.end(); li++) {
        faces.push_back(img(*li));
    }
}

void FaceRefinery::drawBoxes(const std::vector<Rect>& locations, Mat& img)
{
    std::vector<Rect>::const_iterator li;
    for (li = locations.begin(); li != locations.end(); li++) {
        rectangle(img, *li, Scalar(255, 0, 0));
    }
}


void FaceRefinery::loadCascadeClassifier(const std::string& file_path,
        CascadeClassifier& detector)
{
    detector.load(file_path);
    if (detector.empty()) {
        std::cerr << "ERROR: Couldn't load Face Detector (\"";
        std::cerr << file_path << "\")!" << std::endl;
        throw;
    }
}

