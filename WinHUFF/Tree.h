//-------------------------------------------------------------------------------------

#ifndef TreeH
#define TreeH
//-------------------------------------------------------------------------------------
#include <memory>

#define ROOT 0
#define END_OF_STREAM 256
#define ESCAPE 257

#define SYMBOL_COUNT 258
#define NODE_COUNT ((SYMBOL_COUNT * 2) - 1)

#define MAX_WEIGHT 0x8000
//-------------------------------------------------------------------------------------
class Tree
{
private:
	struct Node
	{
		unsigned weight;
		int parent;
		int child;
		bool isLeafChild;
	} nodes[NODE_COUNT];
	int leaves[SYMBOL_COUNT];
	int nextFreeNode;
public:
	void InitializeTree();
	int Leaves(const int c);
	Node Nodes(const int c);
	void AddWeight(const int c);
	void RebuildTree();
	void SwapNodes(const int i, const int j);
	void AddNewNode(const int c);
};
//-------------------------------------------------------------------------------------
#endif
