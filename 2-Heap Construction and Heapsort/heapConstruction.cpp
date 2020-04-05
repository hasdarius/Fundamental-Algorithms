/**
 * @author Darius Haș
 * @group 30432
 *
 * Specificațiile problemei, ex: Comparison between the Bottom Up and Top down building Heap aproaches
 *
 * Interpretarea personală despre complexitate (timp și spațiu), despre cazurile de testare (favorabil,
 * mediu-statistic si nefavorabil):
 * n-size of the array
 * Bottom Up approach: we start from the n/2 index of the array (the elements of the array after this index are childs=>are heaps of size 1)
 * we attempt to heapify at each step, starting from the premise that we have an element that "links" two heaps and we want to merge them into one heap
 * until we reach the top. 
 * Top Down approach: We insert one element at a time into the array, comparing at each step recursively the element with its parent in order to
 * find its place in the heap(if the heap condition is not satisfied we interchange them)
 * In the average case, the Bottom Up, at first sight, seems to have a complexity of O(nlog n) beacuse we apply heapify to n/2 nodes(those that are not leaves)  
 * which has in its worst case O(log n) complexity (we have to go until the last level of the complete binary tree which is log n) but if we observe in the charts
 * and if we look closely, we can see that actually the complexity is O(n) because: for the parents that have the leaves as childs heapify has 0(1) 
 * complexity and for half of the remaining elements it takes only 2 steps to heapify and so on so forth(we are working with small heaps: half of them
 * are of height 0,a quarter are of height 1, 1/8 of height 3,...). By considering this approach until we reach the top we can see
 * that as we advance in levels of the heap the number of elements is divided by 2 and the number of steps to heapify only increases by 1. So our 
 * complexity would be t(n)=n/2*0(for leaves)
						   +n/(2^2)*1(for the parents of the leaves)
						   +...
						   +n/(2^ln n)*O(height-1)
 * and if we apply a little math we are going to see that in fact the complexity of Bottom Up building heap approach is O(n).
 * For the Top Down approach we can see that the complexity is O(nlog n) because we insert into position one element at a time. The "insertion" takes
*  O(log n) as complexity as an upper bound because we always add the element at the last position available in the array and we
* increase the size of the array at each step, so if we add a value that needs to be raised up to the top, we do that in O(log n) -log n is the 
* height of the heap- and we do it for n times(we add each element, one at the time=>O(n logn).
* To sum up, Bottom Up approach is more efficient because we work a lot with heaps of small height and as the number of operations needed to heapify 
* increases, the nr of nodes on which we have to apply heapify is divided by 2. On the other hand, in the Top Down approach, for the big values especially, it takes
* a significant number of operations to put into the right place(we put them at the last position available and we need to raise them up).
* EXTRA:
* In the worst case: It seems that Bottom Up has a complexity of O(nlog n) (it is not a big difference in he worst case from the average case due to the
* fact that we work a lot with small heaps that don't have a big complexity) and is much more efficient that Top Down which has a complexity of O(n^2) 
* because at each step, every element has to be raised until the top(the root changes at each step).
 */
#include <iostream>
#include<conio.h>
#include "Profiler.h"
#define MAX_SIZE 10000
using namespace std;
Profiler profiler("Lab2");
int getParentIndex(int x)
{
	if (x % 2 == 0)
		return x / 2 - 1;
	else
		return x / 2;
}
void heapifyBottomUp(int a[],int n, int index,const char *name)
{
	profiler.countOperation(name, n, 0);
	int max_value, aux;
	if ((2 * index+1) < n && a[2 * index+1] > a[index] )
	{
		max_value = 2 * index+1;
		profiler.countOperation(name, n, 1);
	}
	else {
		if(2*index+1 < n)
			profiler.countOperation(name, n, 1);
		max_value = index;
	}
		
	if ((2 * index + 2) < n && a[2 * index + 2] > a[max_value] )
	{
		max_value = 2 * index + 2;
		profiler.countOperation(name, n, 1);
	}
	if (2 * index+2 < n)
		profiler.countOperation(name, n, 1);
	if (max_value != index)
	{
		profiler.countOperation(name, n, 3);
		swap(a[index], a[max_value]);
		heapifyBottomUp(a,n, max_value, name);
	}
		
}

void bottomUpBuildHeap(int a[], int n, const char *name)
{
	int i;
	for (i = n / 2; i >= 0; i--)
		heapifyBottomUp(a, n, i,name);
}

void heapifyTopDown(int a[], int i, int n, const char* name)
{
	while (i > 0  && a[i] > a[getParentIndex(i)])
	{
		profiler.countOperation(name, n, 4);
		swap(a[i], a[getParentIndex(i)]);
		i = getParentIndex(i);
	}
	if (i > 0)
		profiler.countOperation(name, n, 1);
}

void topDownBuildHeap(int a[], int &i,int n, int value,const char *name)
{
	a[i++] = value;
	profiler.countOperation(name, n, 1);
	heapifyTopDown(a, i-1,n,name);
}
void heapSort(int a[], int n,const char *name)
{
	for (int i = n-1; i > 0; i--)
	{

		int aux;
		aux = a[i];
		a[i] = a[0];
		a[0] = aux;
		heapifyBottomUp(a, --n, 0, name);
	}
	
}
int main()
{
	int n,i, a[MAX_SIZE],v[MAX_SIZE],size;
	profiler.disableCounters();
	FillRandomArray(a, 10, 0, 100, true, 0);
	cout << "Here we have a random array:" << endl;
	for (i = 0; i < 10; i++)
		cout << a[i] << " ";
	cout << endl;
	cout << "Here we have a heap that was formed from that array with a Bottom Up approach:"<<endl;
	bottomUpBuildHeap(a, 10, "");

	for (i = 0; i < 10; i++)
		cout << a[i] << " ";
	cout << endl;
	cout << "Here we have the exemplification for HeapSort for that Heap we just created:" << endl;
	heapSort(a, 10, "");
	for (i = 0; i < 10; i++)
		cout << a[i] << " ";
	cout << endl<<endl;

	FillRandomArray(a, 10, 0, 100, true, 0
	);
	cout << "Here we have another random array:" << endl;
	for (i = 0; i < 10; i++)
		cout << a[i] << " ";
	cout << endl;
	cout << "Here we have a heap that was formed from that array with a Top Down approach:"<<endl;
	size = 0;
	for(i=0;i<10;i++)
		topDownBuildHeap(v,size,10,a[i], "");
	for (i = 0; i < 10; i++)
		cout << v[i] << " ";
	cout << endl;

	profiler.enableCounters();
	cout << endl << "Now we are going to generate some charts:" << endl;
	
	for (n = 100; n <= 10000; n += 100)
	{
		cout << n << ")" << endl;
		for (int i = 0; i <= 4; i++)
		{
			int source1[MAX_SIZE];
			FillRandomArray(a, n, 10, 10000, false, 0);
			for (int j = 0; j < n; j++)
				v[j] = a[j];
			bottomUpBuildHeap(v, n, "AverageBottomUpBuildHeap");
			size = 0;
			for (int j = 0; j < n; j++)
				topDownBuildHeap(source1, size,n, a[j], "AverageTopDownBuildHeap");
		}
		profiler.divideOperation("AverageBottomUpBuildHeap",n, 5);
		profiler.divideOperation("AverageTopDownBuildHeap", n, 5);
		FillRandomArray(a, n, 10, 10000, false, 1);
		for (int j = 0; j < n; j++)
			v[j] = a[j];
		bottomUpBuildHeap(v, n, "WorstBottomUpBuildHeap");
		size = 0;
		int source2[MAX_SIZE];
		for (int j = 0; j < n; j++)
			topDownBuildHeap(source2, size,n, a[j], "WorstTopDownBuildHeap");
		
	}
			profiler.createGroup("AverageOperations", "AverageBottomUpBuildHeap", "AverageTopDownBuildHeap");
			profiler.createGroup("WorstOperations", "WorstBottomUpBuildHeap", "WorstTopDownBuildHeap");
			profiler.showReport();
		//_getch();
		return 0;
}


