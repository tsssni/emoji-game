#pragma once
#include <QGraphicsPixmapItem>
class EmojiPlayerItem;
class AIItem;
class QPropertyAnimation;
class EmojiScene;
class QTimer;

class AtkPointItem :
    public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    //该Property用于实现击打动画
    Q_PROPERTY(qreal hitFactor
        READ hitFactor
        WRITE setHitFactor
        NOTIFY hitFactorChanged)
    
    friend EmojiPlayerItem;
    friend AIItem;

public:
    AtkPointItem(const QPixmap& pixmap, EmojiPlayerItem* player,
        EmojiScene* parentScene,
        QGraphicsItem* parent = Q_NULLPTR);

    void setHitAnimation();
    QPropertyAnimation* hitAnimation();

    //攻击点实例关于角色的偏移量
    qreal offsetX();
    qreal offsetY();

    void setOffsetX(qreal x);
    void setOffsetY(qreal y);

    EmojiPlayerItem* player();
    QTimer* hitTimer();

public:
    //以下三个函数为击打动画的实现函数
    qreal hitFactor()const;
    void setHitFactor(const qreal& hitFactor);
    void hit();

signals:
    void hitFactorChanged(qreal);

public slots: 
    void checkHit();//时钟暂停时检查是否被击中来触发击打动画

protected:
    EmojiPlayerItem* mPlayer;//记录该击打点所属的玩家与场景
    EmojiScene* mParentScene;

    qreal mOffsetX;
    qreal mOffsetY;

    QPropertyAnimation* mHitAnimation;
    QTimer* mHitTimer;
    int mHitHeight = 100;//击打高度

    qreal mHitFactor;
};