#pragma once
#include <QGraphicsScene>
#include <QTimer>
#include <QWidget>
class EmojiPlayerItem;
class AIItem;
class BackgroundItem;
class GroundItem;
class PlatformItem;
class QPropertyAnimation;
class AtkPointItem;

class EmojiScene :
    public QGraphicsScene
{
    Q_OBJECT
    friend class EmojiView;
public:
    EmojiScene(int platformNumber);
    virtual ~EmojiScene();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

public:
        
    //不同情况下的碰撞检测
    qreal checkColliding(EmojiPlayerItem* player);
    bool checkDownConditionColliding(EmojiPlayerItem* player);
    bool checkHitColliding(AtkPointItem* atkPoint);

    //用该值表示下落过程中未碰撞任何物体的情况
    enum {
        CHECK_COLLIDING_FAILURE_HEIGHT = -100
    };

private:
    EmojiPlayerItem* mEmojiPlayer;
    AIItem* mAIPlayer;
    BackgroundItem* mBackground;
    GroundItem* mGround;
    PlatformItem** mPlatform;
    int mPlatformNumber;
};

