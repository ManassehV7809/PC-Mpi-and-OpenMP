#include <iostream>
#include <vector>
#include <limits>

using namespace std;

#define INF numeric_limits<int>::max()

void Dijkstra(const vector<vector<int>>& graph, int source, vector<int>& distance) {
    int numVertices = graph.size();

    vector<bool> visited(numVertices, false);
    distance.resize(numVertices, INF);

    distance[source] = 0;

    for (int i = 0; i < numVertices - 1; i++) {
        int u = -1;
        int minDist = INF;

        // Find the vertex with the minimum distance among unvisited vertices
        for (int v = 0; v < numVertices; v++) {
            if (!visited[v] && distance[v] < minDist) {
                minDist = distance[v];
                u = v;
            }
        }

        if (u == -1)
            break;

        visited[u] = true;

        // Relax edges connected to the selected vertex
        for (int v = 0; v < numVertices; ++v) {
            if (!visited[v] && graph[u][v] != INF) {
                int distThroughU = distance[u] + graph[u][v];
                distance[v] = min(distance[v], distThroughU);
            }
        }
    }
}

int main() {
    // Example graph
    vector<vector<int>> graph ={
        {0, 2, INF, 1},
        {2, 0, 1, INF},
        {INF, 1, 0, 1},
        {1, INF, 1, 0}
    };

    int numVertices = graph.size();
    int source = 0;  // Source vertex

    vector<int> distance;

    Dijkstra(graph, source, distance);

    cout << "Shortest distances from source vertex " << source << ":" << endl;
    for (int i = 0; i < numVertices; ++i)
        cout << "Vertex " << i << ": " << distance[i] << endl;

    return 0;
}
