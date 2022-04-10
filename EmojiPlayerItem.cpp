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
	//每30ms检查一次是否水平移动或者下落 
	mPlayerTimer->setInterval(30);
	connect(mPlayerTimer, &QTimer::timeout, this, &EmojiPlayerItem::moveHorizontalEmojiPlayer);
	connect(mPlayerTimer, &QTimer::timeout, this, &EmojiPlayerItem::moveDownEmojiPlayer);
	
	mPoint = new AtkPointItem(QPixmap("atkpoint.png"), this, mParentScene);
	mPoint->setPos(pos().x() + boundingRect().width() / 2,
		pos().y() + 10);
	mParentScene->addItem(mPoint);
	mPoint->setOffsetX(boundingRect().width() / 2);
	mPoint->setOffsetY(10);
	mPoint->hide();

	setJumpAnimation();
	setDownAnimation();
	setBeHitAnimation();
}

void EmojiPlayerItem::setJumpAnimation()
{
	mJumpAnimation = new QPropertyAnimation;
	mJumpAnimation->setTargetObject(this);
	mJumpAnimation->setPropertyName("jumpFactor");
	mJumpAnimation->setStartValue(0);
	mJumpAnimation->setKeyValueAt(0.5, 1);
	mJumpAnimation->setEndValue(-3.5);//这个值是考虑到跳出界外的情况
	//否则会降落在半空中
	mJumpAnimation->setDuration(800);
	mJumpAnimation->setEasingCurve(QEasingCurve::OutInQuad);
}

void EmojiPlayerItem::setDownAnimation()
{
	mDownAnimation = new QPropertyAnimation;
	mDownAnimation->setTargetObject(this);
	mDownAnimation->setPropertyName("downFactor");
	mDownAnimation->setStartValue(0);
	mDownAnimation->setKeyValueAt(0.5, -1.5);
	mDownAnimation->setEndValue(-5);
	//-5在该地图中可以保证会下落到界外
	mDownAnimation->setDuration(600);
	mDownAnimation->setEasingCurve(QEasingCurve::InQuad);
}

void EmojiPlayerItem::setBeHitAnimation()
{
	mBeHitAnimation = new QPropertyAnimation;
	mBeHitAnimation->setTargetObject(this);
	mBeHitAnimation->setPropertyName("beHitFactor");
	mBeHitAnimation->setStartValue(0);
	mBeHitAnimation->setKeyValueAt(0.5, 1);
	mBeHitAnimation->setEndValue(-3.5);
	//防止停留在半空中
	mBeHitAnimation->setDuration(800);
	mBeHitAnimation->setEasingCurve(QEasingCurve::OutInQuad);
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
	//死亡时不执行任何动作
	if (life() == 0) {
		mPlayerTimer->stop();
		return;
	}
	//方向有-1 0 1三个状态,详见addHorizontalInput
	if (direction() == 0) {
		mPlayerTimer->stop();
		return;
	}
	else if (!mPlayerTimer->isActive()) {
		mPlayerTimer->start();
		return;
	}
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

bool EmojiPlayerItem::checkOutsideOfGameView()
{
	//除了超出上边框,其他情况都返回
	qreal x = pos().x();
	qreal y = pos().y();
	if (x<0 || x>scene()->width() || y>scene()->height() - 150)
	{
		//由于跳跃和被击打动作的endvalue可能不够持续下落,即产生
		//停在半空中的情况,故将复活判断值上调
		return true;
	}
	
	return false;
}

void EmojiPlayerItem::restoreLife()
{
	this->setPos(mStartPosX, mStartPosY);
	mLife = 1;
	mCoefficient = 0;
	mDirection = 0;
	mHorizontalInput = 0;
	mFilter = 1;
	
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

void EmojiPlayerItem::restorecoefficient()
{
	mCoefficient = 0;
}

void EmojiPlayerItem::addcoefficient()
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

void EmojiPlayerItem::setHorizontalInput(int input)
{
	mHorizontalInput = input;
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

	//Filter为1代表刚刚死亡,若此时继续按移动键则将该键被按过记录下来并返回
	//用于在keyReleaseEvent中过滤
	if (mFilter == 1)
	{
		if (event->key() == Qt::Key_A)
		{
			mAPressed = 1;
		}
		else if (event->key() == Qt::Key_D)
		{
			mDPressed = 1;
		}
		return;
	}

	switch (event->key()) {

	case Qt::Key_A:
		mAPressed = 1;
		addHorizontalInput(-1);
		checkTimer();
		break;

	case Qt::Key_D:
		mDPressed = 1;
		addHorizontalInput(1);
		checkTimer();
		break;

	case Qt::Key_Space:
		jump();
		break;

	case Qt::Key_J:
		mPoint->show();//攻击时才会将攻击点显示出来
		mPoint->hitTimer()->start();
		mPoint->hit();
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
	
	if (life() == 0) return;

	//这两个键的release事件不需要被处理
	if (event->key() == Qt::Key_J || event->key() == Qt::Key_Space)
	{
		return;
	}

	//若刚刚死亡且两个键都处在被按住的状态,则释放一个键并返回
	if (mFilter == 1 && (mAPressed && mDPressed))
	{
		switch (event->key()) {
		case Qt::Key_A:
			mAPressed = 0;
			break;
		case Qt::Key_D:
			mDPressed = 0;
			break;
		default:
			break;
		}
		return;
	}
	
	if (mFilter == 1) {
		mFilter = 0;//若死亡则原来的keyRelease不执行
		//下一次出现release才会执行
		return;
	}

	switch (event->key()) {
	case Qt::Key_A:
		mAPressed = 0;
		addHorizontalInput(1);
		checkTimer();
		break;
	case Qt::Key_D:
		mDPressed = 0;
		addHorizontalInput(-1);
		checkTimer();
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
		mJumpStartLevel = pos().y();
		mLastJumpValue = 0;
		mJumpAnimation->start();
	}
}

void EmojiPlayerItem::beHit()
{
	if (QAbstractAnimation::Stopped == mBeHitAnimation->state()) {
		addcoefficient();
		mBeHitStartLevel = pos().y();
		mLastBeHitValue = 0;
		mBeHitStartPos = pos().x();
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

	qreal groundY = mJumpStartLevel;
	qreal curJumpValue = mJumpAnimation->currentValue().toReal();
	qreal y = groundY - curJumpValue * mJumpHeight;
	setY(y);

	if (checkOutsideOfGameView()
		&& life() == 1) {
		restoreLife();
		mJumpAnimation->stop();
		return;
	}

	qreal collideY = mParentScene->checkColliding(this);
	if (curJumpValue < mLastJumpValue
		&& collideY != EmojiScene::CHECK_COLLIDING_FAILURE_HEIGHT)
	{
		setY(collideY);
		mJumpAnimation->stop();
		return;
	}
	mLastJumpValue = curJumpValue;
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

	qreal groundY = mDownStartLevel;
	qreal curDownValue = mDownAnimation->currentValue().toReal();
	qreal y = groundY - curDownValue * mDownHeight;

	if (checkOutsideOfGameView()
		&& life() == 1) {
		restoreLife();
		mDownAnimation->stop();
		return;
	}

	//下落过程中检测是否可以降落
	qreal collideY = mParentScene->checkColliding(this);

	if (collideY != EmojiScene::CHECK_COLLIDING_FAILURE_HEIGHT)
	{
		setY(collideY);
		mDownAnimation->stop();
		return;
	}
	
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

	qreal curBeHitTime = mBeHitAnimation->currentValue().toReal();
	qreal curBeHitStep = mBeHitAnimation->currentTime();

	const int cushionCoefficient = 20000;//除以这个值使得跳跃距离得以缩短

	qreal y = mBeHitStartLevel - curBeHitTime * mBeHitHeight;
	qreal x = mBeHitStartPos + curBeHitStep *
		mBeHitHeight * mBeHitDirection *
		coefficient() / cushionCoefficient;

	if (checkOutsideOfGameView()
		&& life() == 1) {
		restoreLife();
		mBeHitAnimation->stop();
		return;
	}

	//在下降时检测是否可以降落
	qreal curBeHitValue = mBeHitAnimation->currentValue().toReal();
	qreal collideY = mParentScene->checkColliding(this);
	if (mLastBeHitValue> curBeHitValue
		&& collideY != EmojiScene::CHECK_COLLIDING_FAILURE_HEIGHT)
	{
		setY(collideY);
		mBeHitAnimation->stop();
		return;
	}

	setX(x);
	setY(y);
	mLastBeHitValue = curBeHitValue;
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

	const int dx = direction() * mWorldSpeed;
	qreal newX = pos().x() + dx;

	setX(newX);
}

void EmojiPlayerItem::moveDownEmojiPlayer()
{
	if (QAbstractAnimation::Running == mBeHitAnimation->state())
	{
		return;
	}
	if (!mParentScene->checkDownConditionColliding(this)) {
		return;
	}

	if (QAbstractAnimation::Running == mJumpAnimation->state()) {
		return;
	}
	mDownStartLevel = pos().y();
	mDownAnimation->start();
}