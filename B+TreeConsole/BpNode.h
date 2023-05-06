#pragma once

namespace DataSearchTree {
	class BpNode {
	protected:
		int length;
		int maxLength;

		BpElement* leftParent;
		BpElement* rightParent;
		BpElement* startElem;
		BpElement* endElem;
		BpNode* startSubNode;
		BpNode* prev;
		BpNode* next;

		BpElement* GetElement(int key);

		void InsertElements(BpElement* newElem, bool updateLength);
		void InsertElementsToPrev(BpElement* targetElem, BpElement* newElem, bool updateLength);
		void InsertElementsToNext(BpElement* targetElem, BpElement* newElem, bool updateLength);

		void RemoveElement(BpElement* targetElem, bool updateLength);
		void ClearElements(bool updateLength);

		friend class BpTree;
	public:
		BpNode(int maxLength);
		~BpNode();

		bool IsOverflow();  // length > maxLength;

		//the minimum number of keys = floor(maxKey/2) = ceil(order/2) - 1
		bool IsHungry(); // length < maxLength / 2;
		bool IsEmpty();
		bool InMinimum(); // length <= maxLength / 2;
	};
}
