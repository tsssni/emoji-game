#pragma once
#include "PlatformItem.h"
#include <vector>
using std::vector;

class EmojiMap
{
public:
	bool setPlatform(qint16 x, qint16 y, QString platformTexPath = "Platform.png");
	void buildGraph();
	const vector<PlatformItem>& search(QPoint aiPos, QPoint playerPos) const;

	const vector<PlatformItem>& getPlatforms();
protected:
	bool checkPlatformOverlap(qint16 x, qint16 y);

	vector<PlatformItem> mPlatforms;
	vector<vector<PlatformItem>> mPlatformGraph;
};

