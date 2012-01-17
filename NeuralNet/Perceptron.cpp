#include "Perceptron.h"

Perceptron::Perceptron(float thresholdValue, float selfValue) : threshold(thresholdValue), value(selfValue)
{
	currentValue = 0.0f;
}

Perceptron::~Perceptron(void)
{
}

float Perceptron::GetValue()
{
	return value;
}

void Perceptron::Increment(float amount)
{
	currentValue += amount;
}

bool Perceptron::CheckFired()
{
	if (currentValue > threshold)
		return true;
	return false;
}

void Perceptron::Reset()
{
	currentValue = 0.0f;
}
