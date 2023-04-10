#include "opencvutils.h"

#include <opencv2/dnn.hpp>

cv::Mat& OpenCVUtils::QPixmapToMat(const QPixmap& pixmap)
{
    QImage qImage = pixmap.toImage();
    static cv::Mat mat(qImage.height(), qImage.width(), CV_8UC4, const_cast<uchar*>(qImage.bits()), qImage.bytesPerLine());
    // check if necessary
    cv::cvtColor(mat, mat, cv::COLOR_RGBA2RGB);
    return mat;
}

QPixmap OpenCVUtils::MatToQPixmap(const cv::Mat& mat)
{
    cv::Mat temp;
    cv::cvtColor(mat, temp, cv::COLOR_BGR2RGB);
    return QPixmap::fromImage(QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888));
}
