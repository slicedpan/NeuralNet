#pragma once
#include <vector>

class Perceptron
{
public:
	Perceptron(float thresholdValue);
	~Perceptron(void);
	virtual void Increment(float amount);
	virtual float GetValue();
	virtual bool CheckFired();
	virtual void Reset();
	inline float GetThreshold()
	{
		return threshold;
	}
	float threshold;
protected:
	float currentValue;	
};

