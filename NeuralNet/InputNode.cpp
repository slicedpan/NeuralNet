#include "StdAfx.h"
#include "InputNode.h"

InputNode::InputNode() : Perceptron(0.0f, 0.0f)
{
}

InputNode::~InputNode(void)
{
}

void InputNode::SetValue(float amount)
{
	value = amount;
}

bool InputNode::CheckFired()
{
	return true;
}
