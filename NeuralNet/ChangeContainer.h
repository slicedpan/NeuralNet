#pragma once

#include <vector>

struct NetChange;
struct NodeChange;

class ChangeContainer
{
public:
	ChangeContainer(void);
	~ChangeContainer(void);
	std::vector<NetChange*> netChanges;
	std::vector<NodeChange*> nodeChanges;
};

