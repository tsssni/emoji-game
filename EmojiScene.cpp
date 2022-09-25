#include "EmojiScene.h"
#include "EmojiPlayerItem.h"
#include "GroundItem.h"
#include "BackgroundItem.h"
#include "PlatformItem.h"
#include "AtkPoint.h"
#include "AIItem.h"

#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QImage>
#include <QPixmap>

#define PLATFORM1 420-160
#define PLATFORM2 420
//在设计aiplayer时，应当注意到，jump函数中可能存在emojiplayer与aiplayer碰撞的情况
//并且aiplayer的jump函数应当进行适当修改，以及mJumpStartLevel对于aiplayer不适用
EmojiScene::EmojiScene()
	:
	mEmojiPlayer(Q_NULLPTR),
	mAIPlayer(Q_NULLPTR),
	mBackground(Q_NULLPTR),
	mGround(Q_NULLPTR),
	mPlatform(Q_NULLPTR)
{
	mBackground = new BackgroundItem(QPixmap("Background.png"));
	setSceneRect(mBackground->boundingRect());
	mBackground->setPos(0, 0);

	addItem(mBackground);

	mGround = new GroundItem(QPixmap("Ground.png"));
	QRectF groundRect = mGround->boundingRect();
	mGround->setPos((width() - groundRect.width()) / 2,
		height() - groundRect.height());
	addItem(mGround);

	mPlatform = new PlatformItem * [4];
	for (int i = 0; i < 4; ++i)
	{
		mPlatform[i] = new PlatformItem(QPixmap("Platform.png"));
		QRectF platformRect = mPlatform[i]->boundingRect();
		switch (i)
		{
		case 0:mPlatform[i]->setPos(100, PLATFORM2); break;
		case 1:mPlatform[i]->setPos
			  (250, PLATFORM1); break;
		case 2:mPlatform[i]->setPos
			  (width() - 250 - platformRect.width(),
				  PLATFORM1); break;
		case 3:mPlatform[i]->setPos(width() - 100 - platformRect.width(),
			PLATFORM2); break;
		}

		addItem(mPlatform[i]);
	}

	mEmojiPlayer = new EmojiPlayerItem(QPixmap("Player.png"), this);
	mEmojiPlayer->setPos(mGround->pos().x() + 50,
		mGround->pos().y() - mEmojiPlayer->boundingRect().height());
	mEmojiPlayer->setStartPosX(mEmojiPlayer->pos().x());
	mEmojiPlayer->setStartPosY(mEmojiPlayer->pos().y());
	addItem(mEmojiPlayer);

	mAIPlayer = new AIItem(QPixmap("AI.png"), this);
	mAIPlayer->setPos(mGround->pos().x()
		+ mGround->boundingRect().width()
		- mAIPlayer->boundingRect().width() - 50,
		mGround->pos().y() - mAIPlayer->boundingRect().height());
	mAIPlayer->setStartPosX(mAIPlayer->pos().x());
	mAIPlayer->setStartPosY(mAIPlayer->pos().y());
	addItem(mAIPlayer);
}

EmojiScene::~EmojiScene()
{
	if (mBackground) delete mBackground;
	if (mGround) delete mGround;
	if (mPlatform)
	{
		for (int i = 0; i < 4; ++i)
		{
			delete mPlatform[i];
		}
		delete[] mPlatform;
	}
}

//由于EmojiView的keyPressEvent会屏蔽传递给scene和item的keyPressEvent
//故需要由view将event传递给scene,再传给玩家
void EmojiScene::keyPressEvent(QKeyEvent* event)
{
	mEmojiPlayer->keyPressEvent(event);
}

void EmojiScene::keyReleaseEvent(QKeyEvent* event)
{
	mEmojiPlayer->keyReleaseEvent(event);
}

bool EmojiScene::checkMoveCollision(EmojiPlayerItem* player)
{

	//由于角色可能并非精准的踩在物体上,故行走时经常会出发下降动画
	//只需要判断角色脚部与物体顶部y坐标之差是否在一个小范围内即可
	//不需要二者完全相等
	
	const static qreal delta = 7;

	int dir = player->lastDirction();
	qreal playerLeft = player->pos().x();
	qreal playerRight = player->pos().x() + dir * player->boundingRect().width();
	if (dir == -1)
	{
		std::swap(playerLeft, playerRight);
	}
	qreal playerBottom = player->pos().y() + player->boundingRect().height();

	for (QGraphicsItem* item : this->items())
	{
		if (item == mBackground || item == mEmojiPlayer
			|| item == mEmojiPlayer->point() || item == mAIPlayer
			|| item == mAIPlayer->point())
		{
			continue;
		}

		qreal itemLeft = item->pos().x();
 		qreal itemRight = item->pos().x() + item->boundingRect().width();
		qreal itemTop = item->pos().y();

		if (playerRight > itemLeft && playerLeft < itemRight
 			&& playerBottom > itemTop - delta && playerBottom < itemTop + delta)
		{
			player->setY(itemTop - player->boundingRect().height());
			player->stopAnimations();
			return true;
		}
	}

	return false;
}

bool EmojiScene::checkHitCollision(AtkPointItem* atkPoint)
{
	//判断攻击点是否打到角色
	for (QGraphicsItem* item : collidingItems(atkPoint)) {
		EmojiPlayerItem* enemy = dynamic_cast<EmojiPlayerItem*>(item);
		if (enemy && enemy != atkPoint->player())
		{
			enemy->setBeHitDirection(atkPoint->player()->lastDirction());
			enemy->beHit();
			return true;
		}
	}
	return false;
}

