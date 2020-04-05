/**
 * @author Darius Haș
 * @group 30432
 *
 * Specificațiile problemei: Hash Tables with Open addressing and Quadratic Probing
 *
 * Interpretarea personală despre complexitate (timp și spațiu), despre cazurile de testare (favorabil,
 * mediu-statistic si nefavorabil):In the average case, Search has a complexity of O(1) and in the worst it has a complexity of O(n). As we can see from the 
 * tables generated, the average effort to search an item that exists in the hash Table grows with the filling factor. The same thing happens when searching
 * for an item that does not exist in the table(it is obviously bigger than the one when we search for an existing item), but here we can see a big increase 
 * when the filling factor approaches 1, with the maximum effort increasing along with the increase of the filling factor as well.
 * In both cases, the maximum effort increases more when the filling factor approaches 1, but the max effort for searching an element that does not exist 
 * is bigger than the one for searching an element that exists in the hash table, always.
 */

#include <iostream>
#include <string.h>
#include <conio.h>
#include "Profiler.h"
#include<iomanip>

Profiler profiler("Lab5");
#define PRIME 10007
using namespace std;

typedef struct {
	int id;
	char name[30];
} Entry;

int h(int k)
{
	return k % PRIME;
}

void initializeHash(Entry hash[])
{
	for (int i = 0; i < PRIME; i++)
	{
		hash[i].id = -1;
		strcpy_s(hash[i].name, "");
	}
}
int quadraticHash(int k, int i)
{
	return (h(k) + 9 * i + 9 * i * i)%PRIME;
}
int hashInsert(Entry hash[], Entry element) {
	int i = 0,j;
	do {
		j = quadraticHash(element.id, i);

		if (hash[j].id == -1) {
			hash[j] = element;
			return i;
		}
		else {
			if (hash[j].id == element.id) {
				return -1;
			}
			else
				i++;
		}
	} while (i < PRIME);
	cerr << "Hash table overflow"<<endl;
		return -1;
}

void displayHash(Entry hash[]) {
	int j = 0;
	for (int i = 0; i < PRIME; i++) {
		if (hash[i].id != -1)
		{
			cout << "Position: " << i << ". Element inserted: " << hash[i].name << endl;
			j++;
		}
	}
	cout << j << " elements in the hash table."<<endl;
}
void generateRandomElements(Entry hash[],int elements[],int found[],int notfound[],double fillingFactor) {
	
	int i=0,k=0,j=0,q=0,numberOfElements=fillingFactor*PRIME;
	while (i < numberOfElements + 1500) {
		Entry object;
		object.id = elements[k++];
		char x[26];
		_itoa_s(object.id, x, 10);
		strcpy_s(object.name, "name");
		strcat_s(object.name, x);
		if (i < numberOfElements) {
			int successInsert = hashInsert(hash, object);
			if (successInsert != -1)
			{
				i++;
				found[j++] = object.id;
			}
		}
		else {
			notfound[q++] = object.id;
			i++;
		}
	}
}



int hashSearch(Entry hash[], int k,int &operation) {
	int j, i = 0;
	operation = 0;
	do {
		j = quadraticHash(k, i);
		operation++;
		if (hash[j].id == k) {
			
			return j;
		}
		i++;
		
	} while (hash[j].id != -1 && i < PRIME);
	
	return 0;
}

void generateChart(float fillingFactor,int elements[]) {
	Entry a[PRIME];
	int itemsfound[PRIME], itemsnotfound[1500];
	int maxnotfound = 0, maxfound = 0;
	int max1;
	int total1;
	initializeHash(a);
	generateRandomElements(a,elements, itemsfound, itemsnotfound, fillingFactor);
	int nroffoundelements = PRIME * fillingFactor;
	max1 = total1 = 0;
	for (int j = 0; j < 5; j++) {
		random_shuffle(itemsfound, itemsfound + nroffoundelements);
		for (int i = 0; i < 1500; i++) {
			hashSearch(a, itemsfound[i], maxfound);
			if (max1 < maxfound)
				max1 = maxfound;
			total1 += maxfound;
		}
	}
	float average1 = (float)total1 / 1500 / 5;

	cout <<"  "<< fillingFactor << "\t" << "\t" << "\t" <<  max1 << "\t" << "\t" << "\t" << average1;
	int max2, total2;
	max2 = total2 = 0;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 1500; i++) {
			hashSearch(a, itemsnotfound[i], maxfound);
			if (max2 < maxfound)
				max2 = maxfound;
			total2 += maxfound;
		}
	}
	float average2 = (float)total2 / 1500 / 5;
	cout << "\t" << "\t" << "\t" <<  max2 << "\t" << "\t" << "\t" <<  average2;
	cout << endl;
}

void exemplifyCorectness() {
	int operation;
	Entry a[PRIME];
	initializeHash(a);
	int elements[5];
	FillRandomArray(elements, 5, 10007, 50000, true, 0);
	for (int i = 0; i < 5; i++) {
		Entry object;
		object.id = elements[i];
		char x[26];
		_itoa_s(object.id, x, 10);
		cout << x << endl;
		strcpy_s(object.name, "name");
		strcat_s(object.name, x);
		hashInsert(a, object);
		displayHash(a);
		cout << endl;
	}
		cout << endl;
		cout << "Now I am going to search some elements:" << endl<<endl;
		if (hashSearch(a, elements[3], operation) != 0)
			cout << "The element with the key=" << elements[3] << " is in the hash table at position:" << hashSearch(a, elements[3], operation);
		else
			cout << "The element with the key=" << elements[3] << " is not in the hash table.";
		cout << endl << endl;
		if (hashSearch(a, 7, operation) != 0)
			cout << "The element with the key=" << 7 << " is in the hash table at position:" << hashSearch(a, 7, operation);
		else
			cout << "The element with the key=" << 7 << " is not in the hash table.";
		cout << endl << endl;
		if (hashSearch(a, elements[1], operation) != 0)
			cout << "The element with the key=" << elements[1] << " is in the hash table at position:" << hashSearch(a, elements[1], operation);
		else
			cout << "The element with the key=" << elements[1] << " is not in the hash table.";
		cout << endl << endl;

}

int main()
{
	cout << "1)I am going to exemplify the search and Insert functions:" << endl;
	cout << endl << endl << "I am going to insert 5 elements and show the contents of the hash table at each step(only the ones that have elements in them):" << endl << endl;
	exemplifyCorectness();

	cout << "-----------------------------------------------------------" << endl <<endl<< "2) Now I am going to generate the table:" << endl << endl;
	int elements[2 * PRIME];
	cout <<"Filling Factor"<< "\t"<< "Max. Effort found" << "\t" << "Avg. Effort found" << "\t" << "Max. Effort not found " << "\t" << "Avg. Effort not found"<<endl;
	cout << setprecision(4);
	FillRandomArray(elements, PRIME+1500, 10, 50000, true, 0);
	generateChart(0.8,elements);
	generateChart(0.85, elements);
	generateChart(0.9, elements);
	generateChart(0.95, elements);
	generateChart(0.99, elements);

}