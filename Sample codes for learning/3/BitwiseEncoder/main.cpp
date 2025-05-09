#include <iostream>
using namespace std;
#define decode 0b01111111
#define decode1 0b10000000
int main()
{
    int n;
    char number[7];
    for (int i = 0; i < 7; i++)
    {
        cin >> n;
        number[i] = n + 64;
    }
    cin >> n;
    n += 64;
    for (int i = 0; i < 7; i++)
    {
        number[i] |= ((n % 2) << 7);
        n /= 2;
    }
    n = 0;
    for (int i = 0; i < 7; i++)
    {
        cout << int(number[i] & decode) - 64 << endl;
        n |= ((number[i] & decode1) >> 7 - i);
    }
    cout << n - 64;
    return 0;
}