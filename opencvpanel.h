#ifndef OPENCVPANEL_H
#define OPENCVPANEL_H

#include <QLabel>
#include <QSlider>
#include <QWidget>

namespace Ui {
class OpenCVPanel;
}

class OpenCVPanel : public QWidget {
    Q_OBJECT

signals:
    void parametersChanged();

public:
    explicit OpenCVPanel(QWidget* parent = nullptr);
    ~OpenCVPanel();
    Ui::OpenCVPanel* getUi();
    void resetSliders();

private slots:
    void on_slider_value_changed();
    void on_cCheckBox_OnOff_clicked();
    void on_cSlider_AperatureSize_valueChanged(int value);
    void on_hedCheckBox_OnOff_clicked();

private:
    Ui::OpenCVPanel* ui;
    QList<QPair<QSlider*, QLabel*>> sliderLabelPairList;

    void initializeSliders();
    void ensureOddSliderValue(QSlider* slider);
    void setNonHEDSlidersDisabled(bool block);
    void updateSliderLabels();
    void updateLabels();
};

#endif // OPENCVPANEL_H
