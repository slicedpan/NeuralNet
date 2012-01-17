#pragma once

#include "Perceptron.h"

class Connection
{
public:
	Connection(int index, float weight, Perceptron* endPoint);
	~Connection(void);
	void Activate(float amount);
	int inputIndex;
	float weight;
private:
	Perceptron* connected;
	
};

