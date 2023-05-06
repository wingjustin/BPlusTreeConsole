#include "DataSearchTree.h"

using namespace DataSearchTree;

BpNode::BpNode(const int maxLength) {
	//initialize
	length = 0;
	startElem = NULL;
	endElem = NULL;
	startSubNode = NULL;
	prev = NULL;
	next = NULL;
	leftParent = NULL;
	rightParent = NULL;
	BpNode::maxLength = maxLength;
}

BpNode::~BpNode() {
	if (startElem) {
		BpElement* current = startElem;
		BpElement* next = current->nextElem;
		delete current->leftNode;
		if (!next || next->leftNode != current->rightNode)
			delete current->rightNode;
		delete current;
		while (current = next) {
			next = current->nextElem;
			delete current->leftNode;
			if (!next || next->leftNode != current->rightNode)
				delete current->rightNode;
			delete current;
		}
		current = NULL;
		next = NULL;
	}

	startElem = NULL;
	endElem = NULL;
	startSubNode = NULL;
	prev = NULL;
	next = NULL;
	leftParent = NULL;
	rightParent = NULL;
}

bool BpNode::IsOverflow() {
	return length > maxLength;
}

bool BpNode::IsHungry() {
	return length < maxLength >> 1;
}

bool BpNode::IsEmpty() {
	return length < 1;
	//return !startElem;
}

bool BpNode::InMinimum() {
	return length <= maxLength >> 1;
}

BpElement* BpNode::GetElement(const int key) {
	BpElement* current = startElem;
	while (current->nextElem && current->nextElem->key <= key)
		current = current->nextElem;
	return current;
}

void BpNode::InsertElements(BpElement* const newElem, const bool updateLength) {
	if (!newElem)
		return;
	if (endElem) {
		InsertElementsToNext(endElem, newElem, updateLength);
	}
	else {
		startElem = newElem;
		if (startSubNode) {
			startElem->leftNode = startSubNode;
			startSubNode->rightParent = startElem;
		}
		else if (startElem->leftNode)
			startSubNode = startElem->leftNode;
		endElem = startElem;
		endElem->thisNode = this;
		if (updateLength) {
			length++;
			while (endElem->nextElem) {
				endElem = endElem->nextElem;
				endElem->thisNode = this;
				length++;
			}
		}
		else {
			while (endElem->nextElem) {
				endElem = endElem->nextElem;
				endElem->thisNode = this;
			}
		}
	}
}

// newElem -> targetElement
void  BpNode::InsertElementsToPrev(BpElement* const targetElem, BpElement* const newElem, const bool updateLength) {
	if (!newElem)
		return;
	if (!targetElem || targetElem->thisNode != this)
		return;
	if (targetElem->prevElem) {
		BpElement* middleLeft= newElem;
		middleLeft->thisNode = targetElem->thisNode;
		BpElement* middleRight = middleLeft;
		if (updateLength) {
			length++;
			while (middleRight->nextElem) {
				middleRight = middleRight->nextElem;
				middleRight->thisNode = targetElem->thisNode;
				length++;
			}
		}
		else {
			while (middleRight->nextElem) {
				middleRight = middleRight->nextElem;
				middleRight->thisNode = targetElem->thisNode;
			}
		}
		BpElement* left = targetElem->prevElem;
		BpElement* right = targetElem;
		//binding left
		left->nextElem = middleLeft;
		middleLeft->prevElem = left;
		//binding right
		middleRight->nextElem = right;
		right->prevElem = middleRight;
		//binding new child node
		middleLeft->leftNode = left->rightNode;
		if (middleLeft->leftNode)
			middleLeft->leftNode->rightParent = middleLeft;
		right->leftNode = middleRight->rightNode;
		if (right->leftNode)
			right->leftNode->rightParent = right;
	}
	else {
		BpElement* leftLeft = newElem;
		leftLeft->thisNode = targetElem->thisNode;
		BpElement* leftRight = leftLeft;
		if (updateLength) {
			length++;
			while (leftRight->nextElem) {
				leftRight = leftRight->nextElem;
				leftRight->thisNode = targetElem->thisNode;
				length++;
			}
		}
		else {
			while (leftRight->nextElem) {
				leftRight = leftRight->nextElem;
				leftRight->thisNode = targetElem->thisNode;
			}
		}
		BpElement* right = targetElem;
		//binding
		leftRight->nextElem = right;
		right->prevElem = leftRight;
		//change start point
		startElem = leftLeft;
		//binding new child node
		if (startSubNode) {
			leftLeft->leftNode = startSubNode;
			startSubNode->rightParent = leftLeft;
		}
		else
			startSubNode = leftLeft->leftNode;
		right->leftNode = leftRight->rightNode;
		if (right->leftNode)
			right->leftNode->rightParent = right;
	}
}

// targetElement -> newElem
void  BpNode::InsertElementsToNext(BpElement* const targetElem, BpElement* const newElem, const bool updateLength) {
	if (!newElem)
		return;
	if (!targetElem || targetElem->thisNode != this)
		return;
	if (targetElem->nextElem) {
		BpElement* middleLeft = newElem;
		middleLeft->thisNode = targetElem->thisNode;
		BpElement* middleRight = middleLeft;
		if (updateLength) {
			length++;
			while (middleRight->nextElem) {
				middleRight = middleRight->nextElem;
				middleRight->thisNode = targetElem->thisNode;
				length++;
			}
		}
		else {
			while (middleRight->nextElem) {
				middleRight = middleRight->nextElem;
				middleRight->thisNode = targetElem->thisNode;
			}
		}
		BpElement* left = targetElem;
		BpElement* right = targetElem->nextElem;
		//binding left
		left->nextElem = middleLeft;
		middleLeft->prevElem = left;
		//binding right
		middleRight->nextElem = right;
		right->prevElem = middleRight;
		//binding new child node
		middleLeft->leftNode = left->rightNode;
		if (middleLeft->leftNode)
			middleLeft->leftNode->rightParent = middleLeft;
		right->leftNode = middleRight->rightNode;
		if (right->leftNode)
			right->leftNode->rightParent = right;
	}
	else {
		BpElement* left = targetElem;
		BpElement* rightLeft = newElem;
		rightLeft->thisNode = targetElem->thisNode;
		BpElement* rightRight = rightLeft;
		if (updateLength) {
			length++;
			while (rightRight->nextElem) {
				rightRight = rightRight->nextElem;
				rightRight->thisNode = targetElem->thisNode;
				length++;
			}
		}
		else {
			while (rightRight->nextElem) {
				rightRight = rightRight->nextElem;
				rightRight->thisNode = targetElem->thisNode;
			}
		}
		//binding
		left->nextElem = rightLeft;
		rightLeft->prevElem = left;
		//change start point
		endElem = rightRight;
		//binding new child node
		rightLeft->leftNode = left->rightNode;
		if (rightLeft->leftNode)
			rightLeft->leftNode->rightParent = rightLeft;
	}
}

void BpNode::RemoveElement(BpElement* const targetElem, const bool updateLength) {
	if (!targetElem || targetElem->thisNode != this)
		return;
	BpElement* prev = targetElem->prevElem;
	BpElement* next = targetElem->nextElem;
	if (prev && next) {
		next->prevElem = prev;
		prev->nextElem = next;
		if (prev->rightNode) {
			prev->rightNode->rightParent = next;
			next->leftNode = prev->rightNode;
		}
		targetElem->nextElem = NULL;
		targetElem->prevElem = NULL;
		targetElem->leftNode = NULL;
		if (targetElem->rightNode)
			targetElem->rightNode->rightParent = NULL;
	}
	else if (prev) {
		prev->nextElem = NULL;
		if (prev->rightNode)
			prev->rightNode->rightParent = NULL;
		if (endElem == targetElem)
			endElem = prev;
	}
	else if (next) {
		next->prevElem = NULL;
		next->leftNode = targetElem->leftNode;
		if (next->leftNode)
			next->leftNode->rightParent = next;
		if (startElem == targetElem)
			startElem = next;
	}
	else {
		if (targetElem->leftNode)
			targetElem->leftNode->rightParent = NULL;
		if (startElem == targetElem)
			startElem = NULL;
		if (endElem == targetElem)
			endElem = NULL;
		//startSubNode = targetElem->leftNode;
	}
	targetElem->nextElem = NULL;
	targetElem->prevElem = NULL;
	targetElem->leftNode = NULL;
	if (targetElem->rightNode)
		targetElem->rightNode->rightParent = NULL;
	targetElem->thisNode = NULL;
	if (updateLength)
		length--;
}

void BpNode::ClearElements(const bool updateLength) {
	startElem = NULL;
	endElem = NULL;
	if (startSubNode && startSubNode->rightParent) {
		startSubNode->rightParent->leftNode = NULL;
		startSubNode->rightParent = NULL;
	}
	if (updateLength)
		length = 0;
}
