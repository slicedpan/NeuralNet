#pragma once

#include "Perceptron.h"

class Connection
{
public:
	Connection(int index, float weight, Perceptron* endPoint, int outIndex);
	~Connection(void);
	void Activate(float amount);
	int inputIndex;
	float weight;
	int outputIndex;
private:
	Perceptron* connected;
	
};

