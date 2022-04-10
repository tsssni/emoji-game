#include "AIItem.h"
#include "EmojiNodes.h"
#include "EmojiDecisionTree.h"
#include <QTimer>
#include <QPropertyAnimation>

AIItem::AIItem(const QPixmap& pixmap,
	EmojiScene* parentScene,
	QGraphicsItem* parent)
	:EmojiPlayerItem(pixmap, parentScene, parent)
{	
	mDecisionTree = new EmojiDecisionTree(this);
	mAITimer = new QTimer;
	mAITimer->setInterval(30);
	mAITimer->start();

	//每30ms执行一次决策树
	connect(mAITimer, &QTimer::timeout,
		this, &AIItem::traverseDecisionTree);
	
	mPlayerTimer = new QTimer;
	mPlayerTimer->setInterval(30);
	
	//每30ms判断一次是否移动和是否下落
	connect(mPlayerTimer, &QTimer::timeout,
		this, &AIItem::moveHorizontalEmojiPlayer);
	connect(mPlayerTimer, &QTimer::timeout,
		this, &AIItem::moveDownEmojiPlayer);
}

//由于决策树没有继承QObject 所以需要手动delete
AIItem::~AIItem()
{
	delete mDecisionTree;
}

void AIItem::traverseDecisionTree()
{
	if (QAbstractAnimation::Running == mDownAnimation->state()
		|| QAbstractAnimation::Running == mBeHitAnimation->state())
	{
		return;
	}

	if (!mDecisionTree->root()->exec())
	{
		setDirection(0);
	}
}
