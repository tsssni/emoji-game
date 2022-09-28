#include "EmojiPlayerItem.h"
#include "GroundItem.h"
#include "EmojiScene.h"
#include "AtkPoint.h"
#include <QPen>
#include <QObject>
#include <QPropertyAnimation>
#include <QKeyEvent>

EmojiPlayerItem::EmojiPlayerItem
(const QPixmap& pixmap,
	EmojiScene* parentScene,
	QGraphicsItem* parent)
	: QGraphicsPixmapItem(pixmap, parent),
	mParentScene(parentScene),
	mHorizontalInput(0), mDirection(0), mLife(1),
	mCoefficient(0), mLastDirection(1),
	mPlayerTimer(new QTimer)
{
	mScene = parentScene;

	//每1ms检查一次是否水平移动或者下落 
	mPlayerTimer->setInterval(5);
	mPlayerTimer->start();
	connect(mPlayerTimer, &QTimer::timeout, this, &EmojiPlayerItem::checkOutsideOfGameView);
	connect(mPlayerTimer, &QTimer::timeout, this, &EmojiPlayerItem::checkMoveCollision);
	connect(mPlayerTimer, &QTimer::timeout, this, &EmojiPlayerItem::moveHorizontalEmojiPlayer);

	connect(this, &EmojiPlayerItem::directionChanged, this, &EmojiPlayerItem::addHorizontalInput);
	connect(this, &EmojiPlayerItem::jumpStart, this, &EmojiPlayerItem::moveUpEmojiPlayer);
	connect(this, &EmojiPlayerItem::hitStart, this, &EmojiPlayerItem::hitAIPlayer);

	
	mPoint = new AtkPointItem(QPixmap("atkpoint.png"), this, mParentScene);
	mPoint->setPos(x() + boundingRect().width() / 2, y() + 10);
	mParentScene->addItem(mPoint);
	mPoint->setOffsetX(boundingRect().width() / 2);
	mPoint->setOffsetY(10);
	mPoint->hide();

	setWalkAnimation();
	setJumpAnimation();
	setDownAnimation();
	setBeHitAnimation();
}

void EmojiPlayerItem::setWalkAnimation()
{
	mWalkAnimation = new QPropertyAnimation;
	mWalkAnimation->setTargetObject(this);
	mWalkAnimation->setPropertyName("walkFactor");
	mWalkAnimation->setStartValue(0);
	mWalkAnimation->setEndValue(1);	
	mWalkAnimation->setDuration(5);
	mWalkAnimation->setEasingCurve(QEasingCurve::InQuad);
}

void EmojiPlayerItem::setJumpAnimation()
{
	mJumpAnimation = new QPropertyAnimation;
	mJumpAnimation->setTargetObject(this);
	mJumpAnimation->setPropertyName("jumpFactor");
	mJumpAnimation->setStartValue(0);
	mJumpAnimation->setKeyValueAt(0.2, 1);
	mJumpAnimation->setEndValue(-3.5);	
	mJumpAnimation->setDuration(5000);
	mJumpAnimation->setEasingCurve(QEasingCurve::OutQuart);
}

void EmojiPlayerItem::setDownAnimation()
{
	mDownAnimation = new QPropertyAnimation;
	mDownAnimation->setTargetObject(this);
	mDownAnimation->setPropertyName("downFactor");
	mDownAnimation->setStartValue(0);
	mDownAnimation->setEndValue(1);
	mDownAnimation->setDuration(10000);
	mDownAnimation->setEasingCurve(QEasingCurve::Linear);
}

void EmojiPlayerItem::setBeHitAnimation()
{
	mBeHitAnimation = new QPropertyAnimation;
	mBeHitAnimation->setTargetObject(this);
	mBeHitAnimation->setPropertyName("beHitFactor");
	mBeHitAnimation->setStartValue(0);
	mBeHitAnimation->setKeyValueAt(0.2, 1);
	mBeHitAnimation->setEndValue(-3.5);
	mBeHitAnimation->setDuration(5000);
	mBeHitAnimation->setEasingCurve(QEasingCurve::OutQuart);
}

QPropertyAnimation* EmojiPlayerItem::walkAnimation()
{
	return mWalkAnimation;
}

QPropertyAnimation* EmojiPlayerItem::jumpAnimation()
{
	return mJumpAnimation;
}

QPropertyAnimation* EmojiPlayerItem::downAnimation()
{
	return mDownAnimation;
}

QPropertyAnimation* EmojiPlayerItem::beHitAnimation()
{
	return mBeHitAnimation;
}

QTimer* EmojiPlayerItem::playerTimer()
{
	return mPlayerTimer;
}

void EmojiPlayerItem::checkTimer()
{
}

int EmojiPlayerItem::direction() const
{
	return mDirection;
}

void EmojiPlayerItem::setDirection(int direction)
{
	mDirection = direction;
	if (mDirection != 0) {
		mLastDirection = mDirection;
		QTransform transform;
		if (mDirection < 0) {
			//方向朝左则翻转图片
			transform.scale(-1, 1);
		}
		setTransform(transform);
	}
}

int EmojiPlayerItem::lastDirction()
{
	//返回当前朝向
	return mLastDirection;
}

void EmojiPlayerItem::restoreLife()
{
	this->setPos(mStartPosX, mStartPosY);
	mLife = 1;
	mCoefficient = 0;
	
	return;
}

int EmojiPlayerItem::life() const{
	return mLife;
}

void EmojiPlayerItem::setLife()
{
	mLife = 0;
	return;
}

int EmojiPlayerItem::coefficient() const
{
	return mCoefficient;
}

void EmojiPlayerItem::restoreCoefficient()
{
	mCoefficient = 0;
}

void EmojiPlayerItem::addCoefficient()
{
	mCoefficient += 5;
}

void EmojiPlayerItem::setStartPosX(qreal x)
{
	mStartPosX = x;
}

void EmojiPlayerItem::setStartPosY(qreal y)
{
	mStartPosY = y;
}

qreal EmojiPlayerItem::startPosX() const
{
	return mStartPosX;
}

qreal EmojiPlayerItem::startPosY() const
{
	return mStartPosX;
}

void EmojiPlayerItem::addHorizontalInput(int input)
{
	//若为1 则向右走 为0不动 为-1向左
	//例子:keyPrees中的D使得角色向右,mHorizontalInput为1
	//执行keyRelease时会传进-1,使其为0并停止
	mHorizontalInput += input;
	setDirection(qBound(-1, mHorizontalInput, 1));
}

void EmojiPlayerItem::keyPressEvent(QKeyEvent* event)
{
	//长按则返回
	if (event->isAutoRepeat() ) {
		return;
	}

	if (life() == 0) {
		return;
	}

	switch (event->key()) {

	case Qt::Key_A:
		emit(directionChanged(-1));
		break;

	case Qt::Key_D:
		emit(directionChanged(1));
		break;
	}
	
	switch(event->key()) {
	case Qt::Key_Space:
		emit(jumpStart());
		break;

	case Qt::Key_J:
		emit(hitStart());
		break;

	default:
		break;
	}
}

void EmojiPlayerItem::keyReleaseEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat()) {
		return;
	}
	
	switch (event->key()) {
	case Qt::Key_A:
		emit(directionChanged(1));
		break;
	case Qt::Key_D:
		emit(directionChanged(-1));
		break;
	default:
		break;
	}
}

void EmojiPlayerItem::jump()
{
	if ((QAbstractAnimation::Running != mBeHitAnimation->state()
		|| QAbstractAnimation::Running != mDownAnimation->state())
		&& QAbstractAnimation::Stopped == mJumpAnimation->state()) {
		setCurrPlatform(-1);
		mJumpStartLevel = y();
		mLastJumpValue = 0;
		mJumpAnimation->start();
	}
}

void EmojiPlayerItem::beHit()
{
	if (QAbstractAnimation::Stopped == mBeHitAnimation->state()) {
		setCurrPlatform(-1);
		addCoefficient();
		mBeHitStartLevel = y();
		mLastBeHitValue = 0;
		mBeHitStartPos = x();
		mBeHitAnimation->start();
	}
}

void EmojiPlayerItem::setBeHitDirection(int value)
{
	mBeHitDirection = value;
}

AtkPointItem* EmojiPlayerItem::point()
{
	return mPoint;
}

qreal EmojiPlayerItem::walkFactor() const
{
	return mWalkFactor;
}

void EmojiPlayerItem::setWalkFactor(const qreal& walkFactor)
{
}

qreal EmojiPlayerItem::jumpFactor() const
{
	return mJumpFactor;
}

void EmojiPlayerItem::setJumpFactor(const qreal& jumpFactor)
{
	if (mJumpFactor == jumpFactor) {
		return;
	}

	mJumpFactor = jumpFactor;
	emit jumpFactorChanged(mJumpFactor);

	qreal y = mJumpStartLevel - mJumpFactor * mJumpHeight;
	setY(y);
}

qreal EmojiPlayerItem::downFactor() const
{
	return mDownFactor;
}

void EmojiPlayerItem::setDownFactor(const qreal& downFactor)
{
	if (mDownFactor == downFactor) {
		return;
	}

	mDownFactor = downFactor;
	emit downFactorChanged(mDownFactor);

	qreal y = mDownStartLevel  + mDownFactor * mDownHeight;
	setY(y);
}

qreal EmojiPlayerItem::beHitFactor() const
{
	return mBeHitFactor;
}

void EmojiPlayerItem::setBeHitFactor(const qreal& beHitFactor)
{
	if (mBeHitFactor == beHitFactor) {
		return;
	}

	mBeHitFactor = beHitFactor;
	emit beHitFactorChanged(mBeHitFactor);

	qreal curBeHitStep = mBeHitAnimation->currentTime();
	const int cushionCoefficient = 20000;//除以这个值使得跳跃距离得以缩短

	qreal y = mBeHitStartLevel - mBeHitFactor * mBeHitHeight;
	qreal x = mBeHitStartPos + curBeHitStep *
		mBeHitHeight * mBeHitDirection *
		coefficient() / cushionCoefficient;
	setX(x);
	setY(y);
}

void EmojiPlayerItem::setCurrPlatform(int index)
{
	mCurrPlatform = index;
}

int EmojiPlayerItem::platform()
{
	return mCurrPlatform;
}

void EmojiPlayerItem::moveHorizontalEmojiPlayer()
{
	if (QAbstractAnimation::Running == mBeHitAnimation->state())
	{
		return;
	}
	
	if (direction() == 0) {
		return;
	}

 	const qreal dx = (qreal)direction() * mWorldSpeed;
	qreal newX = x() + dx;
	setX(newX);
}

void EmojiPlayerItem::moveUpEmojiPlayer()
{
	jump();
}

void EmojiPlayerItem::hitAIPlayer()
{
	mPoint->show();
	mPoint->hitTimer()->start();
	mPoint->hit();
}

void EmojiPlayerItem::checkOutsideOfGameView()
{
	qreal x = this->x();
	qreal y = this->y();
	if (x<0 || x>scene()->width() || y > scene()->height() - 150)
	{
		//由于跳跃和被击打动作的endvalue可能不够持续下落,即产生
		//停在半空中的情况,故将复活判断值上调
		stopAnimations();
		restoreLife();
	}
}

void EmojiPlayerItem::checkMoveCollision()
{
	if (QAbstractAnimation::Running == mJumpAnimation->state()
		&& mJumpAnimation->currentTime() < 10)
	{
		return;
	}

	if (QAbstractAnimation::Running == mBeHitAnimation->state()
		&& mBeHitAnimation->currentTime() < 10)
	{
		return;
	}

	if (!mParentScene->checkMoveCollision(this) 
		&& QAbstractAnimation::Stopped == mJumpAnimation->state() 
		&& QAbstractAnimation::Stopped==mBeHitAnimation->state())
	{
		setCurrPlatform(-1);
		mDownStartLevel = y();
		mDownAnimation->start();
	}
}

void EmojiPlayerItem::stopAnimations()
{
	if (QAbstractAnimation::Running == mDownAnimation->state())
	{
		mDownAnimation->stop();
	}

	if (QAbstractAnimation::Running == mJumpAnimation->state())
	{
		mJumpAnimation->stop();
	}

	if (QAbstractAnimation::Running == mBeHitAnimation->state())
	{
		mBeHitAnimation->stop();
	}

}

EmojiScene* EmojiPlayerItem::scene()
{
	return mScene;
}
