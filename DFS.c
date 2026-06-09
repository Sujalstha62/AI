#include <stdio.h>
#include <stdbool.h>

#define N 10

int graph[N][N];
int visited[N] = {0};
int path[N];
bool foundPath = false;
void weight(int n){
    printf("Enter the weight of the path:\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j){
                graph[i][j]=0;
                continue;
            }
            printf("Enter weight for edge %d-%d (0 if no edge): ", i, j);
            scanf("%d", &graph[i][j]);
        }
        printf("\n");
    }
}
void dfs(int current, int destination, int depth, int currentWeight) {
    visited[current] = 1;
    path[depth++] = current;

    if (current == destination) {
        printf("Found path: ");
        for (int i = 0; i < depth; i++) {
            printf("%d", path[i]);
            if (i < depth - 1) printf(" -> ");
        }
        printf("\nTotal weight: %d\n", currentWeight);
        foundPath = true;
        visited[current] = 0;
        return;
    }

    for (int next = 0; next < N; next++) {
        if (graph[current][next] != 0 && !visited[next]) {
            dfs(next, destination, depth, currentWeight + graph[current][next]);
            if (foundPath) {
                visited[current] = 0;
                return;
            }
        }
    }

    visited[current] = 0;
}

int main() {
    int startNode, destinationNode,n;
    printf("Enter the number of vertex:");
    scanf("%d",&n);
    printf("Enter start node (0-%d): ", n - 1);
    scanf("%d", &startNode);
    printf("Enter destination node (0-%d): ", n - 1);
    scanf("%d", &destinationNode);
    void weight(n);
    if (startNode < 0 || startNode >= n || destinationNode < 0 || destinationNode >= n) {
        printf("Invalid node values. Please use values between 0 and %d.\n", n - 1);
        return 1;
    }

    dfs(startNode, destinationNode, 0, 0);

    if (!foundPath) {
        printf("No path found from %d to %d.\n", startNode, destinationNode);
    }

    return 0;
}
