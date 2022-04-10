#include "EmojiDecisionTree.h"
#include "EmojiNodes.h"
#include "AIItem.h"
#include <QObject>

EmojiDecisionTree::EmojiDecisionTree(AIItem* ai) :mAI(ai)
{

    //根据文件中附录的决策树(DecisionTree.dot)所创建的决策树
    mRoot = new RootSel(nullptr, this, mAI);

    //选择是攻击还是追逐的选择节点
    SelectorNode* selHandle = new SelectorNode(mRoot);
    mRoot->addChild(selHandle);

    //判断距离远近
    SequenceNode* seqNear = new SequenceNode(mRoot);
    SequenceNode* seqFar = new SequenceNode(mRoot);
    selHandle->addChild(seqNear);
    selHandle->addChild(seqFar);

    //判近
    CondNear* condNearEnough = new CondNear(mRoot);
    BehaviorHit* behaviorHit = new BehaviorHit(mRoot);
    seqNear->addChild(condNearEnough);
    seqNear->addChild(behaviorHit);

    //判远
    InvertNode* invertCondNear = new InvertNode(mRoot);
    CondNear* condTooFar = new CondNear(mRoot);
    invertCondNear->addChild(condTooFar);
    seqFar->addChild(invertCondNear);

    //追逐路线选择节点
    SelectorNode* selRoute = new SelectorNode(mRoot);
    SequenceNode* seqWalk = new SequenceNode(mRoot);
    SequenceNode* seqJump = new SequenceNode(mRoot);
    SequenceNode* seqDown = new SequenceNode(mRoot);
    seqFar->addChild(selRoute);
    selRoute->addChild(seqWalk);
    selRoute->addChild(seqJump);
    selRoute->addChild(seqDown);

    //判断水平移动
    CondWalk* condWalk = new CondWalk(mRoot);
    BehaviorWalk* behaviorWalk = new BehaviorWalk(mRoot);
    seqWalk->addChild(condWalk);
    seqWalk->addChild(behaviorWalk);

    //判断向上移动
    CondJump* condJump = new CondJump(mRoot);
    BehaviorJump* behaviorJump = new BehaviorJump(mRoot);
    seqJump->addChild(condJump);
    seqJump->addChild(behaviorJump);

    //判断向下移动
    CondDown* condDown = new CondDown(mRoot);
    BehaviorDown* behaviorDown = new BehaviorDown(mRoot);
    seqDown->addChild(condDown);
    seqDown->addChild(behaviorDown);
}

EmojiDecisionTree::~EmojiDecisionTree()
{
    delete mRoot;
}

RootSel* EmojiDecisionTree::root()
{
    return mRoot;
}