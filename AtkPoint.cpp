#include "AtkPoint.h"
#include "EmojiPlayerItem.h"
#include "EmojiScene.h"
#include <QPropertyAnimation>
#include <QTimer>

AtkPointItem::AtkPointItem
(const QPixmap& pixmap,
	EmojiPlayerItem* player,
	EmojiScene* parentScene,
	QGraphicsItem* parent)
	:QGraphicsPixmapItem(pixmap, parent),
	mPlayer(player),
	mParentScene(parentScene),
	mHitTimer(new QTimer)
{
	mHitTimer->setInterval(30);
	connect(mHitTimer, &QTimer::timeout, this, &AtkPointItem::checkHit);
	setHitAnimation();
}

qreal AtkPointItem::offsetX()
{
	return mOffsetX;
}

qreal AtkPointItem::offsetY()
{
	return mOffsetY;
}

void AtkPointItem::setOffsetX(qreal x)
{
	mOffsetX = x;
}

void AtkPointItem::setOffsetY(qreal y)
{
	mOffsetY = y;
}

void AtkPointItem::setHitAnimation()
{
	mHitAnimation = new QPropertyAnimation;
	mHitAnimation->setTargetObject(this);
	mHitAnimation->setPropertyName("hitFactor");
	mHitAnimation->setStartValue(0);
	mHitAnimation->setKeyValueAt(0.5, 1);
	mHitAnimation->setEndValue(0);
	mHitAnimation->setDuration(500);
	mHitAnimation->setEasingCurve(QEasingCurve::OutCirc);
}

QPropertyAnimation* AtkPointItem::hitAnimation()
{
	return mHitAnimation;
}

EmojiPlayerItem* AtkPointItem::player()
{
	return mPlayer;
}

QTimer* AtkPointItem::hitTimer()
{
	return mHitTimer;
}

qreal AtkPointItem::hitFactor() const
{
	return mHitFactor;
}

void AtkPointItem::setHitFactor(const qreal& hitFactor)
{
	if (mHitFactor == hitFactor) {
		return;
	}

	mHitFactor = hitFactor;
	emit hitFactorChanged(mHitFactor);

	qreal curHitValue = mHitAnimation->currentValue().toReal();

	qreal groundY = mPlayer->pos().y() + offsetY();
	qreal y = mPlayer->pos().y() + offsetY();

	qreal groundX = mPlayer->pos().x()
		+ offsetX()
		* mPlayer->lastDirction();
	qreal x = groundX + curHitValue * mHitHeight
		* mPlayer->lastDirction();

	if (mParentScene->checkHitCollision(this))
	{
		hide();
		mHitAnimation->stop();
		return;
	}

	setX(x);
	setY(y);
}

void AtkPointItem::hit()
{
	if (QAbstractAnimation::Stopped == mHitAnimation->state()) {
		mHitAnimation->start();
	}
}

void AtkPointItem::checkHit()
{
	if (mHitTimer->isActive() && QAbstractAnimation::Stopped == mHitAnimation->state()) {
		mHitTimer->stop();
		hide();
		mPlayer->setPixmap(QPixmap("player.png"));
		return;
	}
}
