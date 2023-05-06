#include "DataSearchTree.h"

using namespace DataSearchTree;

BpElement::BpElement(BpNode* thisNode, const int key, string* const data) {
	BpElement::key = key;
	BpElement::data = data;

	BpElement::prevElem = NULL;
	BpElement::nextElem = NULL;

	BpElement::thisNode = thisNode;
	BpElement::leftNode = NULL;
	BpElement::rightNode = NULL;
}

BpElement::BpElement(BpNode* thisNode, const int key, BpNode* const leftNode, BpNode* const rightNode) {
	BpElement::key = key;
	BpElement::data = NULL;

	BpElement::prevElem = NULL;
	BpElement::nextElem = NULL;

	BpElement::thisNode = thisNode;
	BpElement::leftNode = leftNode;
	BpElement::rightNode = rightNode;
}

BpElement::~BpElement() {
	delete data;

	data = NULL;
	prevElem = NULL;
	nextElem = NULL;
	thisNode = NULL;
	leftNode = NULL;
	rightNode = NULL;
}

//data
int BpElement::GetKey() {
	return key;
}

string* BpElement::GetData() {
	return data;
}

string BpElement::GetValue() {
	return *data;
}

void BpElement::SetValue(string value) {
	*data = value;
}
