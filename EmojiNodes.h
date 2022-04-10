#pragma once
#include "Node.h"
class AIItem;
class EmojiDecisionTree;

class RootSel :public SelectorNode//根部选择节点
{
public:
	RootSel(Node* root, EmojiDecisionTree* tree, AIItem* ai)
		:SelectorNode(root), mTree(tree), mAI(ai) {}
	AIItem* AI()
	{
		return mAI;
	}
	EmojiDecisionTree* tree()
	{
		return mTree;
	}
protected:
	AIItem* mAI;
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

class CondWalk :public ConditionNode//判断是否应该水平移动
{
public:
	CondWalk(Node* root) :ConditionNode(root) {}
	bool exec()
	{
		return IsWalkExecutable();
	}
private:
	bool IsWalkExecutable();
};

class CondJump : public ConditionNode//判断是否应该向上移动
{
public:
	CondJump(Node* root) :ConditionNode(root) {}
	bool exec()
	{
		return IsJumpExecutable();
	}
private:
	bool IsJumpExecutable();
};

class CondDown :public ConditionNode//判断是否应该向下移动
{
public:
	CondDown(Node* root) :ConditionNode(root) {}
	bool exec()
	{
		return IsDownExecutable();
	}
private:
	bool IsDownExecutable();
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

class BehaviorDown :public BehaviorNode//执行向下移动
{
public:
	BehaviorDown(Node* root) :BehaviorNode(root) {}
	bool exec();
};


