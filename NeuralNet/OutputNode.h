#pragma once
#include "perceptron.h"
class OutputNode : public Perceptron
{
public:
	OutputNode(void);
	~OutputNode(void);
	float GetValue();
	void Increment(float amount);
	void Reset();
private:
	int count;
};

