#pragma once
#include <QGraphicsPixmapItem>

class PlatformItem :
    public QGraphicsPixmapItem
{
public:
    PlatformItem(const QPixmap& pixmap,
        QGraphicsItem* parent = Q_NULLPTR);
    PlatformItem(const PlatformItem&);
    PlatformItem& operator=(const PlatformItem&);
};

