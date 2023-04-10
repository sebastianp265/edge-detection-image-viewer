#include "opencvpanel.h"
#include "ui_opencvpanel.h"

OpenCVPanel::OpenCVPanel(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::OpenCVPanel)
{
    ui->setupUi(this);

    sliderLabelPairList = { { ui->sizeDividor_Slider, ui->sizeDividor_Label },
        { ui->gSlider_KSize, ui->gLabel_KSize },
        { ui->sizeDividor_Slider, ui->sizeDividor_Label },
        { ui->gSlider_KSize, ui->gLabel_KSize },
        { ui->mSlider_KSize, ui->mLabel_KSize },
        { ui->cSlider_Threshold1, ui->cLabel_Threshold1 },
        { ui->cSlider_Threshold2, ui->cLabel_Threshold2 },
        { ui->cSlider_AperatureSize, ui->cLabel_AperatureSize },
        { ui->hedSlider_BlockSize, ui->hedLabel_BlockSize },
        { ui->hedSlider_C, ui->hedLabel_C } };
    initializeSliders();
}

OpenCVPanel::~OpenCVPanel()
{
    delete ui;
}

Ui::OpenCVPanel* OpenCVPanel::getUi()
{
    return ui;
}

void OpenCVPanel::initializeSliders()
{
    for (auto [slider, _] : sliderLabelPairList) {
        QObject::connect(slider, &QSlider::valueChanged, this, &OpenCVPanel::on_slider_value_changed);
    }

    resetSliders();
}

void OpenCVPanel::resetSliders()
{
    for (auto [slider, _] : sliderLabelPairList) {
        slider->blockSignals(true);
        slider->setValue(slider->minimum());
        slider->blockSignals(false);
    }
    updateSliderLabels();

    ui->hedCheckBox_OnOff->setCheckState(Qt::Unchecked);
    ui->cCheckBox_OnOff->setCheckState(Qt::Unchecked);
    setNonHEDSlidersDisabled(false);
}

void OpenCVPanel::updateSliderLabels()
{
    for (auto [slider, label] : sliderLabelPairList) {
        label->setText(QString::number(slider->value()));
    }
}

void OpenCVPanel::on_cSlider_AperatureSize_valueChanged(int value)
{
    int newMaxVal;
    if (value == 3) {
        newMaxVal = 400;
    } else if (value == 5) {
        newMaxVal = 4000;
    } else { // value == 7
        newMaxVal = 40000;
    }

    ui->cSlider_Threshold1->setMaximum(newMaxVal);
    ui->cSlider_Threshold2->setMaximum(newMaxVal);

    ui->cSlider_Threshold1->setSingleStep((newMaxVal * 2) / 100);
    ui->cSlider_Threshold1->setPageStep(2 * ui->cSlider_Threshold1->singleStep());

    ui->cSlider_Threshold2->setSingleStep((newMaxVal * 2) / 100);
    ui->cSlider_Threshold2->setPageStep(2 * ui->cSlider_Threshold2->singleStep());
}

void OpenCVPanel::on_cCheckBox_OnOff_clicked()
{
    ui->hedCheckBox_OnOff->setCheckState(Qt::Unchecked);
    setNonHEDSlidersDisabled(false);

    emit parametersChanged();
}

void OpenCVPanel::on_hedCheckBox_OnOff_clicked()
{
    ui->cCheckBox_OnOff->setCheckState(Qt::Unchecked);
    if (ui->hedCheckBox_OnOff->checkState() == Qt::Checked) {
        setNonHEDSlidersDisabled(true);
    } else if (ui->hedCheckBox_OnOff->checkState() == Qt::Unchecked) {
        setNonHEDSlidersDisabled(false);
    } else {
        assert(false);
    }

    emit parametersChanged();
}

void OpenCVPanel::setNonHEDSlidersDisabled(bool disabled)
{
    // last 2 {slider, label} pairs are for hed
    for (int i = 0; i < sliderLabelPairList.size() - 2; i++) {
        sliderLabelPairList.at(i).first->setDisabled(disabled);
    }
}

void OpenCVPanel::on_slider_value_changed()
{
    ensureOddSliderValue(ui->gSlider_KSize);
    ensureOddSliderValue(ui->mSlider_KSize);
    ensureOddSliderValue(ui->cSlider_AperatureSize);
    ensureOddSliderValue(ui->hedSlider_BlockSize);

    updateSliderLabels();

    emit parametersChanged();
}

// some OpenCV algorithms arguments values have to be odd
void OpenCVPanel::ensureOddSliderValue(QSlider* slider)
{
    int kSizeVal = slider->value();
    if (kSizeVal % 2 == 0) {
        kSizeVal = kSizeVal + 1 > slider->maximum() ? slider->maximum() : kSizeVal + 1;
        slider->setValue(kSizeVal);
    }
}
