#include<stdio.h>
int main(){
    int graph[5][5] = {
        {0, 1, 0, 0, 1},
        {1, 0, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 0, 1},
        {1, 0, 0, 1, 0}
    };
    int visited[5] = {0};
    int stack[5];
    int top = -1;
    int startNode = 0;

    stack[++top] = startNode;

    while (top >= 0) {
        int currentNode = stack[top--];
        if (!visited[currentNode]) {
            printf("Visited Node: %d\n", currentNode);
            visited[currentNode] = 1;

            for (int i = 4; i >= 0; i--) {
                if (graph[currentNode][i] == 1 && !visited[i]) {
                    stack[++top] = i;
                }
            }
        }
    }

    return 0;
}