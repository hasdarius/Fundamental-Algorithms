/**
 * @author Darius Haș
 * @group 30432
 *
 * Specificațiile problemei: DFS algorithm
 *
 * Interpretarea personală despre complexitate (timp și spațiu), despre cazurile de testare (favorabil,
 * mediu-statistic si nefavorabil):
 * The complexity of the DFS algorithm is O(V+E) as it can be seen on the charts.
 * The topological sorting can be done only on a DAG. If the graph has cycles => we can not topologically sort.
 * */

#include <iostream>
#include<list>
#include <algorithm>
#include<set>
#include <time.h>
#include <stack>
#include "Profiler.h"

using namespace std;
typedef enum colour {
    white, grey, black
} Colour;

Profiler profiler("Lab10");

typedef struct node {
    int value;
    Colour colour;
    int discoveryTime, finalTime;
    node *parent;
    list<int> children;
} Node;

typedef struct edge {
    int vertex1, vertex2;
} Edge;

void strongconnect(Node **vertices, Node *source, int time);


Node *newNode(int key,const char *name, int n) {
    Node *node = new Node;
    node->value = key;
    node->parent = NULL;
    node->colour = white;
    node->discoveryTime = -1;
    profiler.countOperation(name, n, 5);
    return node;
}

void DFS_VISIT(Node *vertices[], Node *source, int &time,bool Afis,const char* name,int n) {
    time++;
    source->discoveryTime = time;
    if(Afis==true)
        cout << source->value <<" ";
    source->colour = grey;
    profiler.countOperation(name, n, 2);
    for (auto it: source->children) {
        profiler.countOperation(name, n, 1);
        if (vertices[it]->colour == white) {
            vertices[it]->parent = source;
            profiler.countOperation(name, n, 1);
            DFS_VISIT(vertices, vertices[it], time,Afis,name,n);
        }
    }
    source->colour = black;
    time++;
    source->finalTime = time;
    profiler.countOperation(name, n, 2);

}


void DFS(Node *vertices[], int size,bool Afis,const char *name,int n) {
    int time = 0;
    for (int i = 0; i < size; i++)
        if (vertices[i]->colour == white) {
            DFS_VISIT(vertices, vertices[i], time,Afis,name,n);
        }

}

int strongConnect(Node *vertices[],  Node *source, int &time, int lowlink[],stack<Node*> &Stack,bool isInStack[],int &result) {
    time++;
    source->discoveryTime = time;
    lowlink[source->value] = time;
    Stack.push(source);
    isInStack[source->value]=true;
    for (auto it: source->children) {
        if (vertices[it]->discoveryTime == -1) {
            strongConnect(vertices, vertices[it], time, lowlink,  Stack,isInStack,result);
            lowlink[source->value] = min(lowlink[source->value], lowlink[vertices[it]->value]);
        }
        else if(isInStack[vertices[it]->value]==true){
            lowlink[source->value] = min(lowlink[source->value], vertices[it]->discoveryTime);
        }
    }

    if (lowlink[source->value] == source->discoveryTime) {
        result++;
        Node* node;
       while(Stack.top()->value!=source->value){
           node=Stack.top();
           cout<<node->value<<" ";
           isInStack[node->value]=false;
           Stack.pop();
       }
        node=Stack.top();
        cout<<node->value<<endl<<endl;
        isInStack[node->value]=false;
        Stack.pop();
    }
    return result;
}


int tarjan(Node *vertices[], int size) {
    int result;
    int lowlink[size];
    for(int i=0;i<size;i++)
        lowlink[i]=-1;
    stack<Node*> Stack;
    bool isInStack[size];
    int time = 0;
    for (int i = 0; i < size; i++)
        if (vertices[i]->discoveryTime == -1) {
            result=strongConnect(vertices, vertices[i], time, lowlink, Stack,isInStack,result);
        }
    return result;
}

bool compareByFinishingTime(Node* node1, Node* node2)
{
    return (node1->finalTime > node2->finalTime);
}

void TopologicalSort(Node* vertices[],int size,int DAGIndicator){
    if(DAGIndicator==size){
    cout<<"This is the order in which DFS traversed: "<<endl;
    DFS(vertices,size,1,"",size);
    cout<<endl<<endl;
    for(int i=0;i<size;i++)
        cout<<vertices[i]->value<<"(discoveryTime: "<<vertices[i]->discoveryTime<<", finalTime: "<<vertices[i]->finalTime<<")"<<endl;
    sort(vertices,vertices+size,compareByFinishingTime);
    cout<<endl<<"The topologically sorted order is: "<<endl;
    for(int i=size-1;i>=0;i--)
        cout<<vertices[i]->value<<"(finalTime: "<<vertices[i]->finalTime<<")  ";
    }
    else
        cout<<"We cannot topologically sort. The graph is not a DAG";
}


void generateGraphTarjan(int vertices, int edges, const char *name, int n) {
    pair<int, int> edge;
    set<pair<int, int>> existingEdges;
    int vertex1, vertex2;
    Node *vertex[vertices];
    for (int i = 0; i < vertices; i++) {
        vertex[i] = newNode(i,name,edges);
    }
    for (int i = 0; i < edges; i++) {
        do {
            vertex1 = rand() % vertices;
            vertex2 = rand() % vertices;
            edge = {vertex1, vertex2};
        } while (vertex1 == vertex2 || existingEdges.count(edge));
        existingEdges.insert(edge);
        vertex[vertex1]->children.push_back(vertex2);
    }
    for (int i = 0; i < vertices; i++)
        vertex[i]->children.sort();
    if (vertices < 50) {
        cout << "We will print the list of adjacency of each node:" << endl;
        for (int i = 0; i < vertices; i++) {
            cout << "The vertex with the value:" << vertex[i]->value;
            if (!vertex[i]->children.empty())
                cout << " has the neighbours: ";
            else
                cout << " doesn't have neighbours.";
            for (auto it:vertex[i]->children) {
                cout << it << " ";
            }
            cout << endl << endl;
        }
    }
    Node* vertexForTopologicalSort[vertices];
    for(int i=0;i<vertices;i++)
        vertexForTopologicalSort[i]=vertex[i];
    cout<<endl<<"The strongly connected components are: "<<endl;
    int DAGIndicator=tarjan(vertex, vertices);
    cout<<endl<<"TopologicalSorting: "<<endl;
    TopologicalSort(vertexForTopologicalSort,vertices,DAGIndicator);

}
void generateGraphDFS(int vertices, int edges, const char *name, int n) {
    pair<int, int> edge;
    set<pair<int, int>> existingEdges;
    int vertex1, vertex2;
    Node *vertex[vertices];
    for (int i = 0; i < vertices; i++) {
        vertex[i] = newNode(i,name,n);
    }
    for (int i = 0; i < edges; i++) {
        do {
            vertex1 = rand() % vertices;
            vertex2 = rand() % vertices;
            edge = {vertex1, vertex2};
        } while (vertex1 == vertex2 || existingEdges.count(edge));
        existingEdges.insert(edge);
        vertex[vertex1]->children.push_back(vertex2);
    }
    for (int i = 0; i < vertices; i++)
        vertex[i]->children.sort();
    if (vertices < 50) {
        cout << "We will print the list of adjacency of each node:" << endl;
        for (int i = 0; i < vertices; i++) {
            cout << "The vertex with the value:" << vertex[i]->value;
            if (!vertex[i]->children.empty())
                cout << " has the neighbours: ";
            else
                cout << " doesn't have neighbours.";
            for (auto it:vertex[i]->children) {
                cout << it << " ";
            }
            cout << endl << endl;
        }
    }
    DFS(vertex, vertices,0,name,n);


}

void ExemplifyCorrectness(){

    generateGraphTarjan(7, 42, "", 10);
}

int main() {
    profiler.disableCounters();
    srand(time(NULL));
   ExemplifyCorrectness();
    profiler.enableCounters();
    int vertices = 100, edges;
    for (edges = 1000; edges <= 5000; edges += 100)
        generateGraphDFS(vertices, edges, "ConstantVertices=100",edges);
    edges = 9000;
    for (vertices = 100; vertices <= 200; vertices += 10)
        generateGraphDFS(vertices, edges, "ConstantEdges=9000",vertices);
    profiler.showReport();
    return 0;
}