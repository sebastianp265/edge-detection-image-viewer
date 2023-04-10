#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class ImageView : public QGraphicsView {
    Q_OBJECT

public:
    ImageView(QWidget* parent = nullptr);
    void zoomIn();
    void zoomOut();
    void resetZoomInCount();

protected:
    virtual void wheelEvent(QWheelEvent* event);

private:
    const double SCALE_FACTOR = 1.15;
    int zoomInCount = 0;
    const int MAX_ZOOM_IN_COUNT = 30;
};

#endif // IMAGEVIEW_H
