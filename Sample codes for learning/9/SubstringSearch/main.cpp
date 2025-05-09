#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> findPattern(string& txt, string& pat) {
    vector<int>index;
    for (int i = 0; i < txt.length(); ++i) {
        if(96 < int(txt.at(i))) txt.at(i) =  int(txt.at(i)) - 32;
    }
    for (int i = 0; i < pat.length(); ++i) {
        if(96 < int(pat.at(i))) pat.at(i) =  int(pat.at(i)) - 32;
    }
    bool flg;
    for (int i = 0 ; i < txt.length() - pat.length() + 1 ; ++i) {
        flg = true;
        for (int j = 0; j < pat.length(); ++j) {
            if(txt.at(i + j) != pat.at(j)){
                flg = false;
                break;
            }
        }
        if(flg) index.push_back(i);
    }
    return index;
}

int main() {
    string txt, pat;
    txt = "aAAa";
    pat = "Aa";
    vector<int>a;
    a = findPattern(txt,pat);

    cout << "[";
    for (int i = 0; i < a.size(); ++i) {
        cout << a[i];
        if (i != a.size() -1) cout << ",";
    }
    cout << "]";




    return 0;
}