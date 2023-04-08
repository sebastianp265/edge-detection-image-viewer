#include "opencvutils.h"

#include <opencv2/dnn.hpp>

cv::Mat OpenCVUtils::QPixmapToMat(const QPixmap& pixmap)
{
    QImage qImage = pixmap.toImage();
    cv::Mat mat(qImage.height(), qImage.width(), CV_8UC4, const_cast<uchar*>(qImage.bits()), qImage.bytesPerLine());
    cv::cvtColor(mat, mat, cv::COLOR_RGBA2RGB);
    return mat;
}

QPixmap OpenCVUtils::MatToQPixmap(const cv::Mat& mat)
{
    cv::Mat temp;
    cv::cvtColor(mat, temp, cv::COLOR_BGR2RGB);
    return QPixmap::fromImage(QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888));
}

void OpenCVUtils::processHedEdgedetection(cv::Mat& src, cv::dnn::Net& edgeDetector)
{
    cv::Mat blob = cv::dnn::blobFromImage(src, 1.0, src.size(), cv::Scalar(104.00699, 116.66877, 122.67892));
    // Set the blob as the input to the network and perform a forward pass
    edgeDetector.setInput(blob);
    src = edgeDetector.forward();
    // Convert the output to a grayscale image
    src = src.reshape(1, src.size[2]);

    src = cv::Mat(src.size(), CV_32FC1, src.ptr<float>());

    src.convertTo(src, CV_8UC1, 255);
}

void OpenCVUtils::processGaussianBlur(cv::Mat& src, int kernelSize)
{
    cv::GaussianBlur(src, src, cv::Size(kernelSize, kernelSize), 0);
}

void OpenCVUtils::processMedianBlur(cv::Mat& src, int kernelSize)
{
    cv::medianBlur(src, src, kernelSize);
}

void OpenCVUtils::processCannyFilter(cv::Mat& src, int threshold1, int threshold2, int aperatureSize)
{
    cv::Mat tmp;
    cv::Canny(src, tmp, threshold1, threshold2, aperatureSize);
    src = tmp;
}
