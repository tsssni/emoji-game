#include "PlatformItem.h"

PlatformItem::PlatformItem
(const QPixmap& pixmap, QGraphicsItem* parent)
	:QGraphicsPixmapItem(pixmap,parent){}

PlatformItem::PlatformItem(const PlatformItem&)
{
}

