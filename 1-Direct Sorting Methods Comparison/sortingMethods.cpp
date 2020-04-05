/**
 * @author Darius Haș
 * @group 30432
 *
 * Specificațiile problemei, ex: Comparați metodele de sortare BubbleSort, InsertionSort și SelectionSort
 *
 * Interpretarea personală despre complexitate (timp și spațiu), despre cazurile de testare (favorabil,
 * mediu-statistic si nefavorabil):
 * n-size of the array
 * array[]- este array-ul in care sunt stocate elementele
 * 1)Best Case Scenario: Ca numar de assignments toate cele 3 metode de sortare au O(1)(algoritmul fiind deja sortat corespunzator),
 * iar BubbleSort si InsertionSort au O(n) numar de comparari:
 * -BubbleSort compara numerele vecine doua cate doua, iar in momentul in care nu a fost facuta
 * nicio asignare si s-a ajuns la finalul array-ului se opreste; algoritmul fiind sortat=> no assignments deci vom avea doar n-1 comparari
 * (stim ca atunci s-a parcurs tot array-ul negasindu-se nicio asignare de facut)
 * -InsertionSort porneste cu premisa ca avand un index care ia valori de la 0 la n se compara toate elementele aflate la stanga array[index], 
 * iar in cazul in care este gasit un numar mai mare, cu ajutorul unui auxiliar se vor modifica valorile
 * la fiecare pas se va face doar o comparare, in Best case scenario conditia de iesire fiind indeplinita prima data, de fiecare data,
*  mai putin pentru array[0], acesta nefiind comparat(este primul element) => n-1 comparari (O(n))
 * -SelectionSort:la fiecare pas se fac n-index (indexul pornind de la 1) comparari =>vom avea n(n-1)/2 comparari => O(n^2)
 * Concluzie: in Best Case BubbleSort si InsertionSort sunt mai eficiente decat SelectionSort datorita faptului ca indiferent de array,
*  vom avea n(n-1)/2 comparari iar la cele doua doar n-1, acestea fiind la fel de eficiente intre ele
  2)Worst Case Scenario: 
* -SelectionSort are acelasi numar de comparatii ca si la best case(din acelasi motiv => O(n^2)comparari), si are O(n) assignments
* la fiecare pas fiind facute doar 3 asignari
* -InsertionSort: parcurzand cu un index de la 0 la n, la fiecare pas vom avea index+1 assignments => O(n^2) asa cum este vazut si in chart-ul creat
* iar la fiecare pas vom avea index-1 comparari (vom compara cu elementele din stanga, iar conditia de iesire va fi indeplinita la ultimul 
* pas de fiecare data => O(n^2) comparisons.
* -BubbleSort- are cel mai mare numar de asignari, fiind O(n^2) de asemenea, iar un numar de comparari de n(n-1) comparari deci tot O(n^2).
Concluzie: dupa analiza chart-ului, BubbleSort este cel mai neeficient in Worst Case, numarul de assignments fiind mult mai mare decat al celorlalti
* 2 algoritmi, iar numarul de comparari fiind dublu, toate trei avand un total de O(n^2) , SelectionSort fiind cel mai eficient in acest caz
* 3)Average case scenario: In average case, din punct de vedere al assignments-urilor, SelectionSort are O(n) assignments,
* iar BubbleSort este cel mai neeficient, deoarece pentru orice n, valoarea assignmenturilor depaseste InsertionSort, care la randul sau depaseste SelectionSort .
* Din punct de vedere al compararilor, Bubble pare sa fie neeficient datorita numarului mare de comparari facute la fiecare pas( O(n^2)),
* SelectionSort are de asemenea un numar de O(n^2) comparari(indiferent de caz, la vom avea n(n-1)/2 comparari),
* iar Insertion are cel mai mic numar de comparari, dupa cum se poate observa din chart-uri.
*Concluzie:In average case InsertionSort se arata a fi cel mai eficient(cu putin mai eficient decat Selection, dupa rezultatele din chart), 
* iar Bubble este cel mai neeficient.
* STABILITY:
* BubbleSort este un algoritm stabil datorita conditiei impuse de strict mai mare, astfel niciodata nu se va intampla ca atunci cand vom avea numere
* egale intr-un array sa se schimbe ordinea acestora ( 5(1) 5(2)=> 5(1) 5(2)), la fel se intampla si la InsertionSort unde avem de asemenea o conditie
* de strict mai mare, fiind cu totul similar cu BubbleSort din punct de vedere al stabilitatii.
* La SelectionSort nu putem spune nimic despre stabilitatea acestuia cu certitudine, pentru ca avem cazuri in care pare a fi si cazuri in care nu pare 
* a fi stabil:
* Exemple:
* pentru n=3
* 1)array-ul este: 5(1) 5(2) 3 => dupa sortare: 3 5(2) 5(1) => pare a nu fi stabil
* 2)array-ul este: 5(1) 3 5(2) => dupa sortare: 3 5(1) 5(2) => pare a fi stabil
* Concluzie: BubbleSort si InsertionSort sunt stabile, iar despre SelectionSort nu putem spune nimic din punct de vedere al stabilitatii
 */

#include <iostream>
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include <time.h>  
#include <string>
#include "Profiler.h" 
#define MAX_SIZE 10000

using namespace std;
Profiler profiler("Lab1");


void bubbleSort(int n, int a[], const char * assignments, const char*  comparisons, const char* sum)
{
	profiler.countOperation(assignments, n, 0);
	int i, j, aux;
	bool nrofchanges = true;
	while (nrofchanges != false)
	{
		nrofchanges = false;
		for (i = 0; i < n-1; i++)
		{
			j = i + 1;
			profiler.countOperation(comparisons, n);
			if (a[i] > a[j])
			{
				nrofchanges = true;
				profiler.countOperation(assignments, n, 3);
				profiler.countOperation(sum, n, 4);
				aux = a[j];
				a[j] = a[i];
				a[i] = aux;
			}
			else
				profiler.countOperation(sum, n);
		}
	}
	
}

void insertionSort(int n, int a[], const char* assignments, const char* comparisons, const char* sum)
{
	//nu is bune comparisons
	profiler.countOperation(assignments, n,0);
	int i, j,aux;
	for (i = 1; i < n; i++)
	{
		aux = a[i];
		j = i - 1;
		while (j>-1 && a[j] > aux)
		{
			profiler.countOperation(comparisons, n);
			profiler.countOperation(assignments, n);
			profiler.countOperation(sum, n, 2);
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = aux;
		profiler.countOperation(assignments, n);
		profiler.countOperation(sum, n);
		if (j != -1)
		{
			profiler.countOperation(comparisons, n);
			profiler.countOperation(sum, n);
		}
		if(j+1==i)  //that means that we assigned the item on its exact position so we have to decrement the assignments
		{ 
			profiler.countOperation(assignments, n,-1);
			profiler.countOperation(sum, n, -1);
		}

		
	}
}

void selectionSort(int n, int a[], const char* assignments, const char* comparisons, const char* sum)
{
	profiler.countOperation(assignments, n, 0);
	int i, j, pos,aux;
	for (i = 0; i < n - 1; i++)
	{
		pos = i;
		for (j = i + 1; j < n; j++)
		{
			profiler.countOperation(comparisons, n);
			profiler.countOperation(sum, n);
			if (a[j] < a[pos])
				pos = j;
		}
				
		if (pos != i)
		{
			profiler.countOperation(assignments, n,3);
			profiler.countOperation(sum, n,3);
			aux = a[i];
			a[i] = a[pos];
			a[pos] = aux;
		}
	}
}


int main()
{
	int n,i,a[MAX_SIZE],v[MAX_SIZE];
	profiler.disableCounters();
	cout << "Exemplify corectness: "<<endl;
		FillRandomArray(a, 5, 0, 10000, true, 0);
		cout << "\An array before BubbleSort:\n";
		for (int i = 0; i < 5; i++)
			cout << a[i] << " ";
		bubbleSort(5, a, "", "", "");
		cout << "\nThe array after BubbleSort:\n";
		for (int i = 0; i < 5; i++)
			cout << a[i] << " ";
		cout << endl;
		FillRandomArray(a, 5, 0, 10000, true, 0);
		cout << "\nAn array before SelectionSort:\n";
		for (int i = 0; i < 5; i++)
			cout << a[i] << " ";
		selectionSort(5, a, "", "", "");
		cout << "\nThe array after SelectionSort:\n";
		for (int i = 0; i < 5; i++)
			cout << a[i] << " ";
		cout << endl;
		FillRandomArray(a, 5, 0, 10000, true, 0);
		cout << "\nThe array before InsertionSort:\n";
		for (int i = 0; i < 5; i++)
			cout << a[i] << " ";
		insertionSort(5, a, "", "", "");
		cout << "\nThe array after InsertionSort:\n";
		for (int i = 0; i < 5; i++)
			cout << a[i] << " ";
		cout << endl;
		cout << endl;
		profiler.enableCounters();
		cout << "Now we are going to generate some charts..."<<endl;
	for (n = 100; n <= 10000; n += 500)
	{
		FillRandomArray(a, n, 0, 10000, true, 2);
		for (int i = 0; i < n; i++)
			v[i] = a[i];
		bubbleSort(n, v,"WorstAssignmentsBubble","WorstComparisonsBubble","WorstSumBubble");
		for (int i = 0; i < n; i++)
			v[i] = a[i];
		insertionSort(n, v, "WorstAssignmentsInsertion", "WorstComparisonsInsertion", "WorstSumInsertion");
		for (int i = 0; i < n; i++)
			v[i] = a[i];
		cout << n << ") ";
		selectionSort(n, v, "WorstAssignmentsSelection", "WorstComparisonsSelection", "WorstSumSelection");
		cout << endl << endl;
		FillRandomArray(a, n, 0, 10000, true, 1);
		for (int i = 0; i < n; i++)
			v[i] = a[i];
		bubbleSort(n, v,"BestAssignmentsBubble", "BestComparisonsBubble", "BestSumBubble");
		for (int i = 0; i < n; i++)
			v[i] = a[i];
		insertionSort(n, v, "BestAssignmentsInsertion", "BestComparisonsInsertion", "BestSumInsertion");
		for (int i = 0; i < n; i++)
			v[i] = a[i];
		selectionSort(n, v, "BestAssignmentsSelection", "BestComparisonsSelection", "BestSumSelection");
		for (i = 0; i <= 4; i++)
		{
			FillRandomArray(a, n, 0, 10000, true, 0);
			for (int i = 0; i < n; i++)
				v[i] = a[i];
			bubbleSort(n, v, "AverageAssignmentsBubble", "AverageComparisonsBubble", "AverageSumBubble");
			for (int i = 0; i < n; i++)
				v[i] = a[i];
			insertionSort(n, v, "AverageAssignmentsInsertion", "AverageComparisonsInsertion", "AverageSumInsertion");
			for (int i = 0; i < n; i++)
				v[i] = a[i];
			selectionSort(n, v, "AverageAssignmentsSelection", "AverageComparisonsSelection", "AverageSumSelection");
			
		}
		profiler.divideOperation("AverageAssignmentsBubble", n, 5);
		profiler.divideOperation("AverageComparisonsBubble", n, 5);
		profiler.divideOperation("AverageSumBubble", n, 5);
		profiler.divideOperation("AverageAssignmentsInsertion", n, 5);
		profiler.divideOperation("AverageComparisonsInsertion", n, 5);
		profiler.divideOperation("AverageSumInsertion", n, 5);
		profiler.divideOperation("AverageAssignmentsSelection", n, 5);
		profiler.divideOperation("AverageComparisonsSelection", n, 5);
		profiler.divideOperation("AverageSumSelection", n, 5);
	}
	/*
	*for the average case i made an additional  function called divideOperation which is very similar to the countOperation and put it in the profiler because the profiler i have doesn't have such an operation
	*so after i applied the sort algorithms to 5 different random arrays
	*i divided the results with 5 so that i get an average
	*this is the function:
	void divideOperation(const char* name, int size, int divider)
	{
		if (countersDisabled) return;
		try {
			opcountMap[name][size] /= divider;
		}
		catch (...) {
			OpcountMap::iterator it1 = opcountMap.find(name);
			if (it1 == opcountMap.end()) {
				opcountMap[name] = OpcountSequence();
			}
			OpcountSequence::iterator it2 = opcountMap[name].find(size);
			if (it2 == opcountMap[name].end()) {
				opcountMap[name][size] = 0;
			}
			opcountMap[name][size] /= divider;
		}

	}
	*/
	profiler.createGroup("WorstCaseAssignments", "WorstAssignmentsBubble", "WorstAssignmentsInsertion", "WorstAssignmentsSelection");
	profiler.createGroup("WorstCaseComparisons", "WorstComparisonsBubble", "WorstComparisonsInsertion", "WorstComparisonsSelection");
	profiler.createGroup("WorstCaseTotal", "WorstSumBubble", "WorstSumInsertion", "WorstSumSelection");
	
	profiler.createGroup("BestCaseAssignments", "BestAssignmentsBubble", "BestAssignmentsInsertion", "BestAssignmentsSelection");
	profiler.createGroup("BestCaseComparisons", "BestComparisonsBubble", "BestComparisonsInsertion", "BestComparisonsSelection");
	profiler.createGroup("BestCaseTotal", "BestSumBubble", "BestSumInsertion", "BestSumSelection");

	profiler.createGroup("AverageCaseAssignments", "AverageAssignmentsBubble", "AverageAssignmentsInsertion", "AverageAssignmentsSelection");
	profiler.createGroup("AverageCaseComparisons", "AverageComparisonsBubble", "AverageComparisonsInsertion", "AverageComparisonsSelection");
	profiler.createGroup("AverageCaseTotal", "AverageSumBubble", "AverageSumInsertion", "AverageSumSelection");

	profiler.showReport();

	

}


