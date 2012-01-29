#include "StdAfx.h"
#include "OutputNode.h"


OutputNode::OutputNode(void) : Perceptron(0.0f)
{
}

OutputNode::~OutputNode(void)
{
}

float OutputNode::GetValue()
{
	return currentValue / count;	
}

void OutputNode::Increment(float amount)
{
	currentValue += amount;
	++count;
}

void OutputNode::Reset()
{
	currentValue = 0;
	count = 0;
}
