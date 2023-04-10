#include "imageview.h"

ImageView::ImageView(QWidget* parent)
    : QGraphicsView(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ImageView::zoomIn()
{
    if (zoomInCount < MAX_ZOOM_IN_COUNT) {
        scale(SCALE_FACTOR, SCALE_FACTOR);
        zoomInCount++;
    }
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ImageView::zoomOut()
{
    if (zoomInCount > 0) {
        scale(1 / SCALE_FACTOR, 1 / SCALE_FACTOR);
        zoomInCount--;
    }
    if (zoomInCount == 0) {
        setDragMode(QGraphicsView::NoDrag);
    }
}

void ImageView::resetZoomInCount() { zoomInCount = 0; }

void ImageView::wheelEvent(QWheelEvent* event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}
