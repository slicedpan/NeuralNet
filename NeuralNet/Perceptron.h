#pragma once

class Perceptron
{
public:
	Perceptron(float thresholdValue);
	~Perceptron(void);
	virtual void Increment(float amount);
	virtual float GetValue();
	virtual void Reset();
	float threshold;
protected:
	float currentValue;	
};

