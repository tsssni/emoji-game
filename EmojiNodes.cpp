#include "EmojiNodes.h"
#include "EmojiScene.h"
#include "EmojiDecisionTree.h"
#include "AIItem.h"
#include "PlatformItem.h"
#include "AtkPoint.h"
#include "EmojiMap.h"
#include <vector>

bool cmp(PlatformItem* a, PlatformItem* b)
{
    return a->y() > b->y();
}

bool CondNear::isNear()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    QPointF posPlayer = root->player()->pos();
    QPointF posAI = root->AI()->pos();

	//根据人物之间中心点的距离来判断是否攻击
	qreal playerX = posPlayer.x() + root->player()->boundingRect().width() / 2;
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

bool CondWalk::isWalkExecutable()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    EmojiScene* scene = root->scene();
    vector<PlatformItem*>& platforms = scene->platforms();

    AIItem* ai = root->AI();
    vector<int>& route = ai->route();

    int dir;
    int vert;

    if (route.size() > 1)
    {
        qreal aiLeft;
        qreal aiRight;
        if (ai->lastDirction() == 1)
        {
            aiLeft = ai->x();
            aiRight = ai->x() + ai->boundingRect().width();
        }
        else
        {
            aiLeft = ai->x() - ai->boundingRect().width();
            aiRight = ai->x();
        }

        int currPlat = route[route.size() - 1];
        qreal currPlatformWidth = platforms[currPlat]->boundingRect().width();
        qreal currPlatformLeft = platforms[currPlat]->x();
        qreal currPlatformRight = platforms[currPlat]->x() + platforms[currPlat]->boundingRect().width();
      
        int nextPlat = route[route.size() - 2];
        qreal nextPlatformLeft = platforms[nextPlat]->x();
        qreal nextPlatformRight = platforms[nextPlat]->x() + platforms[nextPlat]->boundingRect().width();

        qreal vert = platforms[nextPlat]->y() - platforms[currPlat]->y();

        if (vert >= 0)
        {
            if (nextPlatformLeft  < currPlatformLeft - 80 && nextPlatformRight > currPlatformRight + 80 )
            {
                int aiToLeft = aiLeft - nextPlatformLeft;
                int aiToRight = nextPlatformRight - aiRight;

                dir = aiToLeft >= aiToRight ? -1 : 1;
                ai->setDirection(dir);
                return true;
            }
            else if (nextPlatformLeft < currPlatformLeft - 80)
            {
                ai->setDirection(-1);

                if (aiLeft - currPlatformLeft >= -40)
                {
                    return true;
                }
                else if (currPlatformLeft - nextPlatformRight >= vert)
                {
                     return false;
                }
                else
                {
                    return true;
                }
            }
            else if (nextPlatformRight > currPlatformRight + 80)
            {
                ai->setDirection(1);

                if (currPlatformRight - aiRight >= -40)
                {
                    return true;
                }
                else if (nextPlatformLeft - currPlatformRight >= vert)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        else
        {
            if (nextPlatformLeft<=currPlatformLeft && nextPlatformRight - currPlatformLeft <= currPlatformWidth / 2)
            {
                ai->setDirection(-1);
                
                if (aiLeft - nextPlatformRight >= 20)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (nextPlatformRight >= currPlatformRight && nextPlatformLeft - currPlatformRight >= -currPlatformWidth / 2)
            {
                ai->setDirection(1);

                if (nextPlatformLeft - aiRight >= 20)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                int playerLeft = root->player()->x();
                ai->setDirection(playerLeft - aiLeft <= 0 ? -1 : 1);
                return false;
            }
        }
    }
    else
    {
        EmojiPlayerItem* player = ai->player();
        dir = player->x() - ai->x() > 0 ? 1 : -1;
        ai->setDirection(dir);

        return true;
    }
    
    return false;
}

bool CondJump::isJumpExecutable()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    EmojiScene* scene = root->scene();
    vector<PlatformItem*>& platforms = scene->platforms();

    AIItem* ai = root->AI();
    vector<int>& route = ai->route(); 

    int currPlat = route[route.size() - 1];
    int nextPlat = route[route.size() - 2];
    
    return true;
}

bool BehaviorHit::exec()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    AtkPointItem* point = root->AI()->point();

    qreal aiLeft;
    qreal playerLeft;

	aiLeft = root->AI()->pos().x();
	playerLeft = root->player()->pos().x();

    if (playerLeft < aiLeft)
    {
        root->AI()->setDirection(-1);
    }
    else
    {
        root->AI()->setDirection(1);
    }
    root->AI()->setPixmap(QPixmap("AIAttack.png"));
	//point->show();
	point->hitTimer()->start();
	point->hit();
	return true;
}


bool BehaviorWalk::exec()
{
	return true;
}

bool BehaviorJump::exec()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    AIItem* ai = root->AI();

    ai->jump();

    return true;
}

bool CondRoute::isRouteExist()
{
    RootSel* root = dynamic_cast<RootSel*>(mRoot);
    EmojiScene* scene = root->scene();
    EmojiMap* map = scene->map();
    
    if (root->AI()->platform() == -1)
    {
        return false;
    }

    if (root->player()->platform() == -1)
    {
        return true;
    }

    static int lastPlayerPlatform = -1;
    auto& route = root->AI()->route();

    if (lastPlayerPlatform == root->player()->platform())
    {
        if (root->AI()->platform() == route.back())
        {
            return true;
        }

        if(root->AI()->platform() != route.back()
            && route.size() > 1 && route[route.size() - 2] == root->AI()->platform())
		{
			route.pop_back();
			return true;
		}
    }

    if (!map->search(root->AI()->platform(), root->player()->platform(), route))
    {
        return false;
    }

    lastPlayerPlatform = root->player()->platform();
    return true;
}

RootSel::RootSel(Node* root, EmojiDecisionTree* tree, AIItem* ai)
    :SelectorNode(root), mTree(tree), mAI(ai), mPlayer(ai->player()), mScene(ai->scene())
{}
