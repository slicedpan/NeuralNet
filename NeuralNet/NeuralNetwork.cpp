#include "NeuralNetwork.h"
#include "Perceptron.h"
#include "OutputNode.h"
#include "InputNode.h"
#include "Connection.h"
#include "ChangeContainer.h"
#include <cstdlib>

NetChange* NeuralNetwork::ChangeConnectionWeight()
{
	NetChange* change = new NetChange();
	change->layer = rand() % depth + 1;
	change->startIndex = rand() % connections[change->layer].size();
	change->oldWeight = connections[change->layer][change->startIndex]->weight;
	change->newWeight = ((float) rand() / RAND_MAX) - 0.5f;
	change->newWeight *= 0.5f;	
	change->newWeight += change->oldWeight;
	if (change->newWeight > 1.0f)
		change->newWeight = 1.0f;
	else if (change->newWeight < 0.0f)
		change->newWeight = 0.0f;
	return change;
}

void NeuralNetwork::ApplyNetChange(NetChange* change)
{
	connections[change->layer][change->startIndex]->weight = change->newWeight;
}

void NeuralNetwork::ApplyNodeChange(NodeChange* change)
{
	perceptrons[change->layer][change->index]->threshold = change->newValue;
}

void NeuralNetwork::RevertNetChange(NetChange* change)
{
	connections[change->layer][change->startIndex]->weight = change->oldWeight;
}

void NeuralNetwork::RevertNodeChange(NodeChange* change)
{
	perceptrons[change->layer][change->index]->threshold = change->oldValue;
}

/*
NetChange NeuralNetwork::AddConnection()
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
	change.startIndex = connections[change.layer].size() - 1;
	return change;
}
*/

NodeChange* NeuralNetwork::ChangeNodeValue()
{
	NodeChange* change = new NodeChange();
	change->layer = rand() % depth + 1;
	change->index = rand() % perceptrons[change->layer].size();
	change->oldValue = perceptrons[change->layer][change->index]->threshold;
	change->newValue = (float) rand() / RAND_MAX;
	return change;
}

void NeuralNetwork::Revert(ChangeContainer* changeContainer)
{
	for (int i = 0; i < changeContainer->nodeChanges.size(); ++i)
	{
		Perceptron* node = perceptrons[changeContainer->nodeChanges[i]->layer][changeContainer->nodeChanges[i]->index];
		node->threshold = changeContainer->nodeChanges[i]->oldValue;
	}
	for (int i = 0; i < changeContainer->netChanges.size(); ++i)
	{
		Connection* conn = connections[changeContainer->netChanges[i]->layer][changeContainer->netChanges[i]->startIndex];
		conn->weight = changeContainer->netChanges[i]->oldWeight;
	}
}

void NeuralNetwork::ApplyChanges(ChangeContainer* changeContainer)
{
	for (int i = 0; i < changeContainer->nodeChanges.size(); ++i)
	{
		Perceptron* node = perceptrons[changeContainer->nodeChanges[i]->layer][changeContainer->nodeChanges[i]->index];
		node->threshold += changeContainer->nodeChanges[i]->newValue - changeContainer->nodeChanges[i]->oldValue;
	}
	for (int i = 0; i < changeContainer->netChanges.size(); ++i)
	{
		Connection* conn = connections[changeContainer->netChanges[i]->layer][changeContainer->netChanges[i]->startIndex];
		conn->weight += changeContainer->netChanges[i]->newWeight - changeContainer->netChanges[i]->oldWeight;
	}
}

ChangeContainer* NeuralNetwork::Mutate(int numChanges)
{
	ChangeContainer* changeContainer = new ChangeContainer();
	for (int i = 0; i < numChanges; ++i)
	{
		int type = rand() % 2;
		switch (type)
		{
		case 0:
			changeContainer->netChanges.push_back(ChangeConnectionWeight());
			break;
		case 1:
			changeContainer->nodeChanges.push_back(ChangeNodeValue());
			break;
		}
	}
	return changeContainer;
}

NeuralNetwork* NeuralNetwork::LoadFromFile(char* filename)
{
	NeuralNetwork* nnet = new NeuralNetwork();
	if (nnet->InitFromFile(filename))
		return nnet;	
	return 0;
}

bool NeuralNetwork::InitFromFile(char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (!file)
		return false;
	connectionCount = 0;
	nodeCount = 0;
	fread(&depth, sizeof(int), 1, file);
	int layerWidth;
	fread(&layerWidth, sizeof(int), 1, file);
	inputs = layerWidth;
	std::vector<Perceptron*> layer;
	for (int i = 0; i < layerWidth; ++i)
	{
		int value;
		int thresh;
		InputNode* node;
		fread(&thresh, sizeof(float), 1, file);		
		node = new InputNode();
		inputLayer.push_back(node);
		layer.push_back(node);
	}
	perceptrons.push_back(layer);
	nodeCount += layer.size();

	for (int j = 1; j <= depth; ++j)
	{		
		fread(&layerWidth, sizeof(int), 1, file);
		layer.clear();
		for (int i = 0; i < layerWidth; ++i)
		{			
			float thresh;
			float value;
			fread(&thresh, sizeof(float), 1, file);
			layer.push_back(new Perceptron(thresh));
		}
		perceptrons.push_back(layer);
		nodeCount += layer.size();
	}

	fread(&layerWidth, sizeof(int), 1, file);
	layer.clear();
	for (int i = 0; i < layerWidth; ++i)
	{
		float value;
		float thresh;
		fread(&thresh, sizeof(float), 1, file);
		layer.push_back(new OutputNode());
	}

	perceptrons.push_back(layer);
	nodeCount += layer.size();

	outputs = layerWidth;

	std::vector<Connection*> connectionLayer;
	for (int j = 0; j <= depth; ++j)
	{
		fread(&layerWidth, sizeof(int), 1, file);
		connectionLayer.clear();
		for (int i = 0; i < layerWidth; ++i)
		{
			int inputIndex;
			int outputIndex;
			float weight;
			fread(&inputIndex, sizeof(int), 1, file);
			fread(&outputIndex, sizeof(int), 1, file);
			fread(&weight, sizeof(float), 1, file);
			Perceptron* outputNode = perceptrons[j + 1][outputIndex];
			connectionLayer.push_back(new Connection(inputIndex, weight, outputNode, outputIndex));
		}
		connectionCount += connectionLayer.size();
		connections.push_back(connectionLayer);
	}
	return true;
}

void NeuralNetwork::SaveToFile(char* filename)
{
	FILE* file = fopen(filename, "wb");
	fwrite(&depth, sizeof(int), 1, file);

	int layerWidth = inputLayer.size();
	fwrite(&layerWidth, sizeof(int), 1, file);
	for (int i = 0; i < layerWidth; ++i)
	{
		int thresh = inputLayer[i]->threshold;
		fwrite(&thresh, sizeof(float), 1, file);		
	}

	for (int j = 1; j <= depth; ++j)
	{
		layerWidth = perceptrons[j].size();
		fwrite(&layerWidth, sizeof(int), 1, file);
		for (int i = 0; i < layerWidth; ++i)
		{
			float thresh = perceptrons[j][i]->threshold;
			fwrite(&thresh, sizeof(float), 1, file);
		}
	}

	layerWidth = perceptrons[depth + 1].size();
	fwrite(&layerWidth, sizeof(int), 1, file);
	for (int i = 0; i < layerWidth; ++i)
	{
		float thresh = perceptrons[depth + 1][i]->threshold;
		fwrite(&thresh, sizeof(float), 1, file);
	}

	for (int j = 0; j <= depth; ++j)
	{
		layerWidth = connections[j].size();
		fwrite(&layerWidth, sizeof(int), 1, file);
		for (int i = 0; i < layerWidth; ++i)
		{
			fwrite(&connections[j][i]->inputIndex, sizeof(int), 1, file);
			fwrite(&connections[j][i]->outputIndex, sizeof(int), 1, file);
			fwrite(&connections[j][i]->weight, sizeof(float), 1, file);
		}
	}
}

NeuralNetwork::NeuralNetwork()
{

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
		//int width = (rand() % (maxWidth / 2)) + (maxWidth / 2);
		int width = maxWidth;
		std::vector<Perceptron*> layer;
		for (int j = 0; j < width; ++j)
		{
			float threshold = (float)rand() / (float)RAND_MAX;
			float value = (float)rand() / (float)RAND_MAX;
			layer.push_back(new Perceptron(threshold));
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
		for (int j = 0; j < perceptrons[i].size() ; ++j)
		{
			for (int k = 0; k < perceptrons[i + 1].size(); ++k)
			{
				float weight = (float)rand() / (float) RAND_MAX;
				Perceptron* outputNode = perceptrons[i + 1][k];				
				connectionLayer.push_back(new Connection(i, weight, outputNode, k));
			}
		}
		connections.push_back(connectionLayer);
		connectionCount += connectionLayer.size();
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
		if (i == depth)
		{
			i = i;
		}
		for (int j = 0; j < connections[i].size(); ++j)
		{
			Perceptron* inNode = perceptrons[i][connections[i][j]->inputIndex];			
			connections[i][j]->Activate(inNode->GetValue());
			++fireCount;			
		}
	}

	for (int i = 0; i < outputs; ++i)
	{
		float blah = perceptrons[depth + 1][i]->GetValue();
		outputArray[i] = blah;
	}
	return outputArray;
}
