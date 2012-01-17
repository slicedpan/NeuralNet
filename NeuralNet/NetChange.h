#pragma once

class NeuralNetwork;

struct NetChange
{
	int layer;
	int startIndex;
	float oldWeight;
	float newWeight;
};

