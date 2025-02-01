#include <iostream>
#include <vector>
using namespace std;


int main() {
    int n, k;



    cout << "Enter the number of people (n): ";
    cin >> n;
    cout << "Enter the number (k): ";
    cin >> k;

    vector<int>table;
    for (int i{0}; i < n; ++i) {
        table.push_back(i + 1);
    }

    int del{0},del2{0};
    while (table.size() != 1){
        if(del2 > table.size()) del2 - table.size();
        del = del2;
        table.erase(table.begin() + del);
        del2 += k;
    }
    cout<<table[0];

    return 0;
}