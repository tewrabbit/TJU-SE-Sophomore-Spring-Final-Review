#include<iostream>
#include<cmath>
#include<vector>
using namespace std;

int main() {
	int n;
	cin >> n;
	if (n == 0) {
		cout << 1;
		return 0;
	}
	vector<int> dp(n + 1, 0);
	dp[1] = 0;
	for (int i = 2; i <= n; ++i) {
		dp[i] = 10 * dp[i - 1] + (9 * pow(10, i - 2) - dp[i - 1]) * (i - 1);
	}
	int sums = 0;
	for (int i = 1; i <= n; ++i) {
		sums += dp[i];
	}
	cout << pow(10, n) - sums;
	return 0;
}