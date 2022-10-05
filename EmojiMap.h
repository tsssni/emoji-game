#pragma once
#include "PlatformItem.h"
#include <vector>
using std::vector;
class EmojiScene;

class EmojiMap
{
public:
	void setPlatformsPtr(vector<PlatformItem*>* platformsPtr);
	void setPlatform(qint16 x, qint16 y, QString platformTexPath);
	void buildGraph(int jumpDist, int jumpHeight);
	bool search(int aiIndex, int playerPos, vector<int>& route);

protected:
	bool dijShortestPath(int start, int end, vector<int>& prePlatform);

	vector<PlatformItem*>* mPlatforms;
	vector<vector<int>> mPlatformGraph;
	vector<vector<int>> mPlatformGraphEdge;
};

