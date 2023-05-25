#include <iostream>
#include <vector>
#include <limits>
#include <mpi.h>

using namespace std;

#define Infinity numeric_limits<int>::max()

void Dijkstra(const vector<vector<int>> &graph, int source, vector<int> &distance, int numProcesses, int rank) {
    int numVerts = graph.size();
    vector<bool> visited(numVerts, false);

    // Set the distance of the source vertex to 0
    distance[source] = 0;

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

        // Broadcast the selected vertex u to all processes
        MPI_Bcast(&u, 1, MPI_INT, 0, MPI_COMM_WORLD);

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

        // Reduce the distance vector across all processes
        MPI_Allreduce(MPI_IN_PLACE, distance.data(), numVerts, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    // Example graph
    vector<vector<int>> graph = {
        {0, 2, Infinity, 1},
        {2, 0, 1, Infinity},
        {Infinity, 1, 0, 1},
        {1, Infinity, 1, 0}
    };

    int numVertices = graph.size();
    int source = 0;  // Source vertex

    // Divide the vertices among processes
    int verticesPerProcess = numVertices / numProcesses;
    int startVertex = rank * verticesPerProcess;
    int endVertex = startVertex + verticesPerProcess;

    vector<int> distance(numVertices, Infinity);  // Resize the distance vector

    // Compute shortest paths using Dijkstra algorithm
    Dijkstra(graph, source, distance, numProcesses, rank);

    // Gather and print the shortest distances
    if (rank == 0) {
        vector<int> allDistances(numVertices);
        MPI_Gather(distance.data() + startVertex, verticesPerProcess, MPI_INT,
                   allDistances.data(), verticesPerProcess, MPI_INT, 0, MPI_COMM_WORLD);

        cout << "Shortest distances from source vertex " << source << ":" << endl;
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i << ": " << allDistances[i] << endl;
        }
    } else {
        MPI_Gather(distance.data() + startVertex, verticesPerProcess, MPI_INT,
                   nullptr, 0, MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
