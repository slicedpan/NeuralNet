#pragma once

struct NodeChange
{
	inline NodeChange(int layer, int index, float oldValue, float newValue)
	{
		this->layer = layer;
		this->index = index;
		this->oldValue = oldValue;
		this->newValue = newValue;
	}
	inline NodeChange() {}
	int layer;
	int index;
	float oldValue; 
	float newValue;
};

