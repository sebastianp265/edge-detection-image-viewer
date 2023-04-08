#ifndef OPENCVUTILS_H
#define OPENCVUTILS_H

#include <QPixmap>
#include <opencv2/opencv.hpp>

namespace OpenCVUtils {
cv::Mat QPixmapToMat(const QPixmap& pixmap);
QPixmap MatToQPixmap(const cv::Mat& mat);

void processHedEdgedetection(cv::Mat& src, cv::dnn::Net& edgeDetector);
void processGaussianBlur(cv::Mat& src, int kernelSize);
void processMedianBlur(cv::Mat& src, int kernelSize);
void processCannyFilter(cv::Mat& src, int threshold1, int threshold2, int aperatureSize);

}

#endif // OPENCVUTILS_H
