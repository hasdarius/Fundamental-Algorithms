/**
 * @author Darius Haș
 * @group 30432
 *
 * Specificațiile problemei: BFS algorithm
 *
 * Interpretarea personală despre complexitate (timp și spațiu), despre cazurile de testare (favorabil,
 * mediu-statistic si nefavorabil):
 * The complexity of the BFS algorithm is O(V+E) as it can be seen on the charts. It has this complexity because for each graph it goes through all the nodes(each node will
 * be enqueued and dequeued from the queue and it will have its parent and color changed, operations that are O(1) for a node) =>O(n) for all nodes. Moreover,at each node
 * it searches in the adjacency list( meaning that we iterate through it's incident edges) a node that has not yet been reached at, so actually it goes through all the edges
 * also(given the fact that every node is reached at one point or another)=>O(E).In total they would make O(V+E).
 * If we have a dense graph (we have few vertices and a large number of edges) the most operations will be done by searching for neighbours in the adjacency list of a node=>
 * we go a lot through the edges and the complexity seems that it would be O(E).
 * If we have a sparse graph( we have few edges and a very big number of nodes) the operations done in the adjacency list (looking for edges, and for nodes that haven't been reached yet)
 * will be very small in comparison to the ones done on the nodes(enqueueing and dequeueing them from the queue,changing the parent and the colour) so we would be inclined to say that the complexity is O(V).
 * Because of these aspects presented below, the complexity of BFS is O(V+E) and that is why the complexity depends on both of them, because we don't know which is bigger.
 */



#include <iostream>
#include<queue>
#include<list>
#include<vector>
#include <algorithm>
#include<set>
#include "Profiler.h"

using namespace std;
Profiler profiler("Lab 9");
typedef enum colour {
    white, grey, black
} Colour;
#define infinity 1000000000

typedef struct node {
    int value;
    Colour colour;
    node *parent;
    int distance;
    list<int> children;
} Node;

typedef struct edge {
    int vertex1, vertex2;
} Edge;

Node *newNode(int key, const char *name, int n) {
    Node *node = new Node;
    node->colour = white;
    node->value = key;
    node->distance = infinity;
    node->parent = NULL;
    profiler.countOperation(name, n, 3);
    return node;
}

vector<Node *> BFS(Node *vertices[], int size, Node *source, vector<Node *> resultingtree,const char* name,int n) {
    queue<int> queueOfVertices;
    source->colour = grey;
    source->parent = NULL;
    source->distance = 0;
    profiler.countOperation(name, n, 3);
    queueOfVertices.push(source->value);

    while (!queueOfVertices.empty()) {
        int a = vertices[queueOfVertices.front()]->value;
        profiler.countOperation(name, n, 1);
        queueOfVertices.pop();
        for (auto i:vertices[a]->children) {
            profiler.countOperation(name, n, 1);
            if (vertices[i]->colour == white) {
                profiler.countOperation(name, n, 3);
                vertices[i]->colour = grey;
                vertices[i]->distance = vertices[a]->distance + 1;
                vertices[i]->parent = vertices[a];
                queueOfVertices.push(i);
            }
        }
        vertices[a]->colour = black;
        profiler.countOperation(name, n, 1);
        resultingtree.push_back(vertices[a]);
    }
    for (int i = 0; i < size; i++)
        if (vertices[i]->colour == white) {
            queue<int> queueOfVertices;
            vertices[i]->colour = grey;
            vertices[i]->parent = NULL;
            vertices[i]->distance = 0;
            profiler.countOperation(name, n, 3);
            queueOfVertices.push(vertices[i]->value);

            while (!queueOfVertices.empty()) {
                int a = vertices[queueOfVertices.front()]->value;
                profiler.countOperation(name, n, 1);
                queueOfVertices.pop();
                for (auto i:vertices[a]->children) {
                    profiler.countOperation(name, n, 1);
                    if (vertices[i]->colour == white) {

                        vertices[i]->colour = grey;
                        vertices[i]->distance = vertices[a]->distance + 1;
                        vertices[i]->parent = vertices[a];
                        profiler.countOperation(name, n, 3);
                        queueOfVertices.push(i);
                    }
                }
                vertices[a]->colour = black;
                profiler.countOperation(name, n, 1);
                resultingtree.push_back(vertices[a]);
            }
        }
    return resultingtree;
}

void prettyPrint(Node *root, int space, vector<Node *> forestOfBST) {
    if (root == NULL)
        return;
    for (int i = 0; i < space; i++)
        cout << " ";
    cout << root->value << "(distance: " << root->distance << ")";
    for (auto it:forestOfBST) {
        if (it->parent == root) {
            cout << endl;
            prettyPrint(it, space + 10, forestOfBST);
        }
    }
}

void forest(vector<Node *> forestOfBFS) {
    for (auto it:forestOfBFS) {
        if (it->distance == 0) {
            cout << "This is a BST:" << endl;
            prettyPrint(it, 0, forestOfBFS);
            cout << endl;
        }
    }

}

void generateGraph(int vertices, int edges, const char *name,int n) {
    pair<int, int> edge1, edge2;
    set<pair<int, int>> existingEdges;
    int vertex1, vertex2;
    Node *vertex[vertices];
    for (int i = 0; i < vertices; i++) {
        vertex[i] = newNode(i, name, n);

    }
    srand(time(NULL));
    for (int i = 0; i < edges; i++) {
        do {
            vertex1 = rand() % vertices;
            vertex2 = rand() % vertices;
            edge1 = {vertex1, vertex2};
            edge2 = {vertex2, vertex1};
        } while (vertex1 == vertex2 || existingEdges.count(edge1) || existingEdges.count(edge2));
        existingEdges.insert(edge1);
        existingEdges.insert(edge2);
        vertex[vertex1]->children.push_back(vertex2);
        vertex[vertex2]->children.push_back(vertex1);
    }

    for (int i = 0; i < vertices; i++)
        vertex[i]->children.sort();
    if (vertices < 50) {
        cout<<"We will print the list of adjacency of each node:"<<endl;
        for (int i = 0; i < vertices; i++) {
            cout << "The vertex with the value:" << vertex[i]->value;
            if (!vertex[i]->children.empty())
                cout << " has the neighbours: ";
            else
                cout << " doesn't have neighbours.";
            for (auto it:vertex[i]->children) {
                cout << it << " ";
            }
            cout << endl<<endl;
        }
    }
    vector<Node *> result;
    result = BFS(vertex, vertices, vertex[0], result,name,n);

    if (vertices < 50) {
        forest(result);
    }

}





int main() {
    profiler.disableCounters();
    generateGraph(5, 8, "",5);
    profiler.enableCounters();
    int vertices = 100, edges;
    for (edges = 1000; edges <= 4900; edges += 100)
        generateGraph(vertices, edges, "ConstantVertices=100",edges);
    edges = 9000;
    for (vertices = 200; vertices <= 300; vertices += 10)
        generateGraph(vertices, edges, "ConstantEdges=9000",vertices);
    profiler.showReport();

    return 0;
}