#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int>& nums, int left, int right) {
    int pivot = nums[right];
    int i = left;

    for (int j = left; j < right; j++) {
        if (nums[j] >= pivot) {
            swap(nums[i], nums[j]);
            i++;
        }
    }

    swap(nums[i], nums[right]);
    return i;
}

int quickSelect(vector<int>& nums, int left, int right, int k) {
    while (left <= right) {
        int pos = partition(nums, left, right);

        if (pos == k - 1) {
            return nums[pos];
        }
        else if (pos > k - 1) {
            right = pos - 1;
        }
        else {
            left = pos + 1;
        }
    }

    return -1;
}

int main() {
    int n, k;
    cin >> n >> k;

    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }

    cout << quickSelect(nums, 0, n - 1, k) << endl;

    return 0;
}