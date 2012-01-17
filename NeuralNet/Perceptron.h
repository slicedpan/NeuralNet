#pragma once
#include <vector>

class Perceptron
{
public:
	Perceptron(float thresholdValue, float selfValue);
	~Perceptron(void);
	virtual void Increment(float amount);
	virtual float GetValue();
	virtual bool CheckFired();
	virtual void Reset();
	float value;
protected:
	float threshold;
	float currentValue;	
};

