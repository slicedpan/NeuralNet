#include "StdAfx.h"
#include "InputNode.h"

InputNode::InputNode() : Perceptron(0.0f)
{
	threshold = 0.0f;
}

InputNode::~InputNode(void)
{
}

void InputNode::SetValue(float amount)
{
	currentValue = amount;	
}
