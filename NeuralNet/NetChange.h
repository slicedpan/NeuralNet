#pragma once

struct NetChange
{
	inline NetChange(int layer, int startIndex, float oldWeight, float newWeight)
	{
		this->layer = layer;
		this->startIndex = startIndex;
		this->oldWeight = oldWeight;
		this->newWeight = newWeight;
	}
	inline NetChange() {}
	int layer;
	int startIndex;
	float oldWeight;
	float newWeight;
};

