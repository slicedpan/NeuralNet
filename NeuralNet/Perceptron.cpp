#include "Perceptron.h"

#define M_E 2.7182818284590452353602874713526624f

Perceptron::Perceptron(float thresholdValue) : threshold(thresholdValue)
{
	currentValue = 0.0f;
}

Perceptron::~Perceptron(void)
{
}

float Perceptron::GetValue()
{
	float amount = threshold - currentValue;
	if (amount < -45)
		return 0.0f;
	else if (amount > 45)
		return 1.0f;
	return 1.0f / (1.0f + exp(amount));	
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
