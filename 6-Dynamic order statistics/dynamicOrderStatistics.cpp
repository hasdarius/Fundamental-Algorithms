
/**
 * @author Darius Haș
 * @group 30432
 *
 * Specificațiile problemei: OS-Operations
 *
 * Interpretarea personală despre complexitate (timp și spațiu), despre cazurile de testare (favorabil,
 * mediu-statistic si nefavorabil):
 * BuildTree: We build a PBT from an ordered array. The complexity is O(n log n) because we have to insert n keys in the BST and insertion's complexity
 * is O(log n).
 * SELECT-OS: This function finds the node with the i-th smallest value. If the node's value is equal with the root's, we return it, if it is smaller we search in 
 * the left subtree and if it is bigger we search in the right subtree=> complexity of searching for the element is O(log n) (log n being the height of
 * the tree). From that, it follows that if we were to search for n elements the complexity would be O(n log n) as it is shown in the charts.
 * DELETE-OS: We delete the i-th smallest element in the tree. We have three cases:
 * 1) The element is a leaf=> we simply delete it
 * 2) The element has at most 1 child. In this case, we simply make its child the child of its parent and delete the node.
 * 3) The element has 2 children. In this case, we find the predecessor, we copy its content to our node and we will delete the predecessor node(being
 * its predecessor we know that it has at most one child). 
 * No matter which case, to make a deletion it takes O(log n). So, if we were to delete n elements the complexity would be O(n log n) as it is shown in the
 * charts.
 * To mantain the size of each node: When we insert an element we update its size.
 * When we delete an element, we simply decrement the size of every node that we traversed until we got to that node.
 */

#include <iostream>
#include"Profiler.h"
using namespace std;

Profiler profiler("Lab6");
#define MAX_VALUE 10000

typedef struct node{
	int value,size;
	struct node* left;
	struct node* parent;
	struct node* right;
}Node;

Node* newNode(int number){

	Node* node = new Node;
	if (node != NULL) {
		node->value = number;
		node->left = NULL;
		node->parent = NULL;
		node->right = NULL;
		node->size = 1;
	}
	return node;

}
Node* buildBST(int first, int last, int array[MAX_VALUE],int n,const char* name) {
	
	if (first > last) { 
		//root->size = 1;
		return NULL; 
	}
	int middle = (first + last) / 2;
	Node* root=newNode(array[middle]);
	profiler.countOperation(name, n, 2);
	root->left=buildBST(first, middle - 1,array,n,name);
	root->right=buildBST(middle + 1, last,array,n,name );
	if (root->left) {
		root->left->parent = root;
		root->size += root->left->size;
		profiler.countOperation(name, n, 3);
	}
	if (root->right){
			root->right->parent = root;
			root->size += root->right->size;
			profiler.countOperation(name, n, 3);
	}
	return root;
}

Node* select(Node * root,int n, int i,const char *name) {
	
	int r;
	profiler.countOperation(name, n, 1);
	if (root) {
		if (root->left != NULL) {
			r = root->left->size + 1;
			profiler.countOperation(name, n, 1);
		}
		else
			r = 1;
		if (i == r)
			return root;
		else if (i < r)
			return select(root->left, n, i, name);
		else
			return select(root->right, n, i - r, name);
	}
}

void updateSize(Node* start, Node* finish,int n,const char* name) {
	Node* traverse = start;
	while (traverse->value != finish->value) {
		profiler.countOperation(name, n, 1);
		traverse->size--;
		if (traverse->value > finish->value)
			traverse = traverse->left;
		else
			traverse=traverse->right;
	}
}
void show(Node* root) {
	if(root->parent)
		cout << root->value << " " << root->size <<"\t"<<root->parent->value<< endl;
	else
		cout << root->value << " " << root->size << endl;
	if(root->left!=NULL)show(root->left);
	if (root->right != NULL)show(root->right);
}
Node* deleteNode(Node* root,int n, int value,const char *name) {
	if (value <= root->size) {
		Node* toDelete = select(root,n, value,name);
		Node* parent = toDelete->parent;
		profiler.countOperation(name, n, 1);
		if (toDelete->size == 1)
		{
			updateSize(root, toDelete,n,name);
			profiler.countOperation(name, n, 1);
			if (parent == NULL)
				root = NULL;
			else if (parent->left == toDelete)
				parent->left = NULL;
			else
				parent->right = NULL;
			return toDelete;
		}
		else if(toDelete->left&&!toDelete->right|| toDelete->right && !toDelete->left)
		{
			updateSize(root, toDelete,n,name);
			profiler.countOperation(name, n, 1);
			if (parent == NULL)
				if (toDelete->left)
				{
					root = toDelete->left;
					root->parent = parent;
				}
				else
				{
					root = toDelete->right;
					root->parent = parent;
				}
			else if (parent->left == toDelete)
				if (toDelete->left)
				{
					profiler.countOperation(name, n, 2);
					toDelete->left->parent = parent;
					parent->left = toDelete->left;

				}
				else
				{
					profiler.countOperation(name, n, 2);
					toDelete->right->parent = parent;
					parent->left = toDelete->right;
				}
				else
				if (toDelete->left)
				{
					profiler.countOperation(name, n, 2);
					toDelete->left->parent = parent;
					parent->right = toDelete->left;

				}
				else
				{
					profiler.countOperation(name, n, 2);
					toDelete->right->parent = parent;
					parent->right = toDelete->right;
				}
			return toDelete;
		}
			else
		{

			Node* toReplace = select(root,n, value-1,name);
			updateSize(root, toReplace,n,name);
			toDelete->value=toReplace->value;
			profiler.countOperation(name, n, 1);
			Node* parent = toReplace->parent;
			if (toReplace->size == 1)
			{
				profiler.countOperation(name, n, 1);
				if (parent->left == toReplace)
					parent->left = NULL;
				else
					parent->right = NULL;
				free(toReplace);
			}
			else{
				if (parent->left == toReplace)
					if (toReplace->left)
					{
						profiler.countOperation(name, n, 1);
						toReplace->left->parent = parent;
						parent->left = toReplace->left;

					}
					else
					{
						profiler.countOperation(name, n, 1);
						toReplace->right->parent = parent;
						parent->left = toReplace->right;
					}
				else
					if (toReplace->left)
					{
						profiler.countOperation(name, n, 1);
						toReplace->left->parent = parent;
						parent->right = toReplace->left;

					}
					else
					{
						profiler.countOperation(name, n, 1);
						toReplace->right->parent = parent;
						parent->right = toReplace->right;
					}
				return toReplace;
			}
		}
	}

}

void prettyPrint(Node* root, int space)
{

	if (root == NULL)
		return;

	space += 10;

	prettyPrint(root->right, space);
	cout << endl;
	for (int i = 10; i < space; i++)
		cout << " ";
	cout << root->value<<"/"<<root->size << "\n";

	prettyPrint(root->left, space);
}

void exemplifyCorrectness() {
	int array[11];
	FillRandomArray(array, 11, 10, 30, TRUE, 1);
	Node* root = newNode(0);
	profiler.disableCounters();
	root = buildBST(0, 10, array,11,"");
	cout<<"This is the initial BST(value/size):"<<endl;
	prettyPrint(root, 0);
	
	for (int i = 0; i < 3; i++)
	{
		int randomSelect = rand() % root->size + 1;
		Node* node = select(root, 11, randomSelect, "");
		cout <<i+1<<") "<< "The "<<randomSelect<<"-th smallest number is: " << node->value <<" and now we are going to delete it." << endl;
		deleteNode(root, 11, randomSelect, "");
		prettyPrint(root, 0);
	}
	profiler.enableCounters();
}

void generateCharts() {
	for (int n = 100; n <= 10000; n += 100) {
			cout << n << ")" << endl;
			for (int i = 0; i < 5; i++) {
				int array[100000];
				FillRandomArray(array, n, 10, 20000, TRUE, 1);
				Node* root = newNode(0);
				root = buildBST(0, n - 1, array,n,"BUILD-OS");
				int k = 1;
				while (k != n)
				{
					int randomSelect = rand() % root->size + 1;
					Node* node = select(root, n, randomSelect,"SELECT-OS" );
					deleteNode(root, n, randomSelect,"DELETE-OS" );
					k++;
				}
			}
			profiler.divideOperation("DELETE-OS", n, 5);
			profiler.divideOperation("SELECT-OS", n, 5);
			profiler.divideOperation("BUILD-OS", n, 5);

		
	}
	profiler.createGroup("OS-Operations","BUILD-OS", "DELETE-OS", "SELECT-OS");
	profiler.showReport();
}


int main()
{
	exemplifyCorrectness();
	generateCharts();
}
