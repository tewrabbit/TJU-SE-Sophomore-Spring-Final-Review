#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int numSquares(int n) {
    vector<int> dp(n + 1, INT_MAX);
    dp[0] = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j * j <= i; j++) {
            int square = j * j;
            if (dp[i - square] != INT_MAX) {
                dp[i] = min(dp[i], dp[i - square] + 1);
            }
        }
    }

    return dp[n];
}

int main() {
    int n;
    cin >> n;

    cout << numSquares(n) << endl;

    return 0;
}