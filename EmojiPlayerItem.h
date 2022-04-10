#pragma once
#include <QGraphicsPixmapItem>
class QPropertyAnimation;
class EmojiScene;
class QPropertyAnimation;
class QTimer;
class AtkPointItem;

class EmojiPlayerItem :
    public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    //该Property用于执行跳跃动画
    Q_PROPERTY(qreal jumpFactor
        READ jumpFactor
        WRITE setJumpFactor
        NOTIFY jumpFactorChanged)
    //该Property用于执行下落动画
    Q_PROPERTY(qreal downFactor
        READ downFactor
        WRITE setDownFactor
        NOTIFY downFactorChanged)
    //该Property用于执行被击打动画
    Q_PROPERTY(qreal beHitFactor
        READ beHitFactor
        WRITE setBeHitFactor
        NOTIFY beHitFactorChanged)
public:
    EmojiPlayerItem(const QPixmap& pixmap,
        EmojiScene* parentScene,
        QGraphicsItem* parent = Q_NULLPTR);

    virtual void setJumpAnimation();
    virtual void setDownAnimation();
    virtual void setBeHitAnimation();

    QPropertyAnimation* jumpAnimation();
    QPropertyAnimation* downAnimation();
    QPropertyAnimation* beHitAnimation();

    QTimer* playerTimer();
    void checkTimer();//检查时钟确定平移方向

    int direction()const;
    void setDirection(int direction);
    int lastDirction();//返回当前朝向

    virtual bool checkOutsideOfGameView();//检查是否出界从而需要重生
    virtual void restoreLife();
    virtual int life()const;
    virtual void setLife();
    
    //击打系数,系数越高被打的越远
    virtual int coefficient()const;
    virtual void restorecoefficient();
    virtual void addcoefficient();

    //设置复活点
    void setStartPosX(qreal x);
    void setStartPosY(qreal y);
    qreal startPosX()const;
    qreal startPosY()const;

    //用于平移的函数
    void addHorizontalInput(int input);
    void setHorizontalInput(int input);

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);

    virtual void jump();
    virtual void beHit();

    void setBeHitDirection(int value);
    
    AtkPointItem* point();

public:
    //通过factor来控制动画执行进度
    qreal jumpFactor()const;
    void setJumpFactor(const qreal &jumpFactor);

    qreal downFactor()const;
    void setDownFactor(const qreal& downFactor);

    qreal beHitFactor()const;
    void setBeHitFactor(const qreal& beHitFactor);

signals:
    void jumpFactorChanged(qreal);
    void downFactorChanged(qreal);
    void beHitFactorChanged(qreal);

public slots:
    void moveHorizontalEmojiPlayer();
    void moveDownEmojiPlayer();

protected:
    int mLastDirection;//只可能是1或者-1的朝向
    int mDirection;//角色的朝向

    int mLife;//角色生命
    int mCoefficient;//影响系数
    int mHorizontalInput;//决定朝向

    qreal mJumpStartLevel;
    qreal mLastJumpValue;
    qreal mJumpHeight = 180;

    qreal mDownStartLevel;
    qreal mDownHeight = 180;

    qreal mBeHitStartLevel;
    qreal mLastBeHitValue;
    qreal mBeHitStartPos;
    qreal mBeHitHeight = 180;
    int mBeHitDirection;//决定被击飞的方向

    qreal mStartPosX;
    qreal mStartPosY;//出生点坐标

    EmojiScene* mParentScene;
    AtkPointItem* mPoint;
    QTimer* mPlayerTimer;

    QPropertyAnimation* mJumpAnimation;
    QPropertyAnimation* mDownAnimation;
    QPropertyAnimation* mBeHitAnimation;

    qreal mJumpFactor;
    qreal mDownFactor;
    qreal mBeHitFactor;

    int mWorldSpeed = 5;
    int mFilter = 0;//若死亡时没有松开按键,keyReleaseEvent中会造成各种问题,
    //所以需要过滤掉死亡之前的按键信号

    //这两个值用于解决复活时同时按住a和d所造成的问题
    bool mAPressed = 0;
    bool mDPressed = 0;
};

