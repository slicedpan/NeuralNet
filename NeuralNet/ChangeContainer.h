#pragma once

#include <vector>

class NetChange;
class NodeChange;

class ChangeContainer
{
public:
	ChangeContainer(void);
	~ChangeContainer(void);
	std::vector<NetChange*> netChanges;
	std::vector<NodeChange*> nodeChanges;
};

