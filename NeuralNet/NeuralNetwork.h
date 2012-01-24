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
	void Revert(std::vector<NodeChange> nodeChanges, std::vector<NetChange> netChanges);
	void ReApply(std::vector<NodeChange> nodeChanges, std::vector<NetChange> netChanges);
private:
	int inputs;
	int outputs;
	int depth;
	int maxWidth;
	std::vector<std::vector<Perceptron*> > perceptrons;
	std::vector<std::vector<Connection*> > connections;
	std::vector<InputNode*> inputLayer;
	NodeChange ChangeNodeValue();
	NetChange AddConnection();
	NetChange ChangeConnectionWeight();
};

