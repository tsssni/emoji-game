#pragma once
#include <QObject>
class RootSel;
class AIItem;

class EmojiDecisionTree
{
public:
    EmojiDecisionTree(AIItem* ai);
    ~EmojiDecisionTree();

    RootSel* root();//取得根节点的函数

private:
    RootSel* mRoot;
    AIItem* mAI;
};
