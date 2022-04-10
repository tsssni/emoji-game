#pragma once
#include <vector>
#include <functional>

class Node
{
public:
	Node(Node* r) :mRoot(r) {}
	virtual bool exec() = 0;
protected:
	Node* mRoot;//根节点会存储决策树所属的AI与场景等重要信息
	//故每个节点都有一个指向根节点的指针
};

class NonLeafNode :public Node//非叶节点,会存储子节点
{
public:
	NonLeafNode(Node* root) :Node(root) {}
	~NonLeafNode()
	{
		for (auto child : childNodes)
		{
			delete child;
		}
	}
	bool exec() = 0;
	void addChild(Node* child)
	{
		childNodes.push_back(child);
	}
protected:
	std::vector<Node*> childNodes;
};

class SelectorNode :public NonLeafNode//选择节点,某一子节点返回为真则停止执行
{
public:
	SelectorNode(Node* root) :NonLeafNode(root) {}
	bool exec()
	{
		for (auto child : childNodes)
		{
			if (child->exec())
			{
				return true;
			}
		}
		return false;
	}
};

class SequenceNode :public NonLeafNode//顺序执行节点,执行到某个子节点为真为止
{
public:
	SequenceNode(Node* root) :NonLeafNode(root) {}
	bool exec()
	{
		for (auto child : childNodes)
		{
			if (!child->exec())
			{
				return false;
			}
		}
		return true;
	}
};

class ParallelNode :public NonLeafNode//并行执行节点
{
public:
	ParallelNode(Node* root) :NonLeafNode(root) {}
	bool exec()
	{
		for (auto child : childNodes)
		{
			child->exec();
		}
		return true;
	}
};

class ConditionNode :public Node//条件节点
{
public:
	ConditionNode(Node* root) :Node(root) {}
	bool exec() = 0;
};

class BehaviorNode :public Node//行为节点
{
public:
	BehaviorNode(Node* root) :Node(root) {}
	bool exec() = 0;
};

class OneChildNonLeafNode :public NonLeafNode
{
public:
	OneChildNonLeafNode(Node* root) :NonLeafNode(root) {}
	void addChild(Node* node)
	{
		if (!childNodes.size())
		{
			childNodes.push_back(node);
		}
	}
};

class InvertNode : public OneChildNonLeafNode //取反节点,只能有一个孩子
{
public:
	InvertNode(Node* root) :OneChildNonLeafNode(root) {}
	virtual bool exec()override
	{
		return !childNodes[0]->exec();
	}
};

class CountNode : public OneChildNonLeafNode {//循环执行节点
public:
	CountNode(Node* root, int cnt = 0) :OneChildNonLeafNode(root), count(cnt) {}
	virtual bool exec()override
	{
		while (--count) {
			if (childNodes[0]->exec() == false)return false;
		}
		return true;
	}
protected:
	int count;
};



