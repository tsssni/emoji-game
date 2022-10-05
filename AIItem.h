#pragma once
#include "EmojiPlayerItem.h"
#include <QGraphicsPixmapItem>
class EmojiScene;
class EmojiDecisionTree;
class AtkPointItem;
#include <vector>

class AIItem ://用从EmojiPlayerItem类继承的AIItem类来实现AI
    public EmojiPlayerItem
{
    Q_OBJECT
public:
    AIItem(const QPixmap& pixmap,
        EmojiScene* parentScene, 
        EmojiPlayerItem* player,
        QGraphicsTextItem* label,
        QString name,
        QString picturePath,
        QGraphicsItem* parent = Q_NULLPTR);
    ~AIItem();

    void setPlayer(EmojiPlayerItem* player);
    EmojiPlayerItem* player();

    std::vector<int>& route();

public slots:   
    void traverseDecisionTree();//遍历决策树

protected:
    EmojiDecisionTree* mDecisionTree; 
    EmojiPlayerItem* mPlayer;
    QTimer* mAITimer; //AI独有的Timer,用来执行决策树
    std::vector<int> mRoute;
};

