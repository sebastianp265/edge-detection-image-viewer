#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "ui_opencvpanel.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QIcon>
#include <QMessageBox>
#include <QScreen>
#include <opencv2/core.hpp>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("OpenCV Edge Detection Image Viewer");
    setWindowIcon(QIcon(":/img/icon.png"));
    scene = new QGraphicsScene(ui->graphicsView);

    imageItem = scene->addPixmap(QPixmap());
    ui->graphicsView->setScene(scene);

    ui->pushButton_Previous->hide();
    ui->pushButton_Next->hide();

    connect(ui->widget, &OpenCVPanel::parametersChanged, this, &MainWindow::applyProcessing);
    connect(this, &MainWindow::imageChanged, ui->widget, &OpenCVPanel::resetSliders);

    resize(QGuiApplication::primaryScreen()->availableSize() * 4 / 5);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::on_actionOpen_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file", QDir::homePath(), "Images (*.jpg *.png *.jpeg *gif)");
    if (filename.isNull()) {
        return;
    }
    QFileInfo fileInfo(filename);
    currentDir.setPath(fileInfo.absolutePath());

    loadImagesNames();

    currentFileIndex = 0;
    for (const QString& imageName : imageNames) {
        if (imageName == fileInfo.fileName()) {
            break;
        }
        currentFileIndex++;
    }

    loadImage();
}

void MainWindow::on_actionOpen_directory_triggered()
{
    QString currentDirStr = QFileDialog::getExistingDirectory(this, "Select directory",
        QDir::homePath());
    if (currentDirStr.isEmpty()) {
        return;
    }
    currentDir = QDir(currentDirStr);
    loadImagesNames();

    currentFileIndex = 0;

    loadImage();
}

void MainWindow::loadImage()
{
    QImage image(currentDir.absoluteFilePath(imageNames.at(currentFileIndex)));

    imageProcessor.setSourceImage(image);
    QPixmap pixmap;
    pixmap.convertFromImage(image);

    wasHEDprocessed = false;
    imageItem->setPixmap(pixmap);
    refreshImageView();

    emit imageChanged();
}

void MainWindow::loadImagesNames()
{
    imageNames = currentDir.entryList(QStringList() << "*.jpg"
                                                    << "*.png"
                                                    << "*.jpeg"
                                                    << "*.gif",
        QDir::Files);

    if (imageNames.size() > 1) {
        ui->pushButton_Previous->show();
        ui->pushButton_Next->show();
    } else {
        ui->pushButton_Previous->hide();
        ui->pushButton_Next->hide();
    }
}

void MainWindow::on_actionSave_file_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Image", QDir::homePath(), "PNG (*.png)");
    if (filePath.isEmpty()) {
        return;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Save file error", "Couldn't save file in this directory");
        return;
    }
    imageItem->pixmap().save(&file, "PNG");
    file.close();
}

// method assures image fits in the view
void MainWindow::refreshImageView()
{
    ui->graphicsView->fitInView(imageItem, Qt::KeepAspectRatio);

    ui->graphicsView->centerOn(imageItem->boundingRect().center());

    ui->graphicsView->setSceneRect(imageItem->boundingRect());
    imageItem->setPos(0, 0);
    ui->graphicsView->resetZoomInCount();
}

void MainWindow::applyProcessing()
{
    if (imageProcessor.isSourceSet()) {
        Ui::OpenCVPanel* panelUi = ui->widget->getUi();

        if (!panelUi->hedCheckBox_OnOff->isChecked()) {
            wasHEDprocessed = false;
        }
        if (!wasHEDprocessed) {
            imageProcessor.reset();
            if (panelUi->sizeDividor_Slider->value() > 1) {
                imageProcessor.processResize(panelUi->sizeDividor_Slider->value());
            }
            if (panelUi->gSlider_KSize->value() > 1) {
                imageProcessor.processGaussianBlur(panelUi->gSlider_KSize->value());
            }
            if (panelUi->mSlider_KSize->value() > 1) {
                imageProcessor.processMedianBlur(panelUi->mSlider_KSize->value());
            }
            if (panelUi->cCheckBox_OnOff->isChecked()) {
                imageProcessor.processCannyFilter(panelUi->cSlider_Threshold1->value(), panelUi->cSlider_Threshold2->value(), panelUi->cSlider_AperatureSize->value());
            }
            if (panelUi->hedCheckBox_OnOff->isChecked()) {
                imageProcessor.processHED();
                imageProcessor.applyAdaptiveThreshold(panelUi->hedSlider_BlockSize->value(), panelUi->hedSlider_C->value());
                wasHEDprocessed = true;
            }
        } else {
            imageProcessor.applyAdaptiveThreshold(panelUi->hedSlider_BlockSize->value(), panelUi->hedSlider_C->value());
        }
        imageItem->setPixmap(imageProcessor.getPixmap());
        refreshImageView();
    }
}

void MainWindow::on_pushButton_Next_clicked()
{
    currentFileIndex++;
    checkCurrentFileIndex(true);
    loadImage();
}

void MainWindow::on_pushButton_Previous_clicked()
{
    currentFileIndex--;
    checkCurrentFileIndex(false);
    loadImage();
}

void MainWindow::checkCurrentFileIndex(bool next)
{
    if (currentFileIndex < 0) {
        currentFileIndex = imageNames.size() - 1;
    } else if (currentFileIndex >= imageNames.size()) {
        currentFileIndex = 0;
    }

    if (imageNames.size() > 1) {
        ui->pushButton_Previous->show();
        ui->pushButton_Next->show();
    } else {
        ui->pushButton_Previous->hide();
        ui->pushButton_Next->hide();
    }

    if (!imageNames.empty() && !QFile::exists(currentDir.absoluteFilePath(imageNames.at(currentFileIndex)))) {
        imageNames.removeAt(currentFileIndex);
        currentFileIndex = next ? currentFileIndex + 1 : currentFileIndex - 1;
        checkCurrentFileIndex(next);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    refreshImageView();
}
