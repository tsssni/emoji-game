#pragma once
#include <QGraphicsPixmapItem>

class GroundItem :
    public QGraphicsPixmapItem
{
public:
    GroundItem(const QPixmap& pixmap,
        QGraphicsItem* parent = Q_NULLPTR);
};

