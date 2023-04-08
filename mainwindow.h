#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDirIterator>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <Qdir>
#include <opencv2/opencv.hpp>
#include <utility>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_file_triggered();

    void on_actionOpen_directory_triggered();

    void on_pushButton_Next_clicked();

    void on_pushButton_Previous_clicked();

    void on_slider_value_changed();

    void on_cCheckBox_OnOff_clicked();

    void on_cSlider_AperatureSize_valueChanged(int value);

    void on_hedCheckBox_OnOff_clicked();

protected:
    virtual void resizeEvent(QResizeEvent* event);

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* imageItem;

    cv::dnn::Net edgeDetector;
    std::unique_ptr<cv::Mat> src;
    std::unique_ptr<cv::Mat> dst;

    qsizetype currentFileIndex;
    QString directoryName;
    QStringList imagesNames;

    QList<std::pair<QSlider*, QLabel*>> sliderLabelPairList;

    bool shouldHedBeProcessed = false;
    bool resetingDone = false;
    std::unique_ptr<cv::Mat> hedBeforeThresh;

    void loadImage();
    void loadImagesNames();
    void resizeImageView();
    void initializeSlidersAndLabels();
    void updateSliderLabels();

    void correctOddSliderValue(QSlider* slider);
    void resetSliders();
    void setNonHedSlidersDisabled(bool block);
};
#endif // MAINWINDOW_H
