#include "EmojiNodes.h"
#include "EmojiScene.h"
#include "EmojiDecisionTree.h"
#include "AIItem.h"
#include "PlatformItem.h"
#include "AtkPoint.h"
#include <vector>

//根据追逐的目标物体来选择ai的朝向
//可能是玩家,可能是跳台,故传入QGraphicsItem
void changeAIDirection(QGraphicsItem* item, EmojiPlayerItem* ai)
{
    qreal xPlayer = item->pos().x();
    qreal xAI = ai->pos().x();
    qreal dist = xPlayer - xAI;

    if (dist > 0)
    {
        ai->setDirection(1);
    }
    else
    {
        ai->setDirection(-1);
    }
}


//对跳台根据距离地面距离由小到大排序
bool cmp(PlatformItem* a, PlatformItem* b)
{
    return a->pos().y() > b->pos().y();
}

//返回追逐路线上的最优跳台
PlatformItem* checkNearestPlatform
(std::vector<PlatformItem*>& vPlatform,
    AIItem* ai, EmojiPlayerItem* player)
{
    std::sort(vPlatform.begin(), vPlatform.end(), cmp);
    int i = 0;
    int j = 1;
    //首先排除比自己矮的跳台
    while (i < vPlatform.size()
        && vPlatform.at(i)->pos().y()
        - ai->boundingRect().height()
        >= ai->pos().y())
    {
        ++i;
    }   
    
    //i到达数组尾部代表没有可行跳台
    if (i == vPlatform.size())
    {
        return Q_NULLPTR;
    }

    j = i;

    //找到第一个可行跳台i后,寻找j使得区间[i,j)中的跳台都在同一水平面上
    while (j < vPlatform.size()
        && vPlatform.at(i)->pos().y() == vPlatform.at(j)->pos().y())
    {
        ++j;
    }
    
    //我们根据x轴上跳台距离玩家的远近来选择最优跳台
    //最近的是最优的
    qreal playerX = player->pos().x() + player->boundingRect().width() / 2;
    qreal dist = 0x3f3f3f3f;
    PlatformItem* nearestPlatform = Q_NULLPTR;

    for (int k = i; k < j; ++k)
    {
        qreal platformX = vPlatform.at(k)->pos().x()
            + vPlatform.at(k)->boundingRect().width() / 2;
        qreal y = vPlatform.at(k)->pos().y();

        qreal distPlayer = fabs(platformX - playerX);
        if (distPlayer < dist)
        {
            dist = distPlayer;
            nearestPlatform = vPlatform.at(k);
        }
    }

    return nearestPlatform;
}

bool CondNear::isNear()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QGraphicsScene* scene = root->AI()->scene();
    for (auto item : scene->items())
    {
        EmojiPlayerItem* player = dynamic_cast<EmojiPlayerItem*>(item);
        
        if (player == root->AI())
        {
            continue;
        }
        
        if (player)
        {
            QPointF posPlayer = player->pos();
            QPointF posAI = root->AI()->pos();

            //根据人物之间中心点的距离来判断是否攻击
            qreal playerX = posPlayer.x() + player->boundingRect().width() / 2;
            qreal playerY = posPlayer.y();
            qreal AIX = posAI.x() + root->AI()->boundingRect().width() / 2;
            qreal AIY = posAI.y();

            qreal delta = 5;

            //若水平距离小于120且处在同一水平面上则执行攻击
            //由于角色可能并非精准的踩在平面上 故设置一个delta值
            if (fabs(playerX - AIX) < 120 
                && fabs(playerY - AIY) < delta)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

bool CondWalk::isWalkExecutable()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QGraphicsScene* scene = root->AI()->scene();
    for (auto item : scene->items())
    {
        EmojiPlayerItem* player = dynamic_cast<EmojiPlayerItem*>(item);
        
        if (item == root->AI())
        {
            continue;
        }
        
        if (player)
        {
            QPointF posPlayer = player->pos();
            QPointF posAI = root->AI()->pos();
            const qreal delta = 5;

            if ((fabs(posPlayer.y() - posAI.y()) < delta)
                && fabs(posPlayer.x() - posAI.x()) >= 120)
            {
                return true;
            }
            
            return false;
        }
    }
}

bool CondJump::isJumpExecutable()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QGraphicsScene* scene = root->AI()->scene();
    for (auto item : scene->items())
    {
        EmojiPlayerItem* player = dynamic_cast<EmojiPlayerItem*>(item);

        if (item == root->AI())
        {
            continue;
        }

        if (player)
        {
            QPointF posPlayer = player->pos();
            QPointF posAI = root->AI()->pos();

            //玩家比自己高则执行
            if (posAI.y() - posPlayer.y() > 0)
            {
                return true;
            }
            return false;
        }
    }
}

bool CondDown::isDownExecutable()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QGraphicsScene* scene = root->AI()->scene();
    for (auto item : scene->items())
    {
        EmojiPlayerItem* player = dynamic_cast<EmojiPlayerItem*>(item);

        if (player == root->AI())
        {
            continue;
        }

        if (player)
        {
            //玩家比自己低则执行
            if (player->pos().y() > root->AI()->pos().y())
            {
                return true;
            }
            return false;
        }
    }
}

bool BehaviorHit::exec()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QGraphicsScene* scene = root->AI()->scene();
    AtkPointItem* point = root->AI()->point();

    //遍历所有item,找到player并执行动作
    for (auto item : scene->items())
    {
        EmojiPlayerItem* player = dynamic_cast<EmojiPlayerItem*>(item);

        if (item == root->AI())
        {            
            continue;
        }

        if (player)
        {
            changeAIDirection(player, root->AI());

            point->show();
            point->hitTimer()->start();
            point->hit();
        }
    }

    return true;
}



bool BehaviorWalk::exec()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QGraphicsScene* scene = root->AI()->scene();

    for (auto item : scene->items())
    {
        if (item == root->AI()) 
        {
            continue;
        }

        EmojiPlayerItem* player = dynamic_cast<EmojiPlayerItem*>(item);
        if (player)
        {
            changeAIDirection(player, root->AI());
            root->AI()->checkTimer();
            return true;
        }
    }
}

bool BehaviorJump::exec()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QGraphicsScene* scene = root->AI()->scene();
    std::vector<PlatformItem*> vPlatform;
    EmojiPlayerItem* player;

    for (auto item : scene->items())
    {
        PlatformItem* platform = dynamic_cast<PlatformItem*>(item);
        EmojiPlayerItem* itemPlayer = dynamic_cast<EmojiPlayerItem*>(item);

        if (itemPlayer)
        {
            if (itemPlayer == root->AI())
            {
                continue;
            }
            else
            {
                player = itemPlayer;
            }
        }


        if (platform)
        {
            vPlatform.emplace_back(platform);
        }
        
    }

    PlatformItem* nearestPlatform
        = checkNearestPlatform(vPlatform, root->AI(), player);
    if (!nearestPlatform)
    {
        return false;
    }
    changeAIDirection(nearestPlatform, root->AI());

    

    //distX1为与跳台左端的距离,distX2为与右端的距离
    qreal distX1 = fabs(nearestPlatform->pos().x()
        - root->AI()->pos().x() - root->AI()->boundingRect().width() / 2);
    qreal distX2 = fabs(nearestPlatform->pos().x()
        + nearestPlatform->boundingRect().width()
        - root->AI()->pos().x() - root->AI()->boundingRect().width() / 2);
    //选择较近的那一个来决定是否跳跃
    qreal distX = fmin(distX1, distX2);

    if (distX > 50)
    {
        root->AI()->checkTimer();
    }
    else
    {
        root->AI()->jump();
    }

    return true;

}

bool BehaviorDown::exec()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QGraphicsScene* scene = root->AI()->scene();

    for (auto item : scene->items())
    {
        if (item == root->AI())
        {
            continue;
        }

        EmojiPlayerItem* player = dynamic_cast<EmojiPlayerItem*>(item);
        if (player)
        {
            //采用较为简单的逻辑:往中间走直到落地
            //由于地图设计的原因,向中间走一定会落地
            //若更改地图则需要重新设计动作
            if (root->AI()->pos().x() < player->scene()->width() / 2)
            {
                root->AI()->setDirection(1);
            }
            else
            {
                root->AI()->setDirection(-1);
            }
            root->AI()->checkTimer();
            return true;
        }
    }
}
