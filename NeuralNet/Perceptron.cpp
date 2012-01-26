#include "Perceptron.h"

#define M_E 2.7182818284590452353602874713526624f

Perceptron::Perceptron(float thresholdValue, float selfValue) : threshold(thresholdValue), value(selfValue)
{
	currentValue = 0.0f;
}

Perceptron::~Perceptron(void)
{
}

float Perceptron::GetValue()
{
	return 1.0f / (1 + pow(M_E, currentValue));
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
