#include <iostream>
#include <limits>
#include <vector>
#include <omp.h>
#include <mpi.h>

using namespace std;
#define INF numeric_limits<int>::max()

void parallelDijkstra(const vector<int> &graph, int source, vector<int> &distance) {
    int numVertices = graph.size();
    int numProcesses, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numLocalVertices = numVertices / numProcesses;
    int startVertex = rank * numLocalVertices;
    int endVertex = startVertex + numLocalVertices - 1;
    if (rank == numProcesses - 1) {
        endVertex = numVertices - 1;
        numLocalVertices = endVertex - startVertex + 1;
    }

    vector<bool> visited(numVertices, false);
    vector<int> localDistance(numLocalVertices, INF);

    // Initialize local distance array
    for (int i = startVertex; i <= endVertex; ++i) {
        if (i == source)
            localDistance[i - startVertex] = 0;
        else
            localDistance[i - startVertex] = INF;
    }

    for (int i = 0; i < numVertices; ++i) {
        int u, minDist = INF;
        
        // Find the vertex with the minimum distance among unvisited vertices
        for (int j = startVertex; j <= endVertex; ++j) {
            if (!visited[j] && localDistance[j - startVertex] < minDist) {
                minDist = localDistance[j - startVertex];
                u = j;
            }
        }

        // Broadcast the minimum distance and vertex from the process that owns it
        int minDistProcess;
        MPI_Allreduce(&minDist, &minDistProcess, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        MPI_Bcast(&u, 1, MPI_INT, minDistProcess / numLocalVertices, MPI_COMM_WORLD);

        if (u >= startVertex && u <= endVertex) {
            visited[u] = true;

            // Relax edges connected to the selected vertex
            #pragma omp parallel for
            for (int v = 0; v < numVertices; ++v) {
                if (!visited[v] && graph[u][v] != INF) {
                    int distThroughU = localDistance[u - startVertex] + graph[u][v];
                    if (distThroughU < localDistance[v - startVertex])
                        localDistance[v - startVertex] = distThroughU;
                }
            }
        }
    }

    // Gather local distances from all processes
    vector<int> allDistances(numVertices);
    MPI_Allgather(&localDistance[0], numLocalVertices, MPI_INT, &allDistances[0], numLocalVertices, MPI_INT, MPI_COMM_WORLD);

    // Copy local distances to the output distance array
    for (int i = 0; i < numVertices; ++i)
        distance[i] = allDistances[i];
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Example graph
    vector<vector<int>> graph = {
        {0, 2, INF, 1},
        {2, 0, 1, INF},
        {INF, 1, 0, 1},
        {1, INF, 1, 0}
    };

    int numVertices = graph.size();
    vector<int> distance(numVertices);

    int source = 0;  // Source vertex

    parallelDijkstra(graph, source, distance);

    if (rank == 0) {
        cout << "Shortest distances from source vertex " << source << ":" << endl;
        for (int i = 0; i < numVertices; ++i)
            cout << "Vertex " << i << ": " << distance[i] << endl;
    }

    MPI_Finalize();
    return 0;
}
