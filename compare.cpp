#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include <chrono>

using namespace std;

#define Infinity numeric_limits<int>::max()

void DijkstraOpenMP(const vector<vector<int>>& graph, int source, vector<int>& distance) {
    int numVerts = graph.size();
    vector<bool> visited(numVerts, false);

    distance[source] = 0;

    for (int i = 0; i < numVerts - 1; ++i) {
        int u = -1;
        int minDist = Infinity;

        // Find the vertex with the minimum distance among unvisited vertices
        #pragma omp parallel for
        for (int v = 0; v < numVerts; ++v) {
            if (!visited[v] && distance[v] < minDist) {
                #pragma omp critical
                {
                    if (distance[v] < minDist) {
                        minDist = distance[v];
                        u = v;
                    }
                }
            }
        }

        if (u == -1) {
            break;
        }

        visited[u] = true;

        // Relax edges connected to the selected vertex
        #pragma omp parallel for
        for (int v = 0; v < numVerts; ++v) {
            if (!visited[v] && graph[u][v] != Infinity) {
                int distThroughU = distance[u] + graph[u][v];
                #pragma omp critical
                {
                    distance[v] = min(distance[v], distThroughU);
                }
            }
        }
    }
}

void DijkstraChrono(const vector<vector<int>>& graph, int source, vector<int>& distance) {
    int numVerts = graph.size();
    vector<bool> visited(numVerts, false);

    distance[source] = 0;

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
    vector<vector<int>> graph = {
        {0, 2, Infinity, 1},
        {2, 0, 1, Infinity},
        {Infinity, 1, 0, 1},
        {1, Infinity, 1, 0}
    };

    int numVertices = graph.size();
    int source = 0; // Source vertex

    vector<int> distance(numVertices, Infinity); // Resize the distance vector

    // Dijkstra's algorithm with OpenMP
    long long timeTakenomp;
    {
        vector<int> distanceOpenMP(distance); // Create a copy of distance vector

        auto start = chrono::high_resolution_clock::now();
        DijkstraOpenMP(graph, source, distanceOpenMP);
        auto end = chrono::high_resolution_clock::now();
  timeTakenomp = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        cout << "Shortest distances from source vertex " << source << " (OpenMP):" << endl;
        for (int i = 0; i < numVertices; i++)
            cout << "Vertex " << i << ": " << distanceOpenMP[i] << endl;
    }
 long long timeTakenChrono;
    // Dijkstra's algorithm with chrono
    {
        vector<int> distanceChrono(distance); // Create a copy of distance vector

        auto start = chrono::high_resolution_clock::now();
        DijkstraChrono(graph, source, distanceChrono);
        auto end = chrono::high_resolution_clock::now();

    timeTakenChrono = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        cout << "Shortest distances from source vertex " << source << " (chrono):" << endl;
        for (int i = 0; i < numVertices; i++)
            cout << "Vertex " << i << ": " << distanceChrono[i] << endl;
    }
float speedUp=timeTakenomp /float(timeTakenChrono);
    cout << "Execution time (OpenMP): " << timeTakenChrono << " milliseconds" << endl;
    cout << "Execution time (chrono): " << timeTakenomp<< " milliseconds" << endl;
      cout << "Speedup: " <<speedUp << " milliseconds" << endl;

    return 0;
}
