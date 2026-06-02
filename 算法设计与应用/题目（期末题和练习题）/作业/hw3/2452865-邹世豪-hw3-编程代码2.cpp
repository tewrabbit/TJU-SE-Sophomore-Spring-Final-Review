#include <iostream>
#include <vector>
using namespace std;

bool canPartition(vector<int>& nums) {
    int sum = 0;

    for (int x : nums) {
        sum += x;
    }

    if (sum % 2 != 0) {
        return false;
    }

    int target = sum / 2;
    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int num : nums) {
        for (int j = target; j >= num; j--) {
            dp[j] = dp[j] || dp[j - num];
        }
    }

    return dp[target];
}

int main() {
    int len;
    cin >> len;

    vector<int> nums(len);
    for (int i = 0; i < len; i++) {
        cin >> nums[i];
    }

    if (canPartition(nums)) {
        cout << "true" << endl;
    }
    else {
        cout << "false" << endl;
    }

    return 0;
}