#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(0);

    if(!cap.isOpened()) {
        std::cerr << "No se pudo abrir la camara :[" << std::endl;
        return -1;
    }

    std::cout << "ESC para salir" << std::endl;

    cv::namedWindow("Camara", cv::WINDOW_AUTOSIZE);

    cv::Mat frame;

    while(true) {
        cap >> frame;
        if(frame.empty()) {
            std::cerr << "no frame" << std::endl;
            break;
        }

        cv::imshow("Cam", frame);

        char c = (char)cv::waitKey(30);
        if(c == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}