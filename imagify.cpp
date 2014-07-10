#include <vector>
#include <cassert>
#include <iostream>
#include <fstream>
#include <algorithm>
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

int main(int argc, char** argv)
{
    FaceRefinery fr;

    for (int i = 1; i < argc; i++) {
        try {
            Mat img = imread(argv[i]);
            std::cout << "Loaded " << argv[i] << "..." << std::endl;
            FaceRefinery::prepareImage(img);

            std::vector<Rect> locations;
            fr.detectFaces(img, locations);

            std::vector<Mat> faces;
            fr.extractFaces(img, locations, faces);

            Mat display = img.clone();
            fr.drawBoxes(locations, display);
            imshow("result", display);
            waitKey();

            std::vector<Mat>::const_iterator fi;
            for (fi = faces.begin(); fi != faces.end(); fi++) {
                std::vector<Rect> eyeLocations;
                fr.detectEyes(*fi, eyeLocations);
                imshow("result", *fi);
                waitKey();
            }
        } catch(...) { }
    }
    return 0;
}

