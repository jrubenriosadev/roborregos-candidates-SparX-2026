// Test usando camara de laptop :P

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

int main() {
    cv::VideoCapture cap(0);

    if(!cap.isOpened()) {
        std::cerr << "No se pudo abrir la camara" << std::endl;
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    // para full hd
    // cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    // cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

    cv::namedWindow("detect", cv::WINDOW_NORMAL);
    cv::resizeWindow("detect ArUco", 1280, 720);

    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
    cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);

    cv::Mat frame;

    while(true) {
        cap >> frame;
        if(frame.empty()) break;

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners, rejectedCandidates;

        detector.detectMarkers(frame, corners, ids, rejectedCandidates);

        if (!ids.empty()) {
            cv::aruco::drawDetectedMarkers(frame, corners, ids);
        }

        cv::imshow("detect", frame);

        char c = (char)cv::waitKey(30);
        if(c == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}