#pragma once

//Each node except root can have a maximum of M children and at least ceil(M / 2) children.
//Each node can contain a maximum of M ¡V 1 keys and a minimum of ceil(M / 2) ¡V 1 keys.
//The root has at least two children and at least one search key.
//While insertion overflow of the node occurs when it contains more than M ¡V 1 search key values.
//Here M is the order of B + tree.

#define MIN_TREE_ORDER 3 // Tree order must be more than 2
#define MIN_DATA_ORDER 3 // Data order must be more than 2
#define DEFAULT_TREE_ORDER 3 // Default 3, the max number of childs
#define DEFAULT_DATA_ORDER 5 // Default 5, the max number - 1 of data in leaf node

namespace DataSearchTree {
	class BpTree{
	private:
		int treeOrder; // default 3, max number of subtree
		int dataOrder; // default 5, max number of data in leaf node + next pointer

		bool HealthCheck(BpNode* current); // for debug
	protected:
		BpNode* root;

		void LeafChangeParentKey(BpNode* leafNode, int oldKey);

		//for insertion
		void InsertElemToLeafNode(BpElement* targetElem, BpNode* leafNode, int key, string* data);
		void SplitOverflowTreeNode(BpNode* overflowNode);

		//for delete operation
		void RemoveElemFromLeafNode(BpElement* targetElem, BpNode* leafNode);
		void LackElemTreeNodeRebalance(BpNode* lackElemNode);
	public:
		BpTree();
		BpTree(int order);
		BpTree(int treeOrder, int dataOrder);
		~BpTree();

		void Clear();

		int GetTreeOrder();
		int GetDataOrder();

		string* SearchData(int key);
		BpElement* SearchElement(int key);
		void Insert(int key, string* data);
		void Insert(int key, string value);
		bool Remove(int key);

		bool HealthCheck(); // for debug
	};
}
