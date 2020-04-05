/**
 * @author Darius Haș
 * @group 30432
 *
 * Specificațiile problemei: Merke K ordered lists
 *
 * Interpretarea personală despre complexitate (timp și spațiu), despre cazurile de testare (favorabil,
 * mediu-statistic si nefavorabil):Average case when k is constant and n varies: we can see that the complexity is O(n log k) because we have to add n*k
 * elements(k is constant and we can treat it as such), and we do that by using a min heap.The cost to heapify is log k and at each step we insert an
 * element to the final list( in O(1) time because we add them in order) and we have to replace that element with either the next element from its list
 * or, if its list is empty, with the last element of the heap( both take O(1) time ), decreasing the size of the heap. After either of those two, we apply
 * heapify again (complexity log k at most because when we will decrease the size of the heap it will be even less).
 * Conclusion: we apply heapify to put the smallest element on the top of the heap and then into the final list n*k times,but k is a small constant => 
 * O(n log k) complexity.The charts are linear because k is the constant(so log k is as well) and n is changing, and O(n log k) is very close to a linear function
 * due to the fact that log k is constant.
 * Average case when n is constant and k varies: From the charts, we can see that the complexity is still O(n log k) by looking at the values obtained,
 * but the shape of the chart looks different. The values are increasing logarithmically because log k schanges while n stays the same. 
 * Conclusion: if n is variable and k is constant=> the charts are linear because n is the one that changes and log k remains the same.
 * If n is constant and k is variable=> the charts are logarithmic because here log k changes and n remains the same.
 */
typedef struct node {
	int list_index, key;
}Node;
#include <iostream>
#include <list>
#include<conio.h>
#include<time.h>
#include <stdlib.h>  
#include <vector>
#include "Profiler.h"

Profiler profiler("Lab4");
using namespace std;
#define MAX_VALUE 10000
list<int> generateRandomSortedList(int n,int range, list<int>List)
{
	list<int>::iterator it = List.begin();
	for (int i = 0; i < n; i++) {
		
		List.insert(it, rand() % range + 1);
		
	}

	List.sort();
	return List;
}

vector<list<int>> storeLists(int k, int n, int range, vector<list<int>>BigList)
{
	for (int i = 0; i < k; i++)
	{
		list<int>List;
		List=generateRandomSortedList(n/k,range, List);
		BigList.push_back(List);
	}
	return BigList;
}

void showList(list<int> l)
{
	list<int>::iterator it;
	cout << "(There are "<<l.size()<<" numbers in the list.)"<<endl;
	for (it = l.begin(); it != l.end(); ++it)
		cout << *it<<" ";
	cout << '\n';
}

void showListOfLists(vector<list<int>>l)
{
	for (auto it = l.begin(); it != l.end(); ++it)
		showList(*it);
	cout << '\n';
}



void heapifyBottomUp(Node arr[], int size,int n, int root, const char* name)
{
		int smallest = root;
		int leftChild = 2 * root + 1;
		int rigthChild = 2 * root + 2;
		
		if (leftChild < size && arr[leftChild].key < arr[smallest].key) {
			smallest = leftChild;
			profiler.countOperation(name, n, 1);
		}
		else if(leftChild < size)
			profiler.countOperation(name, n, 1);
		if (rigthChild < size && arr[rigthChild].key < arr[smallest].key) {
			smallest = rigthChild;
			profiler.countOperation(name, n, 1);
		}else if (rigthChild < size)
			profiler.countOperation(name, n, 1);
		if (smallest != root) {
			profiler.countOperation(name, n, 3);
			swap(arr[root], arr[smallest]);
			heapifyBottomUp(arr, size,  n, smallest, name);
		}
}

Node bottomUpBuildHeap(Node a[], int n,int k, const char* name)
{
	int i;
	for (i = n / 2; i >= 0; i--)
		heapifyBottomUp(a, n,k, i, name);
	return *a;
}

list<int> merge(list<int> &destination, list<int>l1, list<int>l2,int n,const char* name)
{
	while (!l1.empty() && !l2.empty())
	{
		profiler.countOperation(name, n, 1);
		if (*l1.begin() <= *l2.begin()) {
			destination.push_back(*l1.begin());
			l1.erase(l1.begin());
		}
		else {
			destination.push_back(*l2.begin());
			l2.erase(l2.begin());
		}
	}
	while (!l1.empty()) {
		destination.push_back(*l1.begin());
		l1.erase(l1.begin());
	}
	while (!l2.empty()) {
		destination.push_back(*l2.begin());
		l2.erase(l2.begin());
	}
	return destination;
}

Node initializeHeap(Node a[],int n, vector<list<int>>l,const char* name)
{
	for (int i=0;i!=l.size();i++)
	{
		list<int>::iterator it = l[i].begin();
		profiler.countOperation(name, n, 1);
		a[i].key = *it;
		a[i].list_index = i;
	}

	return *a;
}

void showHeap(Node a[], int k)
{
	for (int i = 0; i < k; i++)
		cout << "(" << a[i].key << "," << a[i].list_index << ")" << " ";
}

list<int> updateHeap(Node a[], int k,int n, vector<list<int>>l,const char* name)
{
	list<int> finalList;
	bottomUpBuildHeap(a, k,n, name);
	while (k > 2) {
		profiler.countOperation(name, n, 3);
		finalList.push_back(a[0].key);
		l[a[0].list_index].erase(l[a[0].list_index].begin());
		if (!l[a[0].list_index].empty())
		{
			profiler.countOperation(name, n, 1);
			a[0].key = *l[a[0].list_index].begin();
				heapifyBottomUp(a, k, n, 0, name);

		}
		else {

			a[0] = a[k - 1];
			k--;
			profiler.countOperation(name, n, 1);
				heapifyBottomUp(a, k, n, 0, name);
		}

	}
	finalList = merge(finalList, l[a[0].list_index], l[a[1].list_index],n,name);
	return finalList;
}

void exemplifyCorectness() 
{
	profiler.disableCounters();
	int n = 80;
	int k = 4;
	Node a[100];
	list<int> finalList;
	vector<list<int>>BigList;
	srand(time(NULL));
	BigList = storeLists(k, n,1000, BigList);
	cout << "These are 4 sorted lists:" << endl;
	showListOfLists(BigList);
	*a = initializeHeap(a, k*n, BigList, "OPERATIONS");
	finalList = updateHeap(a, k, k*n, BigList, "OPERATIONS");
	cout << "These are all the elements from those lists sorted in a single list: ";
	cout << endl;
	showList(finalList);
	profiler.enableCounters();
}

void generatecharts() {
	int k = 5,n;
	cout <<endl<< "Generating charts for k=5..." << endl;
	for (n = 100; n < 10000 ; n += 400) {
		Node a[100];
		list<int> finalList;
		vector<list<int>>BigList;
		for (int i = 0; i < 5; i++) {
			srand(time(NULL));
			BigList = storeLists(k, n, 50000, BigList);
			*a = initializeHeap(a, n, BigList, "OPERATIONS_K_5");
			finalList = updateHeap(a, k, n, BigList, "OPERATIONS_K_5");
			finalList.clear();
			BigList.clear();
		}
		profiler.divideOperation("OPERATIONS_K_5", n, 5);
		
	}
	k = 10;
	cout << endl << "Generating charts for k=10..." << endl;
	for (n = 100; n < 10000; n += 400) {
		Node a[100];
		list<int> finalList;
		vector<list<int>>BigList;
		for (int i = 0; i < 5; i++) {
			srand(time(NULL));
			BigList = storeLists(k, n, 50000, BigList);
			*a = initializeHeap(a, n, BigList, "OPERATIONS_K_10");
			finalList = updateHeap(a, k, n, BigList, "OPERATIONS_K_10");
			finalList.clear();
			BigList.clear();
		}
		profiler.divideOperation("OPERATIONS_K_10", n, 5);

	}
	k = 100;
	cout << endl << "Generating charts for k=100..." << endl;
	for (n = 100; n < 10000; n += 400) {
		Node a[100];
		list<int> finalList;
		vector<list<int>>BigList;
		for (int i = 0; i < 5; i++) {
			srand(time(NULL));
			BigList = storeLists(k, n, 50000, BigList);
			*a = initializeHeap(a, n, BigList, "OPERATIONS_K_100");
			finalList = updateHeap(a, k, n, BigList, "OPERATIONS_K_100");
			finalList.clear();
			BigList.clear();
		}
		profiler.divideOperation("OPERATIONS_K_100", n, 5);

	}
	profiler.createGroup("Merge K lists", "OPERATIONS_K_5", "OPERATIONS_K_10", "OPERATIONS_K_100");
	
}
void generateCharts2() {
	int n = 10000;
	cout << endl << "Generating charts for n fixed..." << endl;
	for (int k = 10; k <= 500; k += 10) {
		Node a[500];
		list<int> finalList;
		vector<list<int>>BigList;
		srand(time(NULL));
		BigList = storeLists(k, n, 50000, BigList);
		*a = initializeHeap(a, k, BigList, "OPERATIONS_N_FIXED");
		finalList = updateHeap(a, k, k, BigList, "OPERATIONS_N_FIXED");
	}
}
void mergeExemplification() {
	profiler.disableCounters();
	list<int> list1;
	list<int> list2;
	list<int> destination;
	list1=generateRandomSortedList(10,100, list1);
	list2=generateRandomSortedList(10,100, list2);
	cout << "This is the first list:" << endl;
	showList(list1);
	cout << "This is the second list:" << endl;
	showList(list2);
	destination=merge(destination, list1, list2,10,"");
	cout << "This is the final list:" << endl;
	showList(destination);
	profiler.enableCounters();
}

int main()
{
	exemplifyCorectness();
	cout << endl;
	cout << "Merge exemplification for two lists:" << endl;
	mergeExemplification();
	cout << endl;
	generatecharts();
	generateCharts2();
	profiler.showReport();
	
}