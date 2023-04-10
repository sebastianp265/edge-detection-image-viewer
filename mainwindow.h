#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imageprocessor.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMainWindow>
#include <Qdir>

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
signals:
    void imageChanged();

private slots:
    void on_actionOpen_file_triggered();
    void on_actionOpen_directory_triggered();
    void on_pushButton_Next_clicked();
    void on_pushButton_Previous_clicked();
    void on_actionSave_file_triggered();

protected:
    virtual void resizeEvent(QResizeEvent* event);

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* imageItem;

    qsizetype currentFileIndex;
    QDir currentDir;
    QStringList imageNames;

    ImageProcessor imageProcessor;
    bool wasHEDprocessed;

    void checkCurrentFileIndex(bool next);

    void loadImage();
    void loadImagesNames();
    void refreshImageView();
    void applyProcessing();
};
#endif // MAINWINDOW_H
