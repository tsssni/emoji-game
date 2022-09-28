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
    
    //该Property用于执行行走动画
    Q_PROPERTY(qreal walkFactor
        READ walkFactor
        WRITE setWalkFactor
        NOTIFY walkFactorChanged)
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

    virtual void setWalkAnimation();
    virtual void setJumpAnimation();
    virtual void setDownAnimation();
    virtual void setBeHitAnimation();

    QPropertyAnimation* walkAnimation();
    QPropertyAnimation* jumpAnimation();
    QPropertyAnimation* downAnimation();
    QPropertyAnimation* beHitAnimation();

    void stopAnimations();

    EmojiScene* scene();

    QTimer* playerTimer();
    void checkTimer();//检查时钟确定平移方向

    int direction()const;
    void setDirection(int direction);
    int lastDirction();//返回当前朝向

    //virtual bool checkOutsideOfGameView();//检查是否出界从而需要重生
    virtual void restoreLife();
    virtual int life()const;
    virtual void setLife();
    
    //击打系数,系数越高被打的越远
    virtual int coefficient()const;
    virtual void restoreCoefficient();
    virtual void addCoefficient();

    //设置复活点
    void setStartPosX(qreal x);
    void setStartPosY(qreal y);
    qreal startPosX()const;
    qreal startPosY()const;

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);

    virtual void jump();
    virtual void beHit();

    void setBeHitDirection(int value);
    
    AtkPointItem* point();

    void setCurrPlatform(int index);
    int platform();

public:
    //通过factor来控制动画执行进度
    qreal walkFactor()const;
    void setWalkFactor(const qreal& walkFactor);

    qreal jumpFactor()const;
    void setJumpFactor(const qreal &jumpFactor);

    qreal downFactor()const;
    void setDownFactor(const qreal& downFactor);

    qreal beHitFactor()const;
    void setBeHitFactor(const qreal& beHitFactor);

signals:
    void walkFactorChanged(qreal);
    void jumpFactorChanged(qreal);
    void downFactorChanged(qreal);
    void beHitFactorChanged(qreal);
    void directionChanged(int);
    void jumpStart();
    void hitStart();

public slots:
    void addHorizontalInput(int input); //通过修改mDirection值来决定行走方向
    void moveHorizontalEmojiPlayer();
    void moveUpEmojiPlayer();
    void hitAIPlayer();
    void checkOutsideOfGameView();
    void checkMoveCollision();

protected:
    EmojiScene* mScene;

    int mLastDirection;//只可能是1或者-1的朝向
    int mDirection;//角色的朝向
    int mCurrPlatform;

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

    QPropertyAnimation* mWalkAnimation;
    QPropertyAnimation* mJumpAnimation;
    QPropertyAnimation* mDownAnimation;
    QPropertyAnimation* mBeHitAnimation;

    qreal mWalkFactor;
    qreal mJumpFactor;
    qreal mDownFactor;
    qreal mBeHitFactor;

    qreal mWorldSpeed = 1;
};

