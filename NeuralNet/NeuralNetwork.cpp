#include "NeuralNetwork.h"
#include "Perceptron.h"
#include "OutputNode.h"
#include "InputNode.h"
#include "Connection.h"
#include <cstdlib>

void NeuralNetwork::ChangeConnectionWeight()
{
	NetChange change;
	change.layer = rand() % depth + 1;
	change.startIndex = rand() % connections[change.layer].size();
	change.oldWeight = connections[change.layer][change.startIndex]->weight;
	change.newWeight = ((float) rand() / RAND_MAX) - 0.5f;
	change.newWeight *= 0.5f;	
	change.newWeight += change.oldWeight;
	if (change.newWeight > 1.0f)
		change.newWeight = 1.0f;
	else if (change.newWeight < 0.0f)
		change.newWeight = 0.0f;
	connections[change.layer][change.startIndex]->weight = change.newWeight;
	netChanges.push_back(change);
}

void NeuralNetwork::AddConnection()
{
	NetChange change;
	change.layer = rand() % depth + 1;
	change.startIndex = rand() % perceptrons[change.layer].size();	
	change.oldWeight = 0.0f;
	change.newWeight = ((float) rand() / RAND_MAX) - 0.5f;
	change.newWeight *= 0.5f;	
	change.newWeight += change.oldWeight;
	if (change.newWeight > 1.0f)
		change.newWeight = 1.0f;
	else if (change.newWeight < 0.0f)
		change.newWeight = 0.0f;
	int endIndex = rand() % perceptrons[change.layer + 1].size();
	connections[change.layer].push_back(new Connection(change.startIndex, change.newWeight, perceptrons[change.layer + 1][endIndex]));
	change.startIndex = connections[change.layer].size() - 1;
	netChanges.push_back(change);
	++connectionCount;
}

void NeuralNetwork::ChangeNodeValue()
{
	NodeChange change;
	change.layer = rand() % depth + 1;
	change.index = rand() % perceptrons[change.layer].size();
	change.oldValue = perceptrons[change.layer][change.index]->value;
	change.newValue = (float) rand() / RAND_MAX;
	perceptrons[change.layer][change.index]->value = change.newValue;
	nodeChanges.push_back(change);
}

void NeuralNetwork::Revert()
{
	for (int i = 0; i < nodeChanges.size(); ++i)
	{
		Perceptron* node = perceptrons[nodeChanges[i].layer][nodeChanges[i].index];
		node->value = nodeChanges[i].oldValue;
	}
	for (int i = 0; i < netChanges.size(); ++i)
	{
		Connection* conn = connections[netChanges[i].layer][netChanges[i].startIndex];
		conn->weight = netChanges[i].oldWeight;
	}
}

void NeuralNetwork::ReApply()
{
	for (int i = 0; i < nodeChanges.size(); ++i)
	{
		Perceptron* node = perceptrons[nodeChanges[i].layer][nodeChanges[i].index];
		node->value += nodeChanges[i].newValue - nodeChanges[i].oldValue;
	}
	for (int i = 0; i < netChanges.size(); ++i)
	{
		Connection* conn = connections[netChanges[i].layer][netChanges[i].startIndex];
		conn->weight += netChanges[i].newWeight - netChanges[i].oldWeight;
	}
}

void NeuralNetwork::Mutate()
{
	int numChanges = 5 + rand() % 5;
	netChanges.clear();
	nodeChanges.clear();
	for (int i = 0; i < numChanges; ++i)
	{
		int type = rand() % 3;
		switch (type)
		{
		case 0:
			ChangeConnectionWeight();
			break;
		case 1:
			AddConnection();
			break;
		case 2:
			ChangeNodeValue();
			break;
		}
	}
	printf("%d connections\n", connectionCount);
}

NeuralNetwork::NeuralNetwork(int numberOfInputs, int numberOfOutputs, int depth, int maxWidth)
{
	inputs = numberOfInputs;
	outputs = numberOfOutputs;
	this->depth = depth;
	this->maxWidth = maxWidth;

	nodeCount = 0;
	connectionCount = 0;

	std::vector<Perceptron*> firstLayer;
	for (int i = 0; i < inputs; ++i)
	{		
		InputNode* node;
		node = new InputNode();
		inputLayer.push_back(node);
		firstLayer.push_back(node);
	}
	perceptrons.push_back(firstLayer);

	for (int i = 0; i < depth; ++i)
	{
		int width = (rand() % (maxWidth / 2)) + (maxWidth / 2);
		std::vector<Perceptron*> layer;
		for (int j = 0; j < width; ++j)
		{
			float threshold = (float)rand() / (float)RAND_MAX;
			float value = (float)rand() / (float)RAND_MAX;
			layer.push_back(new Perceptron(threshold, value));
		}
		perceptrons.push_back(layer);
		nodeCount += width;
	}

	std::vector<Perceptron*> outputLayer;
	for (int i = 0; i < outputs; ++i)
	{
		outputLayer.push_back(new OutputNode());
	}
	perceptrons.push_back(outputLayer);

	for (int i = 0; i <= depth; ++i)
	{
		std::vector<Connection*> connectionLayer;
		int connectionNum = (rand() % perceptrons[i].size()) * 10 ;
		connectionNum += perceptrons[i].size() * 2;
		connectionCount += connectionNum;
		for (int j = 0; j < connectionNum; ++j)
		{
			int inputPos = rand() % perceptrons[i].size();
			int outputPos = rand() % perceptrons[i + 1].size();
			float weight = (float)rand() / (float) RAND_MAX;
			Perceptron* outputNode = perceptrons[i + 1][outputPos];
			connectionLayer.push_back(new Connection(inputPos, weight, outputNode));
		}
		connections.push_back(connectionLayer);
	}
	
}

int NeuralNetwork::Inputs()
{
	return inputs;
}

int NeuralNetwork::Outputs()
{
	return outputs;
}

NeuralNetwork::~NeuralNetwork(void)
{
}

float* NeuralNetwork::Compute(float* inputData)
{
	fireCount = 0;
	float* outputArray = (float*)malloc(sizeof(float) * outputs);
	for (int i = 0; i < inputs; ++i)
	{
		inputLayer[i]->SetValue(inputData[i]);
	}

	for (int i = 1; i <= depth + 1; ++i)
	{
		for (int j = 0; j < perceptrons[i].size(); ++j)
			perceptrons[i][j]->Reset();
	}

	for (int i = 0; i <= depth; ++i)
	{
		for (int j = 0; j < connections[i].size(); ++j)
		{
			Perceptron* inNode = perceptrons[i][connections[i][j]->inputIndex];
			if (inNode->CheckFired())
			{
				connections[i][j]->Activate(inNode->GetValue());
				++fireCount;
			}
		}
	}

	for (int i = 0; i < outputs; ++i)
	{
		outputArray[i] = perceptrons[depth + 1][i]->GetValue();
	}
	return outputArray;
}
