#include <iostream>
#include <cmath>

using namespace std;

const double r = 1.7;
const double pi = 3.14159265359;

int main()
{
    double l, s;

    cout << "Enter L (distance between axles in meters): ";
    cin >> l;

    // R = L / tan(s) => s = atan(L / r)
    s = atan(l / r);
    cout << "s (steering angle) = " << s * 180 / pi << " degrees" << endl;
    return 0;
}
