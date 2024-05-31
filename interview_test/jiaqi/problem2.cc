#include <iostream>
#include <set>
#include <vector>

using namespace std;

/*
On a square integer lattice of size N by N, 
we can move according to the following rules
•We always begin at (0,0).
•We must always move first to (1,0).
•We will move N−1 additional steps.  Each time, 
    we can go from (a, b) to any of these fourpoints:{(a+ 1, b), (a−1, b), (a, b+ 1), (a, b−1)}.
•We may not move to the same point twice.
•We may not move outside the lattice.Your goal is to implement the solveProblem2() function 
    that takes N as input, and outputs the number of unique routes 
    that we can take through a lattice of size N


    DFS + memoization
    dp dynamic programming
    
*/
long long solveProblem2 (int N) {
    if (N == 1){
        return 1;
    }

    // dp[i][j] = dp[i-1][j] + dp[i][j-1]
    vector<vector<long long>> dp(N, vector<long long>(N, 0));
    dp[0][0] = 1;
    for (int i = 1; i < N; i++){
        dp[i][0] = 1;
        dp[0][i] = 1;
    }

    // solve for [5,25]
    return 0;
}

int main ()
{
    for (int i = 5; i <= 25; i++)
    {
        cout << solveProblem2(i) << endl;
    }
    return 0;
}
