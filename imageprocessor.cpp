#include "imageprocessor.h"

#include <QResource>

ImageProcessor::ImageProcessor()
{
    QResource prototxt(":/model/deploy.prototxt");
    QByteArray prototxtData = prototxt.uncompressedData();

    QResource caffeModel(":/model/hed_pretrained_bsds.caffemodel");
    QByteArray caffeModelData = caffeModel.uncompressedData();

    edgeDetector = cv::dnn::readNetFromCaffe(std::vector<uchar>(prototxtData.begin(), prototxtData.end()),
        std::vector<uchar>(caffeModelData.begin(), caffeModelData.end()));
}

void ImageProcessor::setSourceImage(QImage srcImg)
{
    img = std::unique_ptr<cv::Mat>(new cv::Mat(srcImg.height(), srcImg.width(), CV_8UC4, const_cast<uchar*>(srcImg.bits()), srcImg.bytesPerLine()));
    // check if necessary
    cv::cvtColor(*img, *img, cv::COLOR_RGBA2RGB);

    imgCopy = std::unique_ptr<cv::Mat>(new cv::Mat(img->clone()));
}

void ImageProcessor::reset()
{
    img.reset(new cv::Mat(imgCopy->clone()));
}

bool ImageProcessor::isSourceSet()
{
    return !imgCopy->empty();
}

QPixmap ImageProcessor::getPixmap()
{
    cv::cvtColor(*img, *img, cv::COLOR_BGR2RGB);
    return QPixmap::fromImage(QImage(img->data, img->cols, img->rows, img->step, QImage::Format_RGB888));
}

void ImageProcessor::processHED()
{
    cv::Mat blob = cv::dnn::blobFromImage(*img, 1.0, img->size(), cv::Scalar(104.00699, 116.66877, 122.67892));
    // Set the blob as the input to the network and perform a forward pass
    edgeDetector.setInput(blob);
    *img = edgeDetector.forward();
    // Convert the output to a grayscale image
    *img = img->reshape(1, img->size[2]);

    *img = cv::Mat(img->size(), CV_32FC1, img->ptr<float>());

    img->convertTo(*img, CV_8UC1, 255);

    hedProcessedImgBefThresh.reset(new cv::Mat(img->clone()));
}

void ImageProcessor::processGaussianBlur(int kernelSize)
{
    cv::GaussianBlur(*img, *img, cv::Size(kernelSize, kernelSize), 0);
}

void ImageProcessor::processMedianBlur(int kernelSize)
{
    cv::medianBlur(*img, *img, kernelSize);
}

void ImageProcessor::processCannyFilter(int threshold1, int threshold2, int aperatureSize)
{
    cv::Mat tmp;
    cv::Canny(*img, tmp, threshold1, threshold2, aperatureSize);
    *img = tmp;
}

void ImageProcessor::processResize(int sizeDivideFactor)
{
    cv::resize(*img, *img, cv::Size(img->size() / sizeDivideFactor));
}

void ImageProcessor::applyAdaptiveThreshold(int blockSize, int C)
{
    cv::adaptiveThreshold(*hedProcessedImgBefThresh, *img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, C);
}
