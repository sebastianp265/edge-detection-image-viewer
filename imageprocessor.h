#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QPixmap>
#include <memory>
#include <opencv2/opencv.hpp>

class ImageProcessor {
public:
    ImageProcessor();
    QPixmap getPixmap();
    void setSourceImage(QImage srcImg);
    void reset();
    bool isSourceSet();

    void processResize(int sizeDivideFactor);
    void processGaussianBlur(int kernelSize);
    void processMedianBlur(int kernelSize);
    void processCannyFilter(int threshold1, int threshold2, int aperatureSize);
    void processHED();
    void applyAdaptiveThreshold(int blockSize, int C);

private:
    cv::dnn::Net edgeDetector;
    std::unique_ptr<cv::Mat> img;
    std::unique_ptr<cv::Mat> imgCopy;
    std::unique_ptr<cv::Mat> hedProcessedImgBefThresh;
};

#endif // IMAGEPROCESSOR_H
