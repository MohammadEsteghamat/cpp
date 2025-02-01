#include <iostream>
#include <vector>
using namespace std;

int orangesRotting(vector<vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    int freshCount = 0;
    int time = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 1) {
                freshCount++;
            }
        }
    }

    if (freshCount == 0) return 0;

    bool hasRotten = true;
    while (hasRotten) {
        hasRotten = false;
        vector<vector<int>> temp = grid;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 2) {
                    if (i > 0 && grid[i-1][j] == 1) {
                        temp[i-1][j] = 2;
                        freshCount--;
                        hasRotten = true;
                    }
                    if (i < rows - 1 && grid[i+1][j] == 1) {
                        temp[i+1][j] = 2;
                        freshCount--;
                        hasRotten = true;
                    }
                    if (j > 0 && grid[i][j-1] == 1) {
                        temp[i][j-1] = 2;
                        freshCount--;
                        hasRotten = true;
                    }
                    if (j < cols - 1 && grid[i][j+1] == 1) {
                        temp[i][j+1] = 2;
                        freshCount--;
                        hasRotten = true;
                    }
                }
            }
        }
        grid = temp;
        if (hasRotten) time++;
    }

    return freshCount == 0 ? time : -1;
}

int main() {
    vector<vector<int>> grid1 = {
            {0,1,2},
            {0,1,2},
            {2,1,1},
            {1,2,0},
            {1,1,1}
    };

    cout << orangesRotting(grid1) << endl;

    return 0;
}
