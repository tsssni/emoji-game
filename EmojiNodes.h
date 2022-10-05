#pragma once
#include "Node.h"
class AIItem;
class EmojiPlayerItem;
class EmojiScene;
class EmojiDecisionTree;

class RootSel :public SelectorNode//根部选择节点
{
public:
	RootSel(Node* root, EmojiDecisionTree* tree, AIItem* ai);

	AIItem* AI()
	{
		return mAI;
	}

	EmojiPlayerItem* player()
	{
		return mPlayer;
	}

	EmojiScene* scene()
	{
		return mScene;
	}

	EmojiDecisionTree* tree()
	{
		return mTree;
	}
protected:
	AIItem* mAI;
	EmojiPlayerItem* mPlayer;
	EmojiScene* mScene;
	EmojiDecisionTree* mTree;
};

class CondNear :public ConditionNode//判断距离是否足够近以触发攻击
{
public:
	CondNear(Node* root) :ConditionNode(root) {}
	bool exec()
	{
		return isNear();
	}
private:
	bool isNear();
};

class CondRoute : public ConditionNode
{
public:
	CondRoute(Node* root):ConditionNode(root){}
	bool exec()
	{
		return isRouteExist();
	}
private:
	bool isRouteExist();
};

class CondWalk :public ConditionNode//判断是否应该水平移动
{
public:
	CondWalk(Node* root) :ConditionNode(root) {}
	bool exec()
	{
		return isWalkExecutable();
	}
private:
	bool isWalkExecutable();
};

class CondJump : public ConditionNode//判断是否应该向上移动
{
public:
	CondJump(Node* root) :ConditionNode(root) {}
	bool exec()
	{
		return isJumpExecutable();
	}
private:
	bool isJumpExecutable();
};

class BehaviorHit :public BehaviorNode//执行攻击
{
public:
	BehaviorHit(Node* root) :BehaviorNode(root) {}
	bool exec();
};

class BehaviorWalk :public BehaviorNode//执行水平移动
{
public:
	BehaviorWalk(Node* root) :BehaviorNode(root) {}
	bool exec();
};

class BehaviorJump :public BehaviorNode//执行向上移动
{
public:
	BehaviorJump(Node* root) :BehaviorNode(root) {}
	bool exec();
};

