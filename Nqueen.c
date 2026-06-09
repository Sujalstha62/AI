#include<stdio.h>
#include<stdbool.h>
void printBoard(int board[10][10],int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(board[i][j]==1){
                printf("Q ");
            }
            else{
                printf("X ");
            }
        }
        printf("\n");
    }
}
bool issafe(int board[10][10],int row,int col,int n){
    for(int i=0;i<col;i++){
        if(board[row][i]==1){
            return false;
        }
    }
    for(int i=row,j=col;i>=0&&j>=0;i--,j--){
        if(board[i][j]==1){
            return false;
        }
    }
    for(int i=row,j=col;j>=0&&i<n;i++,j--){
        if(board[i][j]==1){
            return false;
        }
    }
    return true;
}
bool placeQueen(int board[10][10], int col, int n) {
    if (col >= n) {
        return true; 
    }
    for (int row = 0; row < n; row++) {
        if (issafe(board, row, col, n)) {
            board[row][col] = 1;
            if (placeQueen(board, col + 1, n)) {
                return true;
            }
            board[row][col] = 0; 
        }
    }
    return false;
}
int main(){
    int board[10][10],n;
    printf("Enter the number of queens:");
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            board[i][j]=0;
        }
    }
    if(placeQueen(board, 0, n)) {
        printBoard(board, n);
    } else {
        printf("Solution does not exist");
    }
}