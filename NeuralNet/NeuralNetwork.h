#pragma once

#include <vector>
#include "NodeChange.h"
#include "NetChange.h"

class Perceptron;
class Connection;
class InputNode;

class NeuralNetwork
{
public:
	NeuralNetwork(int numberOfInputs, int numberOfOutputs, int depth, int maxWidth);
	~NeuralNetwork(void);
	int Inputs();
	int Outputs();	
	float* Compute(float* inputs);
	int fireCount;
	int nodeCount;
	int connectionCount;
	void Mutate();
	void Revert();
	void ReApply();
private:
	int inputs;
	int outputs;
	int depth;
	int maxWidth;
	std::vector<std::vector<Perceptron*> > perceptrons;
	std::vector<std::vector<Connection*> > connections;
	std::vector<InputNode*> inputLayer;
	std::vector<NetChange> netChanges;
	std::vector<NodeChange> nodeChanges;
	NodeChange ChangeNodeValue();
	NetChange AddConnection();
	NetChange ChangeConnectionWeight();
};

