/**
 * @author Darius Haș
 * @group 30432
 *
 * Problem: Muti-way trees
 *
 * For R2 transformation: I used an array of nodes. Each node contains a field which represents its value and a vector that represents its children.
 * For R3 transformation: I used nodes that have as fields the value, the first child and the brother, the last two being nodes of the same type.
 * R2 and R3 both run in O(n) time as it is shown by using the profiler. It is necessary to use additional memory in order to make the transformations O(n).
 * In the first case, I used an array of nodes as additional memory( each node representing a possible parent). In that way, we simply traverse once the parent
 * vector and we add the children to their corresponding parent=>O(n).
 * In the second case, we used an auxiliary node so that we could correctly add the brother of a node (we take two nodes and we simply traverse all the children of a 
 * node and we assign the brother to each one. For example, if 1 4 9 are children of the node 2, the aux will take the value 1(the first child) and the brother will
 * be 4. Then we simply go on=> aux will now be 4 and brother will be 9 and if we had more nodes we would continue to do that until the end in that way assuring
 * that each node has been assigned the brother correctly. In this transformation we also use the array of nodes used in the first transformation in order to take the
 * children or brother/s of each node, this being the way in which we keep the complexity O(n).
 */
#include <iostream>
#include<list>
#include<vector>
#include "Profiler.h"

using namespace std;

#define MAX_NR 10000

Profiler profiler("Lab7");

typedef struct node {
	int value;
	vector<int> children;
}Node;

typedef struct node2 {
	int value;
	node2* firstChild;
	node2* brother;
}NodeBin;

Node* newNode(int value)
{
	Node* x = new Node;
	x->value = value;
	return x;
}

NodeBin* newNodeBin(int value) {
	NodeBin* x = new NodeBin;
	x->value = value;
	x->brother = NULL;
	x->firstChild = NULL;
	return x;
}

void initializeVector(Node* array[], int n, const char* name) {
	for (int i = 0; i < n; i++)
	{
		Node* node = newNode(i);
		profiler.countOperation(name, n);
		array[i] = node;
	}
}


NodeBin* firstTransformation(int array[], int n, Node* alreadyExists[],const char* name) {
	NodeBin* root = newNodeBin(0);
	for (int i = 0; i < n; i++) {
		profiler.countOperation(name, n);
		if (array[i] != -1) {
			alreadyExists[array[i]]->children.push_back(i + 1);
		}
		else
			root->value = i + 1;
	
	}
	return root;
}
void printFirstTransformation(Node* alreadyExist[],int n)
{
	for (int i = 0; i < n; i++) {
		if (!alreadyExist[i]->children.empty()) {
			cout << "The parent with the key: " << alreadyExist[i]->value;
			cout << " has the children: ";
			for (auto j = alreadyExist[i]->children.begin(); j != alreadyExist[i]->children.end(); j++)
				cout << *j << " ";
			cout << endl;
		}
	}
}
NodeBin* secondTransformation(Node* alreadyExist[], NodeBin* root,const char* name)
{
	if (!alreadyExist[root->value]->children.empty()) {
		NodeBin* firstChild = newNodeBin(alreadyExist[root->value]->children.front());
		NodeBin* aux = new NodeBin;
		aux = firstChild;
		root->firstChild = aux;
		profiler.countOperation(name, 9,2);
		alreadyExist[root->value]->children.erase(alreadyExist[root->value]->children.begin());
		while(!alreadyExist[root->value]->children.empty() ){
			NodeBin* brother = newNodeBin(alreadyExist[root->value]->children.front());
			aux->brother = brother;
			profiler.countOperation(name, 9,2);
			aux = brother;
			alreadyExist[root->value]->children.erase(alreadyExist[root->value]->children.begin());
		}
		if(root->firstChild)
			secondTransformation(alreadyExist, root->firstChild,name);
		if(root->brother)
			secondTransformation(alreadyExist, root->brother,name);
	}
	return root;
}

void prettyPrint(NodeBin* root,int space)
{
	cout << endl;
	for (int i = 0; i < space; i++)
		cout << " ";
	cout << root->value;
	if (root->firstChild) {
		
		prettyPrint(root->firstChild,space+10);
	}
	if (root->brother) {

		prettyPrint(root->brother,space);
	}
}
int main()
{
	NodeBin* root = newNodeBin(0);
	int array[9] = { 2,7,5,2,7,7,-1,5,2 };
	Node* alreadyExist[9];
	cout << "The father vector is: { 2,7,5,2,7,7,-1,5,2 }"<<endl;
	cout << "After the first transformation we have:" << endl;
	initializeVector(alreadyExist, 9, "First Transformation");
	root=firstTransformation(array, 9, alreadyExist,"First Transformation");
	profiler.countOperation("Second Transformation", 9);
	printFirstTransformation(alreadyExist, 9);
	cout << " The root is: " << root->value;
	root=secondTransformation(alreadyExist, root,"Second Transformation");
	profiler.createGroup("Multi-way", "First Transformation", "Second Transformation");
	profiler.showReport();
	cout << endl<<endl<<"Here is the pretty print:"<<endl;
	prettyPrint(root,10);
}
