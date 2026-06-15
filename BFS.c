#include <stdio.h>
#include <stdbool.h>

#define MAX_N 10

int graph[MAX_N][MAX_N];
int visited[MAX_N] = {0};

// In BFS, to print the path at the end, we track the 'parent' of each node
int parent[MAX_N]; 
bool foundPath = false;

void getWeights(int n) {
    printf("Enter the weight of the path (0 if no edge):\n");
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            printf("Enter weight for edge %d-%d: ", i, j);
            scanf("%d", &graph[i][j]);
            graph[j][i] = graph[i][j]; 
        }
        graph[i][i] = 0;
    }
}

// Function to reconstruct and print the path from start to destination using parents
void printBFSPath(int start, int destination) {
    int path[MAX_N];
    int pathLength = 0;
    int current = destination;
    int totalWeight = 0;

    // Backward trace from destination to start using our parent map
    while (current != -1) {
        path[pathLength++] = current;
        if (parent[current] != -1) {
            totalWeight += graph[parent[current]][current];
        }
        current = parent[current];
    }

    printf("\nFound path: ");
    for (int i = pathLength - 1; i >= 0; i--) {
        printf("%d%s", path[i], (i > 0) ? " -> " : "");
    }
    printf("\nTotal weight: %d\n", totalWeight);
}

void bfs(int start, int destination, int n) {
    // Create an explicit queue array
    int queue[MAX_N];
    int front = 0;
    int rear = 0;

    // Initialize all parents to -1 (meaning no parent yet)
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
    }

    // Enqueue start node
    queue[rear++] = start;
    visited[start] = 1;

    while (front < rear) {
        // Dequeue current node
        int current = queue[front++];

        // If destination reached, print path and stop search
        if (current == destination) {
            foundPath = true;
            printBFSPath(start, destination);
            return;
        }

        // Look at all neighbors layer by layer
        for (int next = 0; next < n; next++) {
            if (graph[current][next] != 0 && !visited[next]) {
                visited[next] = 1;
                parent[next] = current; // Remember where we came from
                queue[rear++] = next;   // Enqueue neighbor
            }
        }
    }
}

int main() {
    int startNode, destinationNode, n;
    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    if (n > MAX_N) {
        printf("Limit is %d vertices.\n", MAX_N);
        return 1;
    }
    
    getWeights(n);
    
    printf("Enter start node (0-%d): ", n - 1);
    scanf("%d", &startNode);
    printf("Enter destination node (0-%d): ", n - 1);
    scanf("%d", &destinationNode);
    
    if (startNode < 0 || startNode >= n || destinationNode < 0 || destinationNode >= n) {
        printf("Invalid node values.\n");
        return 1;
    }

    bfs(startNode, destinationNode, n);

    if (!foundPath) {
        printf("No path found from %d to %d.\n", startNode, destinationNode);
    }

    return 0;
}