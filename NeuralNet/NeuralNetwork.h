#pragma once

#include <vector>
#include "NodeChange.h"
#include "NetChange.h"

class Perceptron;
class Connection;
class InputNode;
class ChangeContainer;

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
	ChangeContainer* Mutate(int numChanges);
	void Revert(ChangeContainer* changeContainer);
	void ApplyChanges(ChangeContainer* changeContainer);
	void SaveToFile(char* filename);
	static NeuralNetwork* LoadFromFile(char* filename);
private:
	int inputs;
	int outputs;
	int depth;
	int maxWidth;
	std::vector<std::vector<Perceptron*> > perceptrons;
	std::vector<std::vector<Connection*> > connections;
	std::vector<InputNode*> inputLayer;
	NodeChange* ChangeNodeValue();
	NetChange* ChangeConnectionWeight();
	void ApplyNodeChange(NodeChange* change);
	void ApplyNetChange(NetChange* change);
	void RevertNodeChange(NodeChange* change);
	void RevertNetChange(NetChange* change);
};

