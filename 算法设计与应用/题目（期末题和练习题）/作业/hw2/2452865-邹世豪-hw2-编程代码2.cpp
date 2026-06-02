#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
using namespace std;

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int m = nums1.size();
    int n = nums2.size();

    // 保证 nums1 是较短的数组
    if (m > n) {
        return findMedianSortedArrays(nums2, nums1);
    }

    int left = 0, right = m;
    int totalLeft = (m + n + 1) / 2;

    while (left <= right) {
        int i = (left + right) / 2;
        int j = totalLeft - i;

        int nums1Left = (i == 0) ? INT_MIN : nums1[i - 1];
        int nums1Right = (i == m) ? INT_MAX : nums1[i];

        int nums2Left = (j == 0) ? INT_MIN : nums2[j - 1];
        int nums2Right = (j == n) ? INT_MAX : nums2[j];

        if (nums1Left <= nums2Right && nums2Left <= nums1Right) {
            if ((m + n) % 2 == 1) {
                return max(nums1Left, nums2Left);
            }
            else {
                return (max(nums1Left, nums2Left) + min(nums1Right, nums2Right)) / 2.0;
            }
        }
        else if (nums1Left > nums2Right) {
            right = i - 1;
        }
        else {
            left = i + 1;
        }
    }

    return 0.0;
}

int main() {
    int m, n;
    cin >> m >> n;

    vector<int> nums1(m), nums2(n);

    for (int i = 0; i < m; i++) {
        cin >> nums1[i];
    }

    for (int i = 0; i < n; i++) {
        cin >> nums2[i];
    }

    double ans = findMedianSortedArrays(nums1, nums2);

    cout << fixed << setprecision(4) << ans << endl;

    return 0;
}