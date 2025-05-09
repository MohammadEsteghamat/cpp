#include <iostream>

using namespace std;
int countOnes(int n) {
    int count = 0;
    if (n == 0) {
        return 0;
    }
    while (n > 0) {
       count += n % 2;
        n /= 2;
    }
    return count;
}
int main() {
    int n;
    cin >> n;
    cout << '[';
    for (int i = 0; i <= n; ++i) {
        cout << countOnes(i);
        if(i!=n) cout << " ,";
    }
    cout << ']' <<endl;

    return 0;
}