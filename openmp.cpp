#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
#include <time.h>  



using namespace std;

#define Infinity numeric_limits<int>::max()

void Dijkstra(const vector<vector<int>> &graph, int source, vector<int> &distance) {
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

int main() {

clock_t t1,t2;





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

t1 = clock();
    Dijkstra(graph, source, distance);


t2 = clock();

double time_dif = (double)(t2 - t1)/CLOCKS_PER_SEC;
    cout << "Shortest distances from source vertex " << source << ":" << endl;
    for (int i = 0; i < numVertices; i++)
        cout << "Vertex " << i << ": " << distance[i] << endl;
        cout<< time_dif<<endl;

    return 0;
}
