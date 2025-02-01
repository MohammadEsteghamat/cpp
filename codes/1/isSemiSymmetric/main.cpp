#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::cin;
using std::string;


bool isSemiSymmetric(const string&);

int main() {
    string a;
    cin >> a;

    cout<<isSemiSymmetric(a);

    return 0;
}
bool isSemiSymmetric(const string& s) {
    int n = s.size();

    if (n == 0) return true;
    else if(n ==1) return false;

    for (int i = 0; i < n / 2; i++) {
        string left = s.substr(0, i + 1);
        string right = s.substr(n - i - 1);


        if (left == right) {
            return isSemiSymmetric(s.substr(i + 1, n - 2 * (i + 1)));
        }
    }

    return !s.size();
}
