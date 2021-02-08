//-------------------------------------------------------------------------------------

#pragma hdrstop

#include "Tree.h"
//-------------------------------------------------------------------------------------
#pragma package(smart_init)
//-------------------------------------------------------------------------------------
void Tree::InitializeTree()
{
	nodes[ROOT].child = ROOT + 1;
	nodes[ROOT].isLeafChild = false;
	nodes[ROOT].weight = 2;
	nodes[ROOT].parent = -1;

	nodes[ROOT + 1].child = END_OF_STREAM;
	nodes[ROOT + 1].isLeafChild = true;
	nodes[ROOT + 1].weight = 1;
	nodes[ROOT + 1].parent = ROOT;
	leaves[END_OF_STREAM] = ROOT + 1;

	nodes[ROOT + 2].child = ESCAPE;
	nodes[ROOT + 2].isLeafChild = true;
	nodes[ROOT + 2].weight = 1;
	nodes[ROOT + 2].parent = ROOT;
	leaves[ESCAPE] = ROOT + 2;

	nextFreeNode = ROOT + 3;

	for (int i = 0; i < END_OF_STREAM; i++)
		leaves[i] = -1;
}
//-------------------------------------------------------------------------------------
void Tree::RebuildTree()
{
	unsigned weight;
	int i, j, k;
	j = nextFreeNode - 1;
	for (i = j; i >= ROOT; i--)
	{
		if (nodes[i].isLeafChild)
		{
			nodes[j] = nodes[i];
			nodes[j].weight = (nodes[j].weight + 1) / 2;
			j--;
		}
	}
	for (i = nextFreeNode - 2; j >= ROOT; i -= 2, j--)
	{
		k = i + 1;
		nodes[j].weight = nodes[i].weight + nodes[k].weight;
		weight = nodes[j].weight;
		nodes[j].isLeafChild = false;
		for (k = j + 1; weight < nodes[k].weight; k++);
		k--;
		memmove(&nodes[j], &nodes[j + 1], (k - j) * sizeof(Node));
		nodes[k].weight = weight;
		nodes[k].child = i;
		nodes[k].isLeafChild = false;
	}
	for (i = nextFreeNode - 1; i >= ROOT; i--)
	{
		if (nodes[i].isLeafChild)
		{
			k = nodes[i].child;
			leaves[k] = i;
		}
		else
		{
			k = nodes[i].child;
			nodes[k].parent = nodes[k + 1].parent = i;
		}
	}
}
//-------------------------------------------------------------------------------------
void Tree::SwapNodes(const int i, const int j)
{
	Node node;
	if (nodes[i].isLeafChild)
		leaves[nodes[i].child] = j;
	else
	{
		nodes[nodes[i].child].parent = j;
		nodes[nodes[i].child + 1].parent = j;
	}
	if (nodes[j].isLeafChild)
		leaves[nodes[j].child] = i;
	else
	{
		nodes[nodes[j].child].parent = i;
		nodes[nodes[j].child + 1].parent = i;
	}
	node = nodes[i];
	nodes[i] = nodes[j];
	nodes[i].parent = node.parent;
	node.parent = nodes[j].parent;
	nodes[j] = node;
}
//-------------------------------------------------------------------------------------
void Tree::AddNewNode(const int c)
{
	int lightestNode = nextFreeNode - 1;
	int newNode = nextFreeNode;
	int zeroWeightNode = nextFreeNode + 1;

	nextFreeNode += 2;

	nodes[newNode] = nodes[lightestNode];
	nodes[newNode].parent = lightestNode;
	leaves[nodes[newNode].child] = newNode;

	nodes[lightestNode].child = newNode;
	nodes[lightestNode].isLeafChild = false;

	nodes[zeroWeightNode].child = c;
	nodes[zeroWeightNode].isLeafChild = true;
	nodes[zeroWeightNode].weight = 0;
	nodes[zeroWeightNode].parent = lightestNode;
	leaves[c] = zeroWeightNode;
}
//-------------------------------------------------------------------------------------
int Tree::Leaves(const int c)
{
	return leaves[c];
}
//-------------------------------------------------------------------------------------
Tree::Node Tree::Nodes(const int c)
{
	return nodes[c];
}
//-------------------------------------------------------------------------------------
void Tree::AddWeight(const int c)
{
	nodes[c].weight++;
}
//-------------------------------------------------------------------------------------
