#include "EmojiMap.h"

bool EmojiMap::setPlatform(qint16 x, qint16 y, QString platformTexPath)
{
    if (!checkPlatformOverlap(x, y))
    {
        return false;
    }
    
    mPlatforms.emplace_back(QPixmap(platformTexPath));
    return true;
}

void EmojiMap::buildGraph()
{
}

const vector<PlatformItem>& EmojiMap::search(QPoint aiPos, QPoint playerPos) const
{
    return mPlatforms;
}

const vector<PlatformItem>& EmojiMap::getPlatforms()
{
    return mPlatforms;
}

bool EmojiMap::checkPlatformOverlap(qint16 x, qint16 y)
{
    for (auto& platform : mPlatforms)
    {
        QRectF platformRect = platform.boundingRect();
        QRectF currRect(x, y, platformRect.width(), platformRect.height());

        if (currRect.intersects(platformRect))
        {
            return false;
        }
    }

    return true;
}
