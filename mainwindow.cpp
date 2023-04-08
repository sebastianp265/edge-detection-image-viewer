#include "mainwindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QResource>
#include <QScreen>
#include <opencv2/core.hpp>

#include "opencvutils.h"
#include <QIcon>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hedBeforeThresh(new cv::Mat())
{
    ui->setupUi(this);
    setWindowTitle("OpenCV Edge Detection Image Viewer");
    setWindowIcon(QIcon(":/img/icon.png"));
    scene = new QGraphicsScene(ui->graphicsView);

    imageItem = scene->addPixmap(QPixmap());
    ui->graphicsView->setScene(scene);

    ui->pushButton_Previous->hide();
    ui->pushButton_Next->hide();

    imageItem->setTransformationMode(Qt::SmoothTransformation);
    initializeSlidersAndLabels();

    QResource prototxt(":/model/deploy.prototxt");
    QByteArray prototxtData = prototxt.uncompressedData();

    QResource caffeModel(":/model/hed_pretrained_bsds.caffemodel");
    QByteArray caffeModelData = caffeModel.uncompressedData();

    edgeDetector = cv::dnn::readNetFromCaffe(std::vector<uchar>(prototxtData.begin(), prototxtData.end()),
        std::vector<uchar>(caffeModelData.begin(), caffeModelData.end()));

    resize(QGuiApplication::primaryScreen()->availableSize() * 4 / 5);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::on_actionOpen_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file", QDir::homePath());
    if (filename.isNull()) {
        return;
    }
    QFileInfo fileInfo(filename);
    directoryName = fileInfo.absolutePath();

    loadImagesNames();

    currentFileIndex = 0;
    foreach (QString imageName, imagesNames) {
        if (imageName == fileInfo.fileName()) {
            break;
        }
        currentFileIndex++;
    }
    loadImage();
}

void MainWindow::loadImage()
{
    QImage image(directoryName + "/" + imagesNames.at(currentFileIndex));
    QPixmap pixmap;
    pixmap.convertFromImage(image);
    imageItem->setPixmap(pixmap);

    src.reset(new cv::Mat(OpenCVUtils::QPixmapToMat(pixmap)));

    resetSliders();

    resizeImageView();
}

void MainWindow::loadImagesNames()
{
    QDir directory(directoryName);
    imagesNames = directory.entryList(QStringList() << "*.jpg"
                                                    << "*.png"
                                                    << "*.jpeg"
                                                    << "*.gif",
        QDir::Files);

    if (imagesNames.size() > 1) {
        ui->pushButton_Previous->show();
        ui->pushButton_Next->show();
    } else {
        ui->pushButton_Previous->hide();
        ui->pushButton_Next->hide();
    }
}

void MainWindow::initializeSlidersAndLabels()
{
    sliderLabelPairList.append({ ui->sizeDividor_Slider, ui->sizeDividor_Label });
    sliderLabelPairList.append({ ui->gSlider_KSize, ui->gLabel_KSize });
    sliderLabelPairList.append({ ui->mSlider_KSize, ui->mLabel_KSize });
    sliderLabelPairList.append({ ui->cSlider_Threshold1, ui->cLabel_Threshold1 });
    sliderLabelPairList.append({ ui->cSlider_Threshold2, ui->cLabel_Threshold2 });
    sliderLabelPairList.append({ ui->cSlider_AperatureSize, ui->cLabel_AperatureSize });
    sliderLabelPairList.append({ ui->hedSlider_BlockSize, ui->hedLabel_BlockSize });
    sliderLabelPairList.append({ ui->hedSlider_C, ui->hedLabel_C });

    for (std::pair<QSlider*, QLabel*> sliderLabelPair : sliderLabelPairList) {
        QObject::connect(sliderLabelPair.first, &QSlider::valueChanged, this, &MainWindow::on_slider_value_changed);
    }

    //    QObject::connect(ui->hedSlider_BlockSize, &QSlider::sliderReleased, this, &MainWindow::on_slider_value_changed);
    //    QObject::connect(ui->hedSlider_C, &QSlider::sliderReleased, this, &MainWindow::on_slider_value_changed);

    resetSliders();
}

void MainWindow::resetSliders()
{
    resetingDone = false;
    for (std::pair<QSlider*, QLabel*> sliderLabelPair : sliderLabelPairList) {
        sliderLabelPair.first->setValue(sliderLabelPair.first->minimum());
        sliderLabelPair.second->setText(QString::number(sliderLabelPair.first->value()));
    }

    ui->hedCheckBox_OnOff->setCheckState(Qt::Unchecked);
    ui->cCheckBox_OnOff->setCheckState(Qt::Unchecked);
    setNonHedSlidersDisabled(false);
    shouldHedBeProcessed = false;
    resetingDone = true;
}

void MainWindow::resizeImageView()
{
    ui->graphicsView->fitInView(imageItem, Qt::KeepAspectRatio);

    ui->graphicsView->centerOn(imageItem->boundingRect().center());

    ui->graphicsView->setSceneRect(imageItem->boundingRect());
    imageItem->setPos(0, 0);
    ui->graphicsView->resetZoomInCount();
}

void MainWindow::on_actionOpen_directory_triggered()
{
    directoryName = QFileDialog::getExistingDirectory(this, "Select directory",
        QDir::homePath());
    if (directoryName.isNull()) {
        return;
    }
    loadImagesNames();

    currentFileIndex = 0;
    loadImage();
}

void MainWindow::on_pushButton_Next_clicked()
{
    currentFileIndex++;
    if (currentFileIndex >= imagesNames.size()) {
        currentFileIndex = 0;
    }
    loadImage();
}

void MainWindow::on_pushButton_Previous_clicked()
{
    currentFileIndex--;
    if (currentFileIndex < 0) {
        currentFileIndex = imagesNames.size() - 1;
    }
    loadImage();
}

void MainWindow::on_slider_value_changed()
{
    if (!resetingDone) {
        return;
    }
    correctOddSliderValue(ui->gSlider_KSize);
    correctOddSliderValue(ui->mSlider_KSize);
    correctOddSliderValue(ui->cSlider_AperatureSize);
    correctOddSliderValue(ui->hedSlider_BlockSize);

    updateSliderLabels();

    if (src != nullptr) {

        if (shouldHedBeProcessed || !ui->hedCheckBox_OnOff->isChecked()) {
            dst.reset(new cv::Mat(src->clone()));

            cv::resize(*dst, *dst, cv::Size(dst->size() / ui->sizeDividor_Slider->value()));

            OpenCVUtils::processGaussianBlur(*dst, ui->gSlider_KSize->value());

            OpenCVUtils::processMedianBlur(*dst, ui->mSlider_KSize->value());
            if (ui->cCheckBox_OnOff->isChecked()) {
                OpenCVUtils::processCannyFilter(*dst, ui->cSlider_Threshold1->value(), ui->cSlider_Threshold2->value(), ui->cSlider_AperatureSize->value());
            }

            if (ui->hedCheckBox_OnOff->isChecked()) {
                OpenCVUtils::processHedEdgedetection(*dst, edgeDetector);
                hedBeforeThresh.reset(new cv::Mat(dst->clone()));
                //                cv::threshold(*dst, *dst, 0, 255, cv::THRESH_BINARY);
                cv::adaptiveThreshold(*dst, *dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, ui->hedSlider_BlockSize->value(), ui->hedSlider_C->value());
                shouldHedBeProcessed = false;
            }

        } else {
            cv::threshold(*hedBeforeThresh, *dst, 0, 255, cv::THRESH_BINARY);
            cv::adaptiveThreshold(*hedBeforeThresh, *dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, ui->hedSlider_BlockSize->value(), ui->hedSlider_C->value());
        }

        imageItem->setPixmap(OpenCVUtils::MatToQPixmap(*dst));
        resizeImageView();
    }
}

void MainWindow::setNonHedSlidersDisabled(bool disabled)
{
    for (int i = 0; i < sliderLabelPairList.size() - 2; i++) {
        sliderLabelPairList.at(i).first->setDisabled(disabled);
    }
}

void MainWindow::correctOddSliderValue(QSlider* slider)
{
    int kSizeVal = slider->value();
    if (kSizeVal % 2 == 0) {
        kSizeVal = kSizeVal + 1 > slider->maximum() ? slider->maximum() : kSizeVal + 1;
        slider->setValue(kSizeVal);
    }
}

void MainWindow::updateSliderLabels()
{
    for (std::pair<QSlider*, QLabel*> sliderLabelPair : sliderLabelPairList) {
        sliderLabelPair.second->setText(QString::number(sliderLabelPair.first->value()));
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    resizeImageView();
}

void MainWindow::on_cCheckBox_OnOff_clicked()
{
    ui->hedCheckBox_OnOff->setCheckState(Qt::Unchecked);
    setNonHedSlidersDisabled(false);
    shouldHedBeProcessed = false;
    on_slider_value_changed();
}

void MainWindow::on_cSlider_AperatureSize_valueChanged(int value)
{
    int newMaxVal;
    if (value == 3) {
        newMaxVal = 400;
    } else if (value == 5) {
        newMaxVal = 4000;
    } else {
        newMaxVal = 40000;
    }

    ui->cSlider_Threshold1->setMaximum(newMaxVal);
    ui->cSlider_Threshold2->setMaximum(newMaxVal);

    ui->cSlider_Threshold1->setSingleStep((newMaxVal * 2) / 100);
    ui->cSlider_Threshold1->setSingleStep(2 * ui->cSlider_Threshold1->singleStep());

    ui->cSlider_Threshold2->setSingleStep((newMaxVal * 2) / 100);
    ui->cSlider_Threshold2->setSingleStep(2 * ui->cSlider_Threshold2->singleStep());
}

void MainWindow::on_hedCheckBox_OnOff_clicked()
{
    ui->cCheckBox_OnOff->setCheckState(Qt::Unchecked);
    if (ui->hedCheckBox_OnOff->checkState() == Qt::Checked) {
        shouldHedBeProcessed = true;
        setNonHedSlidersDisabled(true);
    } else if (ui->hedCheckBox_OnOff->checkState() == Qt::Unchecked) {
        shouldHedBeProcessed = false;
        setNonHedSlidersDisabled(false);
    } else {
        assert(false);
    }

    on_slider_value_changed();
}
