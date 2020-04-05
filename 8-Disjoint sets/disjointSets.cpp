/**
 * @author Darius Haș
 * @group 30432
 *
 * Specificațiile problemei: Disjoint Sets Operations
 *
 * Interpretarea personală despre complexitate (timp și spațiu), despre cazurile de testare (favorabil,
 * mediu-statistic si nefavorabil):
 * Evaluating the complexity of Disjoint Sets operations using Kruskal algorithm:
 * Make_Set: has a complexity of O(n), with a constant of multiplicity which is equal with 3.
 * Union: has a complexity of O(n log n) as we can see from the charts.It is applied only when the nodes of an edge belong to different sets(in that way we make sure that after
 * applying Kruskal algorithm we get a tree (we have no loops).
 * Find_Set: also has a complexity of O(n log n) but it makes the most operations out of the Disjoint set operation because it is applied twice for every edge in the graph.
 * Interpretations:
 * I used path compression so that the first time I apply Find_Set for a node, all the nodes that are traversed to get to the root will get as parent the root,
 * in that way reducing the number of operations done if we want to apply Find_Set to them.If I hadn't used path compression, the number of operations will increase for
 * Find_Set and also for Union_Set because there we make use of Find_Set because for each node, each time, we would have to go up through all the nodes to get to the root.
 * Union is using the rank, in that way it is more efficient to link two nodes. If we didn't have the rank, we would have to calculate the heights of both subtrees and compare
 * them in order to know which of the nodes will be the root in the resulting tree after the union which is more costly in terms of complexity.With rank, we simply make the
 * root the node that has the bigger rank, or if they are equal we choose either of them and increase its rank by 1.
 * My Kruskal algorithm stops when I found n-1 edges because the I know for sure that I obtained a Minimum Spanning Tree. If I hadn't stopped, Find_Union would have made more
 * operations.
 */
#include <iostream>
#include <list>
#include<algorithm>
#include<set>
#include "Profiler.h"

using namespace std;

Profiler profiler("Lab 8");

typedef struct node {
    int rank;
    int name;
    node *parent;
} Node;

typedef struct edge {
    int vertex1, vertex2;
    int weight;
} Edge;

Node *makeSet(int a, int n, const char *name) {
    Node *x = new Node;
    profiler.countOperation(name, n, 3);
    x->name = a;
    x->parent = x;
    x->rank = 0;
    return x;
}

void link(Node *x, Node *y, int n, const char *name) {
    if (x && y) {
        profiler.countOperation(name, n, 3);
        if (x->rank > y->rank)
            y->parent = x;
        else
            x->parent = y;
        if (x->rank == y->rank) {
            y->rank++;
            profiler.countOperation(name, n, 1);
        }
    }
}

Node *findSet(Node *a, int n, const char *name) {
    if (!a)
        return NULL;
    profiler.countOperation(name, n, 1);
    if (a->parent != a) {
        profiler.countOperation(name, n, 1);
        a->parent = findSet(a->parent, n, name);
    }
    return a->parent;
}

void unionSet(Node *a, Node *b, int n, const char *name) {

    if (a && b) {
        Node *find1, *find2;
        find1 = findSet(a, n, name);
        find2 = findSet(b, n, name);
        if (a && b && find1 != find2) {
            link(find1, find2, n, name);
        }
        if (a == b) {
            cout << " We can't unite an element with itself" << endl;
        }
    }
}

int partition(Edge *a[], int n, int first, int last, const char *name) {
    profiler.countOperation(name, n, 0);
    int x = a[last]->weight;
    profiler.countOperation(name, n, 1);

    int i = first - 1;
    for (int j = first; j <= last - 1; j++) {
        profiler.countOperation(name, n, 1);
        if (a[j]->weight < x) {
            profiler.countOperation(name, n, 3);
            i++;
            swap(a[i], a[j]);
        }

    }
    swap(a[i + 1], a[last]);
    profiler.countOperation(name, n, 3);
    return i + 1;

}

int randomPartition(Edge *a[], int n, int first, int last, const char *name) {
    int i = rand() % (last - first + 1) + first;
    profiler.countOperation(name, n, 3);
    swap(a[last], a[i]);
    return partition(a, n, first, last, name);
}


void quicksort(Edge *a[], int n, int first, int last, const char *name) {
    int q;
    profiler.countOperation(name, n, 0);
    if (first < last) {
        q = partition(a, n, first, last, name);
        quicksort(a, n, first, q - 1, name);
        quicksort(a, n, q + 1, last, name);
    }
}

void kruskalAlgorithm(Edge *a[], int n, Node *tree[]) {
    int k = 1;
    profiler.disableCounters();
    quicksort(a, n, 0, n - 1, "");
    if(n<100){
        cout<<"Edge-urile ordonate:"<<endl;
        for(int i=0;i<n;i++){
            cout<<a[i]->vertex1<<" "<<a[i]->vertex2<<" weight: "<<a[i]->weight<<endl;
        }
    }
    profiler.enableCounters();
    if(n<100)
        cout<<endl<<"The Nodes chosen by Kruskal are:"<<endl;
    for (int i = 0; i < n; i++) {
        if (findSet(tree[a[i]->vertex1], n / 4, "FindSetOperations") !=
            findSet(tree[a[i]->vertex2], n / 4, "FindSetOperations")) {
            if(n<100)
            {
                cout<<a[i]->vertex1<<" "<<a[i]->vertex2<<" weight: "<<a[i]->weight<<endl;
            }
            unionSet(tree[a[i]->vertex1], tree[a[i]->vertex2], n / 4, "UnionSetOperations");
            k++;
        }
        if (k == n / 4) break;
    }
}

void initializeEdges(Edge *edges[], int n) {
    for (int i = 0; i < n; i++) {
        edges[i] = new Edge;
        edges[i]->weight = 0;
    }
}

void generateGraph(int size) {
    pair<int, int> edge;
    set<pair<int, int>> existingEdges;
    int array2[4 * size];
    FillRandomArray(array2, 4 * size, 10, 4 * size, 0, 0);
    int i = 0;
    Edge *edges[4 * size];
    initializeEdges(edges, 4 * size);
    for (int i = 0; i < size; i++) {
        edges[i]->weight = array2[i];
        do {

            edges[i]->vertex1 = i;
            edges[i]->vertex2 = rand() % size;
            if (edges[i]->vertex1 > edges[i]->vertex2)
                swap(edges[i]->vertex1, edges[i]->vertex2);
            edge = {edges[i]->vertex1, edges[i]->vertex2};
        } while (existingEdges.count(edge) || edges[i]->vertex1 == edges[i]->vertex2);
        existingEdges.insert(edge);
    }
    for (int i = size; i < 4 * size; i++) {
        edges[i]->weight = array2[i];
        do {

            edges[i]->vertex1 = rand() % size;
            edges[i]->vertex2 = rand() % size;
            if (edges[i]->vertex1 > edges[i]->vertex2)
                swap(edges[i]->vertex1, edges[i]->vertex2);
            edge = {edges[i]->vertex1, edges[i]->vertex2};
        } while (existingEdges.count(edge) || edges[i]->vertex1 == edges[i]->vertex2);
        existingEdges.insert(edge);
    }
    if(size<100){
        for(int i=0;i<4*size;i++){
            cout<<edges[i]->vertex1<<" "<<edges[i]->vertex2<<" weight: "<<edges[i]->weight<<endl;
        }
    }
    Node *tree[size];
    for (int i = 0; i < size; i++)
        tree[i] = makeSet(i, size, "MakeSetOperations");
    kruskalAlgorithm(edges, 4 * size, tree);
}

void printOneSet(Node *node, Node *nodes[], int size) {
    set<Node *> setOfEdges;
    Node *parent = findSet(node, size, "");
    for (int i = 0; i < size; i++) {
        if (findSet(nodes[i], size, "") == parent) {
            if (!setOfEdges.count(nodes[i]))
                setOfEdges.insert(nodes[i]);

        }
    }
    for (auto i: setOfEdges) {
        cout << i->name << " (rank: " << i->rank << ")" << endl;
    }
}

void exemplifyCorectness() {
    Node *nodes[10];
    for (int i = 0; i < 10; i++)
        nodes[i] = makeSet(i, 10, "");
    cout << "We created 10 sets:" << endl;
    for (int i = 0; i < 10; i++)
        cout << nodes[i]->name << " with rank: " << nodes[i]->rank << endl;
    cout << endl << endl << "Now we are going to apply Union to 5 random pairs of elements and Find_Set to 5 elements:" << endl<<endl;
    int arrayForUnion[10];
    int arrayForPrint[5];
    FillRandomArray(arrayForUnion, 10, 0, 9, 0, 0);
    FillRandomArray(arrayForPrint, 5, 0, 9, 1, 0);
    for (int i = 0; i < 5; i++) {
        cout<<i+1<<") ";
        cout << "We are going to apply union to the elements with the keys: " << arrayForUnion[2 * i] << " "
             << arrayForUnion[2 * i + 1]<<endl;
        unionSet(nodes[arrayForUnion[2 * i]], nodes[arrayForUnion[2 * i + 1]], 10, "");
        Node *node;
        cout<<endl<<"We are going to apply Find_Set to the element with the  key: "<< arrayForPrint[i]<<endl;
        cout << endl << "The element with the key: " << nodes[arrayForPrint[i]]->name << " (rank: " << nodes[arrayForPrint[i]]->rank << ")"
             << " belongs to the set that has the representative with the key: ";
        node = findSet(nodes[arrayForPrint[i]], 10, "");
        cout << node->name << " (rank: " << node->rank << ")"<<endl;
        cout << endl << "We are going to print the set of " << arrayForPrint[i] << ":" << endl;
        printOneSet(nodes[arrayForPrint[i]], nodes, 10);
        cout<<endl<<endl;
    }

}

int main() {
    profiler.disableCounters();
    exemplifyCorectness();
    cout<<endl<<"Demo for Kruskal";
    cout << endl;
    cout << "Now we are going to generate some charts:" << endl;
    generateGraph(10);
    profiler.enableCounters();
    for (int i = 100; i <= 10000; i += 100) {
        cout << i << ")" << endl;
        generateGraph(i);
    }
    profiler.createGroup("DisjointSets", "MakeSetOperations", "FindSetOperations", "UnionSetOperations");
    profiler.showReport();

    return 0;
}