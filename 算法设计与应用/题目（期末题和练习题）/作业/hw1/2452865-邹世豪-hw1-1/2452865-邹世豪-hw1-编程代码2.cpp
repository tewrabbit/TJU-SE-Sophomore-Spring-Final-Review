#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

const double EPS = 1e-12;

// 线性时间选择：返回 arr 中第 k 小的元素（0-based）
double selectKth(vector<double> arr, int k) {
    int n = (int)arr.size();

    if (n <= 5) {
        sort(arr.begin(), arr.end());
        return arr[k];
    }

    // 每 5 个一组，取每组中位数
    vector<double> medians;
    for (int i = 0; i < n; i += 5) {
        int r = min(i + 5, n);
        vector<double> group(arr.begin() + i, arr.begin() + r);
        sort(group.begin(), group.end());
        medians.push_back(group[group.size() / 2]);
    }

    // 递归求中位数的中位数作为枢轴
    double pivot = selectKth(medians, (int)medians.size() / 2);

    vector<double> less, equal, greater;
    less.reserve(n);
    equal.reserve(n);
    greater.reserve(n);

    for (double x : arr) {
        if (x < pivot) less.push_back(x);
        else if (x > pivot) greater.push_back(x);
        else equal.push_back(x);
    }

    if (k < (int)less.size()) {
        return selectKth(less, k);
    }
    else if (k < (int)less.size() + (int)equal.size()) {
        return pivot;
    }
    else {
        return selectKth(greater, k - (int)less.size() - (int)equal.size());
    }
}

// 递归求最终水面高度
// active: 当前尚未确定的候选高度集合
// water: 当前剩余水量
// fixedCount: 已确定会在最终水面以下（或恰在水面）的区域个数
// fixedSum: 这些已处理区域的高度和
double solveWaterLevel(const vector<double>& active, double water,
    long long fixedCount, double fixedSum) {
    if (active.empty()) {
        // 所有区域都已经在水面以下，直接解方程 fixedCount * H - fixedSum = water
        return (water + fixedSum) / fixedCount;
    }

    double pivot = selectKth(active, (int)active.size() / 2);

    vector<double> less, greater;
    long long equalCount = 0;
    double sumLess = 0.0;

    less.reserve(active.size());
    greater.reserve(active.size());

    for (double x : active) {
        if (x < pivot) {
            less.push_back(x);
            sumLess += x;
        }
        else if (x > pivot) {
            greater.push_back(x);
        }
        else {
            equalCount++;
        }
    }

    // 把“已处理区域 + less 区域”抬升到 pivot 所需的水量
    double need = (fixedCount + (long long)less.size()) * pivot - (fixedSum + sumLess);

    if (fabs(water - need) <= EPS) {
        return pivot;
    }
    else if (water < need) {
        // 水不够到 pivot，答案在 less 中
        return solveWaterLevel(less, water, fixedCount, fixedSum);
    }
    else {
        // 水超过 pivot，less 和 equal 都并入已处理区域
        long long newFixedCount = fixedCount + (long long)less.size() + equalCount;
        double newFixedSum = fixedSum + sumLess + equalCount * pivot;
        return solveWaterLevel(greater, water - need, newFixedCount, newFixedSum);
    }
}

double computeWaterLevelOptimal(const vector<double>& a, double M) {
    return solveWaterLevel(a, M, 0, 0.0);
}

int main() {
    int n;
    cout << "Please input n: ";
    cin >> n;

    vector<double> a(n);
    cout << "Please input " << n << " pit heights:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    double M;
    cout << "Please input total water amount M: ";
    cin >> M;

    double H = computeWaterLevelOptimal(a, M);

    cout << fixed << setprecision(6);
    cout << "Final water level = " << H << endl;

    return 0;
}