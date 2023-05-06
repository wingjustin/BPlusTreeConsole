#include "DataSearchTree.h"

using namespace DataSearchTree;

BpTree::BpTree(const int treeOrder, const int dataOrder) {
	BpTree::treeOrder = treeOrder > MIN_TREE_ORDER
		? treeOrder
		: MIN_TREE_ORDER;
	BpTree::dataOrder = dataOrder > MIN_TREE_ORDER
		? dataOrder
		: MIN_TREE_ORDER;
	root = NULL;
}

BpTree::BpTree(const int order)
	: BpTree(order, order) {
}

BpTree::BpTree()
	: BpTree(DEFAULT_TREE_ORDER, DEFAULT_DATA_ORDER) {
}

BpTree::~BpTree() {
	delete root;
	root = NULL;
}

void BpTree::Clear() {
	delete root;
	root = NULL;
}

int BpTree::GetTreeOrder() {
	return treeOrder;
}

int BpTree::GetDataOrder() {
	return dataOrder;
}

void BpTree::LeafChangeParentKey(BpNode* const leafNode, const int oldKey) {
	//change parent key
	const int newKey = leafNode->startElem->key;
	if (oldKey == newKey)
		return;
	BpNode* current = leafNode;
	while (!current->leftParent && current->rightParent)
		current = current->rightParent->thisNode;
	if (current->leftParent)
		current->leftParent->key = newKey;
	//-----------------------------------------------
	/*BpElement* target = leafNode->leftParent
		&& leafNode->leftParent->key == oldKey
		? leafNode->leftParent
		: leafNode->rightParent;
	while (target && target->key != oldKey) {
		target = target->thisNode->leftParent
			&& target->thisNode->leftParent->key == oldKey
			? target->thisNode->leftParent
			: target->thisNode->rightParent;
	}
	if (target)
		target->key = newKey;*/
}

void BpTree::InsertElemToLeafNode(BpElement* const targetElem, BpNode* const leafNode, const int key, string* const data) {
	//replace if the key is equal
	if (targetElem->key == key) {
		delete targetElem->data;
		targetElem->data = data;
		return;
	}
	//insertion
	else if (targetElem->key < key) {
		leafNode->InsertElementsToNext(targetElem, new BpElement(leafNode, key, data), true);
	}
	else {
		int oldFirstKey = leafNode->startElem->key;
		leafNode->InsertElementsToPrev(targetElem, new BpElement(leafNode, key, data), true);
		if (oldFirstKey != leafNode->startElem->key)
			LeafChangeParentKey(leafNode, oldFirstKey);
	}
	//check if it is overflow
	if (!leafNode->IsOverflow()) {
		return;
	}
	//1. left leaf's length = ceil(MaxLength / 2)
	//2. right leaf's length = floor(MaxLength / 2) + 1
	//3. copy right first key as parent key and parent's length = 1
	BpNode* currentOverflowNode = leafNode;
	int maxLength = currentOverflowNode->maxLength;
	BpElement* rightElem = currentOverflowNode->endElem;
	int rightLength = (maxLength >> 1) + 1;
	int leftLength = currentOverflowNode->length - rightLength;
	for (int i = 1; i < rightLength; i++)
		rightElem = rightElem->prevElem;
	//create a new data Node on right
	BpNode* leftNode = currentOverflowNode;
	BpNode* rightNode = new BpNode(maxLength);
	leftNode->endElem = rightElem->prevElem;
	leftNode->endElem->nextElem = NULL;
	rightElem->prevElem = NULL;
	rightNode->InsertElements(rightElem, false);

	rightNode->next = leftNode->next;
	if (rightNode->next)
		rightNode->next->prev = rightNode;
	leftNode->next = rightNode;
	rightNode->prev = leftNode;

	leftNode->length = leftLength;
	rightNode->length = rightLength;
	//create a parent tree Node on top
	if (leftNode->leftParent) {
		BpNode* parentNode = leftNode->leftParent->thisNode;
		BpElement* parentNewElem = new BpElement(parentNode, rightElem->key, leftNode, rightNode);
		parentNode->InsertElementsToNext(leftNode->leftParent, parentNewElem, true);
		leftNode->rightParent = parentNewElem;
		rightNode->leftParent = parentNewElem;
		if (currentOverflowNode == root)
			root = parentNode;
		if (parentNode->IsOverflow())
			SplitOverflowTreeNode(parentNode);
	}
	else if (leftNode->rightParent) {
		BpNode* parentNode = leftNode->rightParent->thisNode;
		BpElement* parentNewElem = new BpElement(parentNode, rightElem->key, leftNode, rightNode);
		parentNode->InsertElementsToPrev(leftNode->rightParent, parentNewElem, true);
		leftNode->rightParent = parentNewElem;
		rightNode->leftParent = parentNewElem;
		if (currentOverflowNode == root)
			root = parentNode;
		if (parentNode->IsOverflow())
			SplitOverflowTreeNode(parentNode);
	}
	else {
		BpNode* parentNode = new BpNode(treeOrder - 1); // empty Node
		BpElement* parentNewElem = new BpElement(parentNode, rightElem->key, leftNode, rightNode);
		parentNode->InsertElements(parentNewElem, true);
		leftNode->rightParent = parentNewElem;
		rightNode->leftParent = parentNewElem;
		if (currentOverflowNode == root)
			root = parentNode;
	}
}

void BpTree::SplitOverflowTreeNode(BpNode* const overflowNode) {
	if (!overflowNode || !overflowNode->IsOverflow())
		return;
	//1. left leaf's length = ceil(MaxLength / 2)
	//2. right leaf's length = floor(MaxLength / 2)
	//3. switch right first key to parent and parent's length = 1
	BpNode* currentOverflowNode = overflowNode;
	do {
		int maxLength = currentOverflowNode->maxLength;
		BpElement* rightElem = currentOverflowNode->endElem;
		int rightLength = maxLength >> 1;
		int leftLength = currentOverflowNode->length - rightLength - 1;
		for (int i = 1; i < rightLength; i++)
			rightElem = rightElem->prevElem;
		BpElement* parentNewElem = rightElem->prevElem;
		//create a new tree Node on right
		BpNode* leftNode = currentOverflowNode;
		BpNode* rightNode = new BpNode(maxLength);
		leftNode->endElem = parentNewElem->prevElem;
		leftNode->endElem->nextElem = NULL;
		if (leftNode->endElem->rightNode)
			leftNode->endElem->rightNode->rightParent = NULL;
		parentNewElem->prevElem = NULL;
		parentNewElem->leftNode = leftNode;
		//parentNewElem->leftNode->rightParent = parentNewElem;
		rightNode->InsertElements(rightElem, false);

		rightElem->prevElem = NULL;
		parentNewElem->nextElem = NULL;
		rightElem->leftNode = parentNewElem->rightNode;
		if (rightElem->leftNode) {
			rightElem->leftNode->rightParent = rightElem;
			rightElem->leftNode->leftParent = NULL;
		}
		parentNewElem->rightNode = rightNode;
		//parentNewElem->rightNode->leftParent = parentNewElem;

		leftNode->length = leftLength;
		rightNode->length = rightLength;
		//create a parent tree Node on top
		if (leftNode->leftParent) {
			BpNode* parentNode = leftNode->leftParent->thisNode;
			parentNode->InsertElementsToNext(leftNode->leftParent, parentNewElem, true);
			leftNode->rightParent = parentNewElem;
			rightNode->leftParent = parentNewElem;
			if (currentOverflowNode == root)
				root = parentNode;
			/*if (parentNode->IsOverflow())
				SplitOverflowTreeNode(parentNode);*/
			// change to iteration
			if (parentNode->IsOverflow())
				currentOverflowNode = parentNode;
			else
				currentOverflowNode = NULL;
		}
		else if (leftNode->rightParent) {
			BpNode* parentNode = leftNode->rightParent->thisNode;
			parentNode->InsertElementsToPrev(leftNode->rightParent, parentNewElem, true);
			leftNode->rightParent = parentNewElem;
			rightNode->leftParent = parentNewElem;
			if (currentOverflowNode == root)
				root = parentNode;
			/*if (parentNode->IsOverflow())
				SplitOverflowTreeNode(parentNode);*/
			// change to iteration
			if (parentNode->IsOverflow())
				currentOverflowNode = parentNode;
			else
				currentOverflowNode = NULL;
		}
		else {
			BpNode* parentNode = new BpNode(treeOrder - 1); // empty Node
			parentNode->InsertElements(parentNewElem, true);
			leftNode->rightParent = parentNewElem;
			rightNode->leftParent = parentNewElem;
			if (currentOverflowNode == root)
				root = parentNode;

			currentOverflowNode = NULL;
		}
	} while (currentOverflowNode);
}

void BpTree::RemoveElemFromLeafNode(BpElement* targetElem, BpNode* const leafNode) {
	//remove leaf
	int oldFirstKey = leafNode->startElem->key;
	leafNode->RemoveElement(targetElem, true);
	delete targetElem;
	targetElem = NULL;
	if (leafNode->startElem && oldFirstKey != leafNode->startElem->key) {
		LeafChangeParentKey(leafNode, oldFirstKey);
	}
	//the minimum number of keys = floor(maxKey/2) = ceil(order/2) - 1
	if (!leafNode->IsHungry())
		return;
	// the strategy of lack keys :
	// 1. check if the count of left sibling node. If more than ceil(order/2)-1, steal a key.
	// 2. otherwise check if the count of right sibling node. If more than ceil(order/2)-1, steal a key.
	// 3. if there has not key can be stolen, merge with left node or right node. Select left node first.
	// 4. go to parent rebalance.
	//the minimum number of keys = floor(maxKey/2) = ceil(order/2) - 1
	BpNode* lackElemNode = leafNode;
	int lackElemNode_oldFirstKey = lackElemNode->startElem
		? lackElemNode->startElem->key
		: oldFirstKey;
	BpNode* siblingNode = NULL;
	if (lackElemNode->leftParent
		&& (siblingNode = lackElemNode->leftParent->leftNode)
		&& !siblingNode->InMinimum()) {
		//stealing a key from left sibling
		BpElement* removeElem = siblingNode->endElem;
		int siblingNode_oldFirstKey = siblingNode->startElem->key;
		siblingNode->RemoveElement(removeElem, true);
		if (lackElemNode->startElem)
			lackElemNode->InsertElementsToPrev(lackElemNode->startElem, removeElem, true);
		else
			lackElemNode->InsertElements(removeElem, true);
		if (lackElemNode->startElem && lackElemNode_oldFirstKey != lackElemNode->startElem->key) {
			LeafChangeParentKey(lackElemNode, lackElemNode_oldFirstKey);
		}
	}
	else if (lackElemNode->rightParent
		&& (siblingNode = lackElemNode->rightParent->rightNode)
		&& !siblingNode->InMinimum()) {
		//stealing a key from right sibling
		BpElement* removeElem = siblingNode->startElem;
		int siblingNode_oldFirstKey = siblingNode->startElem->key;
		siblingNode->RemoveElement(removeElem, true);
		if (lackElemNode->endElem)
			lackElemNode->InsertElementsToNext(lackElemNode->endElem, removeElem, true);
		else {
			lackElemNode->InsertElements(removeElem, true);
			if (lackElemNode->startElem && lackElemNode_oldFirstKey != lackElemNode->startElem->key) {
				LeafChangeParentKey(lackElemNode, lackElemNode_oldFirstKey);
			}
		}
		if (siblingNode->startElem && siblingNode_oldFirstKey != siblingNode->startElem->key) {
			LeafChangeParentKey(siblingNode, siblingNode_oldFirstKey);
		}
	}
	else if (lackElemNode->leftParent
		&& (siblingNode = lackElemNode->leftParent->leftNode)) {
		//merge with left sibling
		BpNode* removeNode = lackElemNode;
		BpNode* remainNode = siblingNode;
		int remainNode_oldFirstKey = siblingNode->startElem->key;
		BpElement* mergeLeftElem = remainNode->endElem;
		BpElement* mergeRightElem = removeNode->startElem;
		removeNode->ClearElements(false);
		if (mergeLeftElem)
			remainNode->InsertElementsToNext(mergeLeftElem, mergeRightElem, false);
		else
			remainNode->InsertElements(mergeRightElem, false);
		if (remainNode->startElem && remainNode_oldFirstKey != remainNode->startElem->key) {
			LeafChangeParentKey(remainNode, remainNode_oldFirstKey);
		}
		remainNode->next = removeNode->next;
		if (remainNode->next)
			remainNode->next->prev = remainNode;
		remainNode->length += removeNode->length;
		//remove parent key
		BpElement* removeElem = removeNode->leftParent;
		BpNode* targetNode = removeElem->thisNode;
		targetNode->RemoveElement(removeElem, true);
		delete removeElem;
		delete removeNode;
		if (targetNode == root && targetNode->IsEmpty()) {
			root = remainNode;
			delete targetNode;
		}
		else if (targetNode != root && targetNode->IsHungry())
			LackElemTreeNodeRebalance(targetNode);
	}
	else if (lackElemNode->rightParent
		&& (siblingNode = lackElemNode->rightParent->rightNode)) {
		//merge with right sibling
		BpNode* removeNode = siblingNode;
		BpNode* remainNode = lackElemNode;
		int remainNode_oldFirstKey = lackElemNode_oldFirstKey;
		BpElement* mergeLeftElem = remainNode->endElem;
		BpElement* mergeRightElem = removeNode->startElem;
		removeNode->ClearElements(false);
		if (mergeLeftElem)
			remainNode->InsertElementsToNext(mergeLeftElem, mergeRightElem, false);
		else
			remainNode->InsertElements(mergeRightElem, false);
		if (remainNode->startElem && remainNode_oldFirstKey != remainNode->startElem->key) {
			LeafChangeParentKey(remainNode, remainNode_oldFirstKey);
		}
		remainNode->next = removeNode->next;
		if (remainNode->next)
			remainNode->next->prev = remainNode;
		remainNode->length += removeNode->length;
		//remove parent key
		BpElement* removeElem = removeNode->leftParent;
		BpNode* targetNode = removeElem->thisNode;
		targetNode->RemoveElement(removeElem, true);
		delete removeElem;
		delete removeNode;
		if (targetNode == root && targetNode->IsEmpty()) {
			root = remainNode;
			delete targetNode;
		}
		else if (targetNode != root && targetNode->IsHungry())
			LackElemTreeNodeRebalance(targetNode);
	}
	else if (lackElemNode == root && lackElemNode->IsEmpty()) {
		root = NULL;
		delete lackElemNode;
	}
}

void BpTree::LackElemTreeNodeRebalance(BpNode* const lackElemNode) {
	if (!lackElemNode || !lackElemNode->IsHungry())
		return;
	// the strategy of lack keys :
	// 1. check if the count of left sibling node. If more than ceil(order/2)-1, steal a key from parent and parent steal a key from left sibling.
	// 2. otherwise check if the count of right sibling node. If more than ceil(order/2)-1, steal a key.
	// 3. if there has not key can be stolen, merge with left and parent node or right and parent node. Select left node first.
	// 4. go to parent rebalance.
	//the minimum number of keys = floor(maxKey/2) = ceil(order/2) - 1
	BpNode* currentLackElemNode = lackElemNode;
	BpNode* siblingNode = NULL;
	do {
		siblingNode = NULL;
		if (currentLackElemNode->leftParent
			&& (siblingNode = currentLackElemNode->leftParent->leftNode)
			&& !siblingNode->InMinimum()) {
			//stealing a key from parent and parent steals a key from left sibling
			BpElement* siblingElem = siblingNode->endElem;
			siblingNode->RemoveElement(siblingElem, true);
			BpElement* parentElem = currentLackElemNode->leftParent;
			BpElement* parentPrevElem = parentElem->prevElem;
			BpElement* parentNextElem = parentElem->nextElem;
			BpNode* parentNode = parentElem->thisNode;
			parentNode->RemoveElement(parentElem, true);
			//sibling become parent
			parentElem->rightNode = currentLackElemNode->startSubNode;
			parentElem->rightNode->leftParent = parentElem;
			parentElem->rightNode->rightParent = NULL;
			currentLackElemNode->startSubNode = siblingElem->rightNode;
			currentLackElemNode->startSubNode->leftParent = NULL;
			if (currentLackElemNode->startElem)
				currentLackElemNode->startElem->leftNode = currentLackElemNode->startSubNode;
			currentLackElemNode->startSubNode->rightParent = currentLackElemNode->startElem;
			siblingElem->rightNode = currentLackElemNode;
			siblingElem->rightNode->leftParent = siblingElem;
			if (parentPrevElem)
				parentNode->InsertElementsToNext(parentPrevElem, siblingElem, true);
			else if (parentNextElem)
				parentNode->InsertElementsToPrev(parentNextElem, siblingElem, true);
			else
				parentNode->InsertElements(siblingElem, true);
			currentLackElemNode->startElem
				? currentLackElemNode->InsertElementsToPrev(currentLackElemNode->startElem, parentElem, true)
				: currentLackElemNode->InsertElements(parentElem, true);

			currentLackElemNode = NULL;
		}
		else if (currentLackElemNode->rightParent
			&& (siblingNode = currentLackElemNode->rightParent->rightNode)
			&& !siblingNode->InMinimum()) {
			//stealing a key from parent and parent steals a key from right sibling
			BpElement* siblingElem = siblingNode->startElem;
			siblingNode->RemoveElement(siblingElem, true);
			BpElement* parentElem = currentLackElemNode->rightParent;
			BpElement* parentPrevElem = parentElem->prevElem;
			BpElement* parentNextElem = parentElem->nextElem;
			BpNode* parentNode = parentElem->thisNode;
			parentNode->RemoveElement(parentElem, true);
			//sibling become parent
			parentElem->rightNode = siblingNode->startSubNode;
			parentElem->rightNode->leftParent = parentElem;
			parentElem->rightNode->rightParent = NULL;
			siblingNode->startSubNode = siblingElem->rightNode;
			siblingNode->startSubNode->leftParent = NULL;
			if (siblingNode->startElem)
				siblingNode->startElem->leftNode = siblingNode->startSubNode;
			siblingNode->startSubNode->rightParent = siblingNode->startElem;
			siblingElem->rightNode = siblingNode;
			siblingElem->rightNode->leftParent = siblingElem;
			if (parentPrevElem)
				parentNode->InsertElementsToNext(parentPrevElem, siblingElem, true);
			else if (parentNextElem)
				parentNode->InsertElementsToPrev(parentNextElem, siblingElem, true);
			else
				parentNode->InsertElements(siblingElem, true);
			currentLackElemNode->endElem
				? currentLackElemNode->InsertElementsToNext(currentLackElemNode->endElem, parentElem, true)
				: currentLackElemNode->InsertElements(parentElem, true);

			currentLackElemNode = NULL;
		}
		else if (currentLackElemNode->leftParent
			&& (siblingNode = currentLackElemNode->leftParent->leftNode)) {
			//merge with left sibling and parent
			BpElement* parentElem = currentLackElemNode->leftParent;
			BpNode* parentNode = parentElem->thisNode;
			BpNode* removeNode = currentLackElemNode;
			BpNode* remainNode = siblingNode;
			parentElem->rightNode = removeNode->startSubNode;
			parentElem->rightNode->leftParent = parentElem;
			removeNode->startSubNode = NULL;
			BpElement* mergeLeftElem = remainNode->endElem;
			BpElement* mergeMiddleElem = parentElem;
			BpElement* mergeRightElem = removeNode->startElem;
			//merge nodes
			removeNode->ClearElements(false);
			parentNode->RemoveElement(parentElem, true);//remove parent key
			if (mergeLeftElem)
				remainNode->InsertElementsToNext(mergeLeftElem, mergeMiddleElem, false);
			else
				remainNode->InsertElements(mergeMiddleElem, false);
			if (mergeMiddleElem)
				remainNode->InsertElementsToNext(mergeMiddleElem, mergeRightElem, false);
			else
				remainNode->InsertElements(mergeRightElem, false);
			remainNode->length += removeNode->length + 1;
			delete removeNode;
			/*if (parentNode == root && parentNode->IsEmpty()) {
				root = remainNode;
				delete parentNode;
			}
			else if (parentNode != root && parentNode->IsHungry()) {
				LackElemTreeNodeRebalance(parentNode);
			}*/
			// change to iteration
			if (parentNode == root && parentNode->IsEmpty()) {
				root = remainNode;
				delete parentNode;

				currentLackElemNode = NULL;
			}
			else if (parentNode != root && parentNode->IsHungry())
				currentLackElemNode = parentNode;
			else
				currentLackElemNode = NULL;
		}
		else if (currentLackElemNode->rightParent
			&& (siblingNode = currentLackElemNode->rightParent->rightNode)) {
			//merge with right sibling and parent
			BpElement* parentElem = currentLackElemNode->rightParent;
			BpNode* parentNode = parentElem->thisNode;
			BpNode* removeNode = siblingNode;
			BpNode* remainNode = currentLackElemNode;
			parentElem->rightNode = removeNode->startSubNode;
			parentElem->rightNode->leftParent = parentElem;
			removeNode->startSubNode = NULL;
			BpElement* mergeLeftElem = remainNode->endElem;
			BpElement* mergeMiddleElem = parentElem;
			BpElement* mergeRightElem = removeNode->startElem;
			//merge nodes
			removeNode->ClearElements(false);
			parentNode->RemoveElement(parentElem, true);//remove parent key
			if (mergeLeftElem)
				remainNode->InsertElementsToNext(mergeLeftElem, mergeMiddleElem, false);
			else
				remainNode->InsertElements(mergeMiddleElem, false);
			if (mergeMiddleElem)
				remainNode->InsertElementsToNext(mergeMiddleElem, mergeRightElem, false);
			else
				remainNode->InsertElements(mergeRightElem, false);
			remainNode->length += removeNode->length + 1;
			delete removeNode;
			/*if (parentNode == root && parentNode->IsEmpty()) {
				root = remainNode;
				delete parentNode;
			}
			else if (parentNode != root && parentNode->IsHungry()) {
				LackElemTreeNodeRebalance(parentNode);
			}*/
			// change to iteration
			if (parentNode == root && parentNode->IsEmpty()) {
				root = remainNode;
				delete parentNode;

				currentLackElemNode = NULL;
			}
			else if (parentNode != root && parentNode->IsHungry())
				currentLackElemNode = parentNode;
			else
				currentLackElemNode = NULL;
		}
		else if (currentLackElemNode == root && currentLackElemNode->IsEmpty()) {
			root = NULL;
			delete currentLackElemNode;

			currentLackElemNode = NULL;
		}
		else
			currentLackElemNode = NULL;
	} while (currentLackElemNode);
}

BpElement* BpTree::SearchElement(const int key) {
	if (!root)
		return NULL;
	BpNode* currentNode = root;
	BpElement* currentElem = NULL;
	//go to leaf node
	while (currentNode) {
		currentElem = currentNode->GetElement(key);
		if (currentElem->key <= key)
			currentNode = currentElem->rightNode;
		else //if (currentElem->key > key)
			currentNode = currentElem->leftNode;
	}
	return currentElem && currentElem->key == key ? currentElem : NULL;
}

string* BpTree::SearchData(const int key) {
	BpElement* currentElem = SearchElement(key);
	return currentElem ? currentElem->data : NULL;
}

void BpTree::Insert(const int key, string* const data) {
	if (!root) {
		root = new BpNode(dataOrder - 1);
		root->InsertElements(new BpElement(root, key, data), true);
		return;
	}
	//go to leaf node
	BpNode* currentNode = root;
	BpElement* currentElem = NULL;
	while (currentNode) {
		currentElem = currentNode->GetElement(key);
		if (currentElem->key <= key)
			currentNode = currentElem->rightNode;
		else //if (currentElem->key > key)
			currentNode = currentElem->leftNode;
	}
	if (!currentElem)
		return;
	InsertElemToLeafNode(currentElem, currentElem->thisNode, key, data);
}

void BpTree::Insert(const int key, const string value) {
	Insert(key, new string(value));
}

bool BpTree::Remove(const int key) {
	if (!root)
		return false;
	//go to leaf node
	BpNode* currentNode = root;
	BpElement* currentElem = NULL;
	while (currentNode) {
		currentElem = currentNode->GetElement(key);
		if (currentElem->key <= key)
			currentNode = currentElem->rightNode;
		else //if (currentElem->key > key)
			currentNode = currentElem->leftNode;
	}
	//return if key is not equal.
	if (!currentElem || currentElem->key != key)
		return false;
	RemoveElemFromLeafNode(currentElem, currentElem->thisNode);
	return true;
}

// for debug
bool BpTree::HealthCheck(BpNode* const  current) {
	if (!current)
		return false;
	if (!(current->startElem && !current->startElem->prevElem))
		return false;
	if (!(current->endElem && !current->endElem->nextElem))
		return false;
	if (current->leftParent && current->leftParent->rightNode != current)
		return false;
	if (current->rightParent && current->rightParent->leftNode != current)
		return false;
	//tree node
	if (current->startSubNode) {
		if (current->prev || current->next)
			return false;
		if (current->maxLength != treeOrder - 1)
			return false;
		//overflow
		if (current->length > current->maxLength)
			return false;
		//hungry
		if (current != root && current->length < current->maxLength >> 1)
			return false;
		//empty
		if (current->length < 1)
			return false;
		if (!
			(current->startSubNode->leftParent == NULL
				&& current->startSubNode->rightParent == current->startElem
				&& current->startElem->leftNode == current->startSubNode)
			)
			return false;
		//check elements
		int currentCount = 0;
		//check start element
		BpElement* currElem = current->startElem;
		if (!currElem)
			return false;
		if (currElem->prevElem)
			return false;
		if (!
			(currElem->leftNode == current->startSubNode
				&& currElem->leftNode->rightParent == currElem
				&& currElem->leftNode->leftParent == NULL
				)
			)
			return false;
		//check middle elements
		while (currElem->nextElem) {
			if (currElem->key >= currElem->nextElem->key)
				return false;
			if (currElem->thisNode != current)
				return false;
			if (!currElem->leftNode || currElem->leftNode->rightParent != currElem)
				return false;
			if (!currElem->rightNode || currElem->rightNode->leftParent != currElem)
				return false;
			if (!
				(currElem->nextElem->prevElem == currElem
					&& currElem->rightNode->rightParent == currElem->nextElem
					&& currElem->nextElem->leftNode->leftParent == currElem
					)
				)
				return false;
			if (currElem->data)
				return false;
			//check if key is valid
			BpElement* tempElem = SearchElement(currElem->key);
			if (!tempElem || tempElem->key != currElem->key)
				return false;
			//check child
			if (!HealthCheck(currElem->leftNode))
				return false;
			currElem = currElem->nextElem;
			currentCount++;
		}
		//check the end element
		if (currElem != current->endElem)
			return false;
		if (currElem->thisNode != current)
			return false;
		if (!currElem->leftNode || currElem->leftNode->rightParent != currElem)
			return false;
		if (!currElem->rightNode || currElem->rightNode->leftParent != currElem)
			return false;
		if (!
			(currElem->rightNode->rightParent == NULL)
			)
			return false;
		if (currElem->data)
			return false;
		//check if key is valid
		BpElement* tempElem = SearchElement(currElem->key);
		if (!tempElem || tempElem->key != currElem->key)
			return false;
		//check child
		if (!HealthCheck(currElem->leftNode))
			return false;
		if (!HealthCheck(currElem->rightNode))
			return false;
		currentCount++;

		if (currentCount != current->length)
			return false;
		else
			return true;
	}
	//data node
	else {
		if (current->maxLength != dataOrder - 1)
			return false;
		//overflow
		if (current->length > current->maxLength)
			return false;
		//hungry
		if (current != root && current->length < current->maxLength >> 1)
			return false;
		//empty
		if (current->length < 1)
			return false;
		if (current->startSubNode)
			return false;
		//check elements
		int currentCount = 0;
		//check start element
		BpElement* currElem = current->startElem;
		if (!currElem)
			return false;
		if (currElem->prevElem)
			return false;
		if (currElem->leftNode)
			return false;
		//check middle elements
		while (currElem->nextElem) {
			if (currElem->key >= currElem->nextElem->key)
				return false;
			if (currElem->thisNode != current)
				return false;
			if (currElem->leftNode)
				return false;
			if (currElem->rightNode)
				return false;
			if (!
				(currElem->nextElem->prevElem == currElem)
				)
				return false;
			currElem = currElem->nextElem;
			currentCount++;
		}
		//check the end element
		if (currElem != current->endElem)
			return false;
		if (currElem->thisNode != current)
			return false;
		if (currElem->leftNode)
			return false;
		if (currElem->rightNode)
			return false;
		if (current->leftParent && current->leftParent->leftNode
			&& current->leftParent->leftNode != current->prev)
			return false;
		if (current->rightParent && current->rightParent->rightNode
			&& current->rightParent->rightNode != current->next)
			return false;
		if (current->prev && current->prev->next != current)
			return false;
		if (current->next && current->next->prev != current)
			return false;
		//check if the key same with parent key
		int tempKey = current->startElem->key;
		BpNode* tempNode = current;
		while (tempNode && !tempNode->leftParent && tempNode->rightParent) {
			tempNode = tempNode->rightParent->thisNode;
		}
		if (tempNode && tempNode->leftParent && tempNode->leftParent->key != tempKey)
			return false;
		currentCount++;
		if (currentCount != current->length)
			return false;
		else
			return true;
	}
}

bool BpTree::HealthCheck() {
	return !root
		|| (!root->leftParent && !root->rightParent
			&& HealthCheck(root)
			);
}
