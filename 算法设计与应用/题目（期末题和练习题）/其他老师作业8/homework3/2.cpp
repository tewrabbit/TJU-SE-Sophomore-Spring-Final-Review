#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

int removeOverlapping(vector<vector<int> >& intervals) {
    if (intervals.empty()) {
        return 0;
    }

    //按照结束时间排序
    sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    });
    //记录上一个时间段的结束时间
    int end = intervals[0][1];
    int removeNum = 0;
    for (auto i = intervals.begin() + 1; i != intervals.end(); ++i) {
        if ((*i)[0] < end) {
            removeNum++;
        }
        else {
            end = (*i)[1];
        }
    }
    return removeNum;
}

int main() {
    vector<vector<int> > intervals;
    //input
    while (cin.peek() != '\n') {
        vector<int> temp(2);
        cin >> temp[0] >> temp[1];
        intervals.push_back(temp);
    }
    
    cout << removeOverlapping(intervals);
    return 0;
}