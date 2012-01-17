#pragma once
#include "perceptron.h"
class InputNode : public Perceptron
{
public:
	InputNode();
	~InputNode(void);
	void SetValue(float amount);
	bool CheckFired();
};

