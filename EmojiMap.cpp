#include "EmojiMap.h"
#include "EmojiScene.h"
#include <algorithm>

bool cmpPlatform(PlatformItem* p1, PlatformItem* p2)
{
    return p1->y() > p2->y() || (p1->y() == p2->y() && p1->x() < p2->x());
}

void EmojiMap::setPlatformsPtr(vector<PlatformItem*>* platformsPtr)
{
    mPlatforms = platformsPtr;
}

void EmojiMap::setPlatform(qint16 x, qint16 y, QString platformTexPath)
{
    QSize rect = QPixmap(platformTexPath).size();
    mPlatforms->push_back(new PlatformItem(QPixmap(platformTexPath)));
    mPlatforms->back()->setPos(x, y);
}

void EmojiMap::buildGraph(int jumpDist, int jumpHeight)
{
    auto& platforms = *mPlatforms;

    sort(platforms.begin(), platforms.end(), cmpPlatform);
    for (int i = 0; i < platforms.size(); ++i)
    {
        mPlatformGraph.emplace_back();
        mPlatformGraphEdge.emplace_back();
    }

    for (int i = 0; i < platforms.size(); ++i)
    {
        for (int j = i + 1; j < platforms.size() && fabs(platforms[j]->y() - platforms[i]->y()) <= jumpHeight; ++j)
        {
            if (!(platforms[j]->x() > platforms[i]->x() + platforms[i]->boundingRect().width() + jumpDist
                || platforms[j]->x() + platforms[j]->boundingRect().width() + jumpDist < platforms[i]->x()))
            {
                int currX = platforms[i]->x() + platforms[i]->boundingRect().width() / 2;
                int currY = platforms[i]->y();
                int nextX = platforms[j]->x() + platforms[j]->boundingRect().width() / 2;
                int nextY = platforms[j]->y();
                int dist = sqrt(pow(currX - nextX, 2) + pow(currY - nextY, 2));

                mPlatformGraph[i].push_back(j);
                mPlatformGraph[j].push_back(i);
                mPlatformGraphEdge[i].push_back(dist);
                mPlatformGraphEdge[j].push_back(dist);
            }
        }
    }
    
}

bool EmojiMap::search(int aiPlatform, int playerPlatform, vector<int>& route)
{
    if (aiPlatform == -1 || playerPlatform == -1)
    {
        return false;
    }

    vector<int> prePlatform(mPlatforms->size(), 0);
    if (!dijShortestPath(aiPlatform, playerPlatform, prePlatform))
    {
        return false;
    }

    int i = playerPlatform;
    route.clear();

    while (i != aiPlatform)
    {
        route.push_back(i);
        i = prePlatform[i];
    }
    route.push_back(aiPlatform);

    return true;
}

bool EmojiMap::dijShortestPath(int start, int end, vector<int>& prePlatform)
{
    vector<int> dist(prePlatform.size(), INT_MAX);
    vector<bool> vis(prePlatform.size());

    for (int i = 0; i < mPlatformGraph[start].size(); ++i)
    {
        dist[mPlatformGraph[start][i]] = mPlatformGraphEdge[start][i];
        prePlatform[mPlatformGraph[start][i]] = start;
    }

    dist[start] = 0;
    vis[start] = true;

    for (int i = 0; i < dist.size(); ++i)
    {
        int minDist = INT_MAX;
        int minPlatform = 0;

        for (int j = 0; j < dist.size(); ++j)
        {
            if (!vis[j] && dist[j] < minDist)
            {
                minDist = dist[j];
                minPlatform = j;
            }
        }
        
        if (minDist == INT_MAX)
        {
            break;
        }

        vis[minPlatform] = true;
        for (int j = 0; j < mPlatformGraph[minPlatform].size(); ++j)
        {
            int k = mPlatformGraph[minPlatform][j];
            int e = mPlatformGraphEdge[minPlatform][j];

            if (dist[k] > dist[minPlatform] + e)
            {
                dist[k] = dist[minPlatform] + e;
                prePlatform[k] = minPlatform;
            }
        }  
    }

    if (dist[end] == INT_MAX)
    {
        return false;
    }

    return true;
}
