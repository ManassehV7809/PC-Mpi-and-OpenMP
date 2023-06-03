#include <iostream>
#include <vector>
#include <limits>
#include <chrono>

using namespace std;

#define Infinity numeric_limits<int>::max()


void Dijkstra(const vector<vector<int>> &graph, int source, vector<int> &distance) {
    int numVerts = graph.size();
    vector<bool> visited(numVerts, false);

     // Set the distance of the source vertex to 0
    distance[source] = 0;

    // setting up costs of source's neighbours
    for (int v = 0; v < numVerts; ++v) {
        if (v != source) {
            if (graph[source][v] != Infinity) {
                distance[v] = graph[source][v]; // Set cost of neighbors with direct edge
            } else {
                distance[v] = Infinity; // Set cost of unreachable neighbors to infinity
            }
        }
    }

    // Main loop
    for (int i = 0; i < numVerts - 1; ++i) {
        int u = -1;
        int minDist = Infinity;

        // Find the vertex with the minimum distance among unvisited vertices
        for (int v = 0; v < numVerts; ++v) {
            if (!visited[v] && distance[v] < minDist) {
                minDist = distance[v];
                u = v;
            }
        }

        if (u == -1) {
            break;
        }

        visited[u] = true;

        // Relax edges connected to the selected vertex
        for (int v = 0; v < numVerts; ++v) {
            if (!visited[v] && graph[u][v] != Infinity) {
                int distThroughU = distance[u] + graph[u][v];
                distance[v] = min(distance[v], distThroughU);
            }
        }
    }
}


int main() {




    // Example graph
    vector<vector<int>> graph ={
        {0, 2, Infinity, 1},
        {2, 0, 1, Infinity},
        {Infinity, 1, 0, 1},
        {1, Infinity, 1, 0}
    };

    int numVertices = graph.size();
    int source = 0;  // Source vertex

    vector<int> distance(numVertices, Infinity);  // Resize the distance vector

 auto start = chrono::high_resolution_clock::now();
    Dijkstra(graph, source, distance);


 auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Shortest distances from source vertex " << source << ":" << endl;
    for (int i = 0; i < numVertices; i++)
        cout << "Vertex " << i << ": " << distance[i] << endl;
           cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}
