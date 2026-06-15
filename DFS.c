#include <stdio.h>
#include <stdbool.h>
#define MAX_N 10
int graph[MAX_N][MAX_N];
int visited[MAX_N] = {0};
int path[MAX_N];
bool foundPath = false;
void getWeights(int n) {
    printf("Enter the weight of the path (0 if no edge):\n");
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) { // Start j from i+1 to avoid self-loops
            printf("Enter weight for edge %d-%d: ", i, j);
            scanf("%d", &graph[i][j]);
            // Symmetry: Make it undirected
            graph[j][i] = graph[i][j]; 
        }
        graph[i][i] = 0; // Ensure diagonal is 0
    }
}
void dfs(int current, int destination, int depth, int currentWeight, int n) {
    visited[current] = 1;
    path[depth] = current;
    if (current == destination) {
        printf("\nFound path: ");
        for (int i = 0; i <= depth; i++) {
            printf("%d%s", path[i], (i < depth) ? " -> " : "");
        }
        printf("\nTotal weight: %d\n", currentWeight);
        foundPath = true;
        visited[current] = 0;
        return;
    }
    // Loop up to 'n', not the global 'MAX_N'
    for (int next = 0; next < n; next++) {
        if (graph[current][next] != 0 && !visited[next]) {
            dfs(next, destination, depth + 1, currentWeight + graph[current][next], n);
            if (foundPath) {
                visited[current] = 0;
                return;
            }
        }
    }
    visited[current] = 0; // Backtrack
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
    dfs(startNode, destinationNode, 0, 0, n);
    if (!foundPath) {
        printf("No path found from %d to %d.\n", startNode, destinationNode);
    }

    return 0;
}