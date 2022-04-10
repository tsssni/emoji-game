#pragma once
#include <QGraphicsPixmapItem>

class BackgroundItem :
    public QGraphicsPixmapItem
{
public:
    BackgroundItem(const QPixmap& pixmap,
        QGraphicsItem* parent = Q_NULLPTR);
};

