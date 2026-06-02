#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// 微分方程 y' = f(x, y)
double f(double x, double y) {
    return cos(x);
}

int main() {
    const double PI = acos(-1.0);

    // 题目给定条件
    double x_start = 0.0;
    double x_end = 2.0 * PI;
    double h = 0.1;
    double x = x_start;
    double y = 0.0;   // 初值 y(0) = 0

    double mse = 0.0;
    int count = 0;

    cout << fixed << setprecision(10);
    cout << "RK4 numerical solution for y' = cos(x), y(0) = 0" << endl;
    cout << "Exact solution: y = sin(x)" << endl;
    cout << "Interval: [0, 2pi], h = 0.1" << endl << endl;
    cout << "x\t\tNumerical y\tExact y\t\tError" << endl;

    while (true) {
        double exact = sin(x);
        double error = y - exact;

        mse += error * error;
        count++;

        cout << x << "\t" << y << "\t" << exact << "\t" << error << endl;

        if (x + 1e-12 >= x_end) {
            break;
        }

        // 为了确保最后一个点正好到 2pi
        double step = min(h, x_end - x);

        double k1 = step * f(x, y);
        double k2 = step * f(x + step / 2.0, y + k1 / 2.0);
        double k3 = step * f(x + step / 2.0, y + k2 / 2.0);
        double k4 = step * f(x + step, y + k3);

        y = y + (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
        x = x + step;
    }

    mse /= count;

    cout << endl;
    cout << "MSE = " << mse << endl;

    return 0;
}