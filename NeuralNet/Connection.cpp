#include "StdAfx.h"
#include "Connection.h"


Connection::Connection(int index, float weight, Perceptron* endPoint, int outIndex)
{
	inputIndex = index;
	outputIndex = outIndex;
	this->weight = weight;
	connected = endPoint;
}

Connection::~Connection(void)
{
}

void Connection::Activate(float amount)
{
	connected->Increment(amount * weight);
}
