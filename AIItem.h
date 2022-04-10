#pragma once
#include "EmojiPlayerItem.h"
#include <QGraphicsPixmapItem>
class EmojiScene;
class EmojiDecisionTree;
class AtkPointItem;

class AIItem ://用从EmojiPlayerItem类继承的AIItem类来实现AI
    public EmojiPlayerItem
{
    Q_OBJECT
public:
    AIItem(const QPixmap& pixmap,
        EmojiScene* parentScene, 
        QGraphicsItem* parent = Q_NULLPTR);
    ~AIItem();

public slots:   
    void traverseDecisionTree();//遍历决策树

protected:
    EmojiDecisionTree* mDecisionTree;  
    QTimer* mAITimer; //AI独有的Timer,用来执行决策树
};

