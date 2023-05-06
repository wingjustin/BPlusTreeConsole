#pragma once

#include <string>
#include "DataSearchTree.h"

using namespace std;

namespace DataSearchTree {
	class BpNode;
	class BpElement {
	protected:
		int key;
		string* data;

		BpElement* prevElem;
		BpElement* nextElem;

		BpNode* thisNode;
		BpNode* leftNode;
		BpNode* rightNode;

		friend class BpNode;
		friend class BpTree;
	public:
		BpElement(BpNode* thisNode, int key, string* data); // leaf
		BpElement(BpNode* thisNode, int key, BpNode* leftNode, BpNode* rightNode); // tree
		~BpElement();

		// data
		//**key
		int GetKey();
		//**value
		string* GetData();
		string GetValue();
		void SetValue(string data);
	};
}
