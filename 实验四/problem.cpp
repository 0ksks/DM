#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct Edge {  // define Edge structure
    int src, dest, weight;
};

bool compare_edges(const Edge &a, const Edge &b) {  // define comparison function for edge's weight
    return a.weight < b.weight;
}

class UnionFind {  // Disjoint-set
private:
    vector< int > parent, rank;

public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) {
            return;
        }
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
};

vector< Edge > kruskal(vector< Edge > &edges, int V) {
    sort(edges.begin(), edges.end(), compare_edges);  // ascend sort
    vector< Edge > result;
    UnionFind uf(V);
    for (int i = 0; i < edges.size(); i++) {
        int src = edges[i].src;
        int dest = edges[i].dest;
        if (uf.find(src) != uf.find(dest)) {
            result.push_back(edges[i]);
            uf.unite(src, dest);
        }
    }
    return result;
}

int main() {
    while (true) {
        string input;
        int V, E;
        cout << "Enter number of vertices and edges(enter 0 0 to quit):\n";
        getline(cin, input);
        stringstream ss(input);
        if (!(ss >> V >> E)) {
            cout << "Invalid input\n";
            continue;
        }
        char extra;
        if (ss >> extra) {
            cout << "Exist extra character\n";
            continue;
        }
        if (V == 0 && E == 0) {
            cout << "Thanks for using\n";
            break;
        }
        if (V < 0 || E < 0) {
            cout << "Invalid input\n";
            continue;
        }
        vector< Edge > edges(E);
        int i = 0;
        while (i < E) {
            printf("Enter source, destination, and weight for each edge(%d/%d):\n", i + 1, E);
            string input;
            getline(cin, input);
            stringstream ss(input);
            if (!(ss >> edges[i].src >> edges[i].dest >> edges[i].weight)) {
                cout << "Invalid input\n";
                continue;
            }
            char extra;
            if (ss >> extra) {
                cout << "Exist extra character\n";
                continue;
            }
            i++;
        }

        vector< Edge > minSpanningTree = kruskal(edges, V);

        cout << "Edges in Minimum Spanning Tree:\n";
        int totalWeight = 0;
        Edge edge;
        for (int i = 0; i < minSpanningTree.size(); i++) {
            edge = minSpanningTree[i];
            cout << edge.src << " - " << edge.dest << " : " << edge.weight << "\n";
            totalWeight += edge.weight;
        }
        cout << "Total weight of Minimum Spanning Tree: " << totalWeight << "\n";
    }

    return 0;
}
