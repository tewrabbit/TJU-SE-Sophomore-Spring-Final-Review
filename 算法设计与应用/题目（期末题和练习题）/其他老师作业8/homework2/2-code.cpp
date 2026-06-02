#include<iostream>
#include<vector>
#include<string>
using namespace std;

#define min(x,y) (((x)<(y))?(x):(y))

int main() {
	string start, target;
	cin >> start >> target;

	int n = start.size();

	//存储每一位的数字，为简化处理设置大小为n+1
	vector<int> a(n + 3, 0);
	vector<int> b(n + 3, 0);
	//初始化
	for (int i = 1; i <= n; ++i) {
		a[i] = start[i - 1] - '0';
		b[i] = target[i - 1] - '0';
	}

	//dp[i][x][y]表示前i位已归位，第i+1位为x，第i+2位为y需要最小移动次数
	vector<vector<vector<int> > > dp(n + 1,
		vector<vector<int> >(10, vector<int>(10, 10*n)));
	
	//边界条件:初始状态
	dp[0][a[1]][a[2]] = 0;

	for (int i = 1; i <= n; ++i) {
		for (int x = 0; x <= 9; ++x) {
			for (int y = 0; y <= 9; ++y) {
				
				//将第i位从x移动到目标b[i]需要的向上移动次数
				int  upperDis = (b[i] - x + 10) % 10;
				for (int j = 0; j <= upperDis; ++j) {
					for (int k = 0; k <= j; ++k) {
						dp[i][(y + j) % 10][(a[i + 2] + k) % 10] = min(dp[i][(y + j) % 10][(a[i + 2] + k) % 10], dp[i - 1][x][y] + upperDis);
					}
				}

				//将第i位从x移动到目标b[i]需要的向下移动次数
				int lowerDis = 10 - upperDis;
				for (int j = 0; j <= lowerDis; ++j) {
					for (int k = 0; k <= j; ++k) {
						dp[i][(y - j + 10) % 10][(a[i + 2] - k + 10) % 10] = min(dp[i][(y - j + 10) % 10][(a[i + 2] - k + 10) % 10], dp[i - 1][x][y] + lowerDis);
					}
				}

			}
		}
	}

	//前i位归位即为结果
	cout << dp[n][0][0];
	return 0;
	
}